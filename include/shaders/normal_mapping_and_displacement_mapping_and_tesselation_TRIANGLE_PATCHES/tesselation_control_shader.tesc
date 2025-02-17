#version 440 core

layout (vertices = 3) out;

in vec3 vNormal[];
in vec3 vColor[];
in vec3 vPosition[];
in vec2 vTexture_coordinates[];
in vec3 vTangent[];
in vec3 vBitangent[];

uniform float tesselation_multiplier;

out vec3 cNormal[];
out vec3 cColor[];
out vec3 cPosition[];
out vec2 cTexture_coordinates[];
out vec3 cTangent[];
out vec3 cBitangent[];

void main() {

  gl_TessLevelOuter[0] = tesselation_multiplier;
  gl_TessLevelOuter[1] = tesselation_multiplier;
  gl_TessLevelOuter[2] = tesselation_multiplier;

  gl_TessLevelInner[0] = tesselation_multiplier;

  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
  
  cPosition[gl_InvocationID] = vPosition[gl_InvocationID];
  cNormal[gl_InvocationID] = vNormal[gl_InvocationID];
  cTangent[gl_InvocationID] = vTangent[gl_InvocationID];
  cBitangent[gl_InvocationID] = vBitangent[gl_InvocationID];
  cTexture_coordinates[gl_InvocationID] = vTexture_coordinates[gl_InvocationID];
  cColor[gl_InvocationID] = vColor[gl_InvocationID];

};