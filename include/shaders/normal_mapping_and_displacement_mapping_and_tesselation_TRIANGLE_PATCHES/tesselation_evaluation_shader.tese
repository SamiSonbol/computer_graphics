#version 440 core

layout (triangles, equal_spacing, ccw) in;

in vec3 cNormal[];
in vec3 cColor[];
in vec3 cPosition[];
in vec2 cTexture_coordinates[];
in vec3 cTangent[];
in vec3 cBitangent[];

uniform mat4 model_transformation_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform sampler2D uDisplacement_map;
uniform bool displacement_mapping;
uniform float displacement_scale;

out vec3 tNormal;
out vec3 tColor;
out vec3 tPosition;
out vec2 tTexture_coordinates;
out vec3 tTangent;
out vec3 tBitangent;

vec4 interpolate(vec4 v1, vec4 v2, vec4 v3, vec4 v4) {

  vec4 a = mix(v1, v2, gl_TessCoord.x);
  vec4 b = mix(v4, v3, gl_TessCoord.x);
  return mix(a, b, gl_TessCoord.y);

};
vec4 interpolate(vec4 v1, vec4 v2, vec4 v3) {

  return v1 * gl_TessCoord.x + v2 * gl_TessCoord.y + v3 * gl_TessCoord.z;

};

vec3 interpolate(vec3 v1, vec3 v2, vec3 v3, vec3 v4) {

  vec3 a = mix(v1, v2, gl_TessCoord.x);
  vec3 b = mix(v4, v3, gl_TessCoord.x);
  return mix(a, b, gl_TessCoord.y);

};
vec3 interpolate(vec3 v1, vec3 v2, vec3 v3) {

  return v1 * gl_TessCoord.x + v2 * gl_TessCoord.y + v3 * gl_TessCoord.z;

};

vec2 interpolate(vec2 v1, vec2 v2, vec2 v3, vec2 v4) {

  vec2 a = mix(v1, v2, gl_TessCoord.x);
  vec2 b = mix(v4, v3, gl_TessCoord.x);
  return mix(a, b, gl_TessCoord.y);

};
vec2 interpolate(vec2 v1, vec2 v2, vec2 v3) {

  return v1 * gl_TessCoord.x + v2 * gl_TessCoord.y + v3 * gl_TessCoord.z;

};

vec3 displace(vec3 position, vec3 normal, float displacement_offset) {

    return position + normal * displacement_offset; 

}; 

void main() {

  tPosition = interpolate(cPosition[0], cPosition[1], cPosition[2]);
  tNormal = interpolate(cNormal[0], cNormal[1], cNormal[2]);
  tTangent = interpolate(cTangent[0], cTangent[1], cTangent[2]);
  tBitangent = interpolate(cBitangent[0], cBitangent[1], cBitangent[2]);
  tTexture_coordinates = interpolate(cTexture_coordinates[0], cTexture_coordinates[1], cTexture_coordinates[2]);
  tColor = interpolate(cColor[0], cColor[1], cColor[2]);
  
  if (displacement_mapping) {

	float displacement_offset = texture(uDisplacement_map, tTexture_coordinates).r * displacement_scale;
	tPosition = displace(tPosition, normalize(tNormal), displacement_offset);   
         
  };

  mat3 model_matrix = transpose(inverse(mat3(model_transformation_matrix)));
  tNormal = normalize(model_matrix * tNormal);
  tTangent = normalize(model_matrix * tTangent);
  tBitangent = normalize(model_matrix * tBitangent);

  gl_Position = projection_matrix * view_matrix * model_transformation_matrix * vec4(tPosition, 1);

};
