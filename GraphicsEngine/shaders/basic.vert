#version 400 core

layout (location = 0) in  vec3    position;
layout (location = 1) in  vec3    normal;
layout (location = 2) in  float   matIndex;
layout (location = 3) in  vec2    uv;
layout (location = 4) in  vec4    joint;
layout (location = 5) in  vec4    weight;
layout (location = 6) in  int     node;

out vec3    fposition;
out vec3    fnormal;
out vec2    fuv;
out float   fmatIndex;

uniform mat4    model_matrix = mat4(1.0);
uniform mat4    view_matrix = mat4(1.0);
uniform mat4    proj_matrix = mat4(1.0);

void main() {
    fposition = position;
    fnormal = normal;
    fuv = uv;
    fmatIndex = matIndex;

    int nodes = node;
    vec4 joints = joint;
    vec4 weights = weight;

    mat4    mvp = proj_matrix * view_matrix;

	gl_Position = proj_matrix * view_matrix *  model_matrix * vec4(position, 1.0);
}