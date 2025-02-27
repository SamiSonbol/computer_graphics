#version 440 core

layout (points) in;
layout (points, max_vertices = 1) out;

in vec3 vColor[];
in vec3 vPosition[];

out vec3 gColor;
out vec3 gPosition;

void main() {

    for (int i = 0; i < 1; ++i) {

        gColor = vColor[i];
        gPosition = vPosition[i];

        gl_Position = gl_in[i].gl_Position;
        EmitVertex();

    };

    EndPrimitive();

};
