#version 440 core

uniform bool height_coloring;

uniform float min_height;
uniform float max_height;

in vec3 gPosition;
in vec3 gColor;

out vec4 FragColor;

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

void main() {
   
    if (height_coloring) {
    
       FragColor = vec4(color_based_off_height(gPosition.z, min_height, max_height), 1.0f);
    
    } else {
    
      FragColor = vec4(gColor, 1.0f);
    
    };

};