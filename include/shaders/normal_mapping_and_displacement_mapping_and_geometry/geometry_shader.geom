#version 440 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 vNormal[];
in vec3 vColor[];
in vec3 vPosition[];
in vec2 vTexture_coordinates[];
in vec3 vTangent[];
in vec3 vBitangent[];

out vec3 gNormal;
out vec3 gColor;
out vec3 gPosition;
out vec2 gTexture_coordinates;
out vec3 gTangent;
out vec3 gBitangent;

void main() {

    for (int i = 0; i < 3; ++i) {
        gNormal = vNormal[i];
        gColor = vColor[i];
        gPosition = vPosition[i];
        gTexture_coordinates = vTexture_coordinates[i];
        gTangent = vTangent[i];
        gBitangent = vBitangent[i];

        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }

    EndPrimitive();

};
