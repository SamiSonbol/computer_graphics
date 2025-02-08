#version 440 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec2 aTexture_coordinates;
layout(location = 4) in vec3 aTangent;
layout(location = 5) in vec3 aBitangent;

uniform mat4 model_transformation_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform bool displacement_mapping;
uniform sampler2D uDisplacement_map;
uniform float displacement_scale;

out vec3 vPosition;
out vec3 vNormal;
out vec3 vColor;
out vec2 vTexture_coordinates;
out vec3 vTangent;
out vec3 vBitangent;

vec3 displace(vec3 position, vec3 normal, float displacement_offset) {

    return position + normal * displacement_offset; 

}; 

void main() {
 
    if (displacement_mapping) {

	  float displacement_offset = texture(uDisplacement_map, aTexture_coordinates).r * displacement_scale;
	  vPosition = displace(aPosition, normalize(aNormal), displacement_offset);   
         
    } else {
    
      vPosition = aPosition;
    
    };

    mat3 model_matrix = transpose(inverse(mat3(model_transformation_matrix)));
    vNormal = normalize(model_matrix * aNormal);
    vTangent = normalize(model_matrix * aTangent);
    vBitangent = normalize(model_matrix * aBitangent);
    vTexture_coordinates = aTexture_coordinates;
    vColor = aColor;

    gl_Position = projection_matrix * view_matrix * model_transformation_matrix * vec4(vPosition, 1.0);
    
};
