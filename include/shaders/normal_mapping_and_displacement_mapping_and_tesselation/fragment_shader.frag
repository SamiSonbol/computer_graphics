#version 440 core

uniform vec3 light_position;
uniform vec3 light_color;
uniform vec3 eye_vector;

uniform float ambient;
uniform float diffuse;
uniform float specular;
uniform float shininess;
uniform bool gamma_correction;

uniform sampler2D uNormal_map;
uniform bool normal_mapping;

uniform sampler2D uTexture;
uniform bool texturing;

in vec3 tColor;
in vec3 tPosition;
in vec3 tNormal;
in vec2 tTexture_coordinates;
in vec3 tTangent;
in vec3 tBitangent;

out vec4 FragColor;

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

void main() {
   
    float distance_from_light = length(light_position - tPosition);
    float light_intensity = calculate_light_intensity(distance_from_light, 100.0, 0.0, 0.1, 0.1);

    vec3 Color = tColor;
    if (texturing) {

       Color = texture(uTexture, tTexture_coordinates).rgb;

    };

    vec3 Normal = tNormal;
	if (normal_mapping) {

		vec3 sampled_normal = texture(uNormal_map, tTexture_coordinates).rgb * 2.0 - 1.0;//getting the normal from the normal map and making it in the range of [-1, 1]
		mat3 TBN = mat3(tTangent, tBitangent, tNormal);
		Normal = normalize(TBN * sampled_normal);

	};

    vec3 phong = calculate_phong_lighting(tPosition, Normal, eye_vector, light_position, light_color, light_intensity, Color, ambient, diffuse, specular, shininess);
    if (gamma_correction) {

        vec3 gamma = vec3(1.0 / 2.2);
        phong = pow(phong, gamma);   

     };

    FragColor = vec4(phong, 1.0);

};