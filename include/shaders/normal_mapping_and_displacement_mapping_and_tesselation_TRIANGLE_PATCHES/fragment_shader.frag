#version 440 core

uniform bool gamma_correction;
uniform bool height_coloring;
uniform bool normal_mapping;
uniform bool texturing;

uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 camera_position;
uniform vec3 mouse_ray_vector;

uniform float ambient;
uniform float diffuse;
uniform float specular;
uniform float shininess;

uniform float min_height;
uniform float max_height;

uniform sampler2D uNormal_map;
uniform sampler2D uTexture;

in vec3 tColor;
in vec3 tPosition;
in vec3 tNormal;
in vec2 tTexture_coordinates;
in vec3 tTangent;
in vec3 tBitangent;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 FragPosition;

float calculate_light_intensity(float distance_from_light, float initial_light_intensity, float constant_attenuation_component, float linear_attenuation_coefficient, float quadratic_attenuation_coefficient) {

    //constant_attenuation_component is overall light intensity, works like a multiplier to ambient light
    //linear_attenuation_coefficient is diminishing light intensity over distance
    //quadratic_attenuation_coefficient is further decreasing intensity with the square of the distance

    float attenuation = constant_attenuation_component + linear_attenuation_coefficient * distance_from_light + quadratic_attenuation_coefficient * pow(distance_from_light, 2);
    return initial_light_intensity / attenuation;

}; 

vec3 calculate_phong_lighting(vec3 frag_position, vec3 normal, vec3 eye_vector, vec3 light_position, vec3 light_color, float light_intensity, vec3 frag_color, float ambient, float diffuse, float specular, float shininess) {

    vec3 view_vector = normalize(eye_vector - frag_position);
    vec3 light_vector = normalize(light_position - frag_position);
    vec3 half_vector = normalize(view_vector + light_vector);

    float cos_alpha = max(dot(normal, light_vector), 0.0);
    float cos_theta_prime = max(dot(normal, half_vector), 0.0);
    
    vec3 Ca = ambient * frag_color;
    vec3 Cd = diffuse * cos_alpha * frag_color * light_intensity;   
    vec3 Cs = specular * pow(cos_theta_prime, shininess) * light_color * light_intensity;

    return Ca + Cd + Cs;

};

vec3 interpolate(float interpolation_factor, vec3 min_value, vec3 max_value) {

  return min_value + (max_value - min_value) * interpolation_factor;

};

vec3 color_based_off_height(float current_height, float min_height, float max_height) {

    vec3 min_color = vec3(0.0, 0.0, 255.0);
    vec3 mid_color = vec3(0.0, 255.0, 0.0);
    vec3 max_color = vec3(255.0, 0.0, 0.0);
    vec3 color = vec3(0.0, 0.0, 0.0);

    float interpolation_factor = (current_height - min_height) / (max_height - min_height);
    interpolation_factor = clamp(interpolation_factor, 0.0, 1.0);

    if (interpolation_factor < 0.5) {

        //interpolate between min_color and mid_color
        interpolation_factor = interpolation_factor * 2;//scale to [0, 2]
        color = interpolate(interpolation_factor, min_color, mid_color);

    } else {
    
        interpolation_factor = (interpolation_factor - 0.5) * 2;//scale to [0, 2]
        color = interpolate(interpolation_factor, mid_color, max_color);

    };

    return color;

};

bool check_for_ray_intersection(vec3 camera_position, vec3 ray_direction, float ray_length, vec3 sphere_position, float radius) {

    vec3 ray_end = camera_position + ray_direction * ray_length;
    float SDF = length(sphere_position - ray_end) - radius;

    return SDF < 0.0; //if SDF < 0.0 means intersection

};

void main() {
   
    float distance_from_light = length(light_position - tPosition);
    float light_intensity = calculate_light_intensity(distance_from_light, 100.0, 0.0, 0.1, 0.1);

    vec3 Color = tColor;
    if (texturing) {

       Color = texture(uTexture, tTexture_coordinates).rgb;

    } else if (height_coloring) {
    
       Color = color_based_off_height(tPosition.z, min_height, max_height);
    
    };
    
    if (check_for_ray_intersection(camera_position, mouse_ray_vector, 1000.0, tPosition, 1.0)) {
    
       Color = vec3(255.0, 0.0, 0.0);

    };

    vec3 Normal = tNormal;
	if (normal_mapping) {

		vec3 sampled_normal = texture(uNormal_map, tTexture_coordinates).rgb * 2.0 - 1.0;//getting the normal from the normal map and making it in the range of [-1, 1]
		mat3 TBN = mat3(tTangent, tBitangent, tNormal);
		Normal = normalize(TBN * sampled_normal);

	};

    vec3 phong = calculate_phong_lighting(tPosition, Normal, camera_position, light_position, light_color, light_intensity, Color, ambient, diffuse, specular, shininess);
    if (gamma_correction) {

        vec3 gamma = vec3(1.0 / 2.2);
        phong = pow(phong, gamma);   

     };

    FragColor = vec4(phong, 1.0);
    FragPosition = vec4(tPosition, 1.0);

};