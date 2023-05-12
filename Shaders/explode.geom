#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
//默认是OpenGL默认提供的是gl_in
//而我们提供的是gs_in
//同时几何着色器的输入必须+[]
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;

} gs_in[];

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
uniform float time;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    return position + vec4(direction, 0.0);
}

vec3 GetNormal() {
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}


void main() {

    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    FragPos=gs_in[0].FragPos;
    TexCoords = gs_in[0].TexCoords;
    Normal = gs_in[0].Normal;
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, normal);
    FragPos=gs_in[1].FragPos;
    TexCoords = gs_in[1].TexCoords;
    Normal = gs_in[1].Normal;
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, normal);
    FragPos=gs_in[2].FragPos;
    TexCoords = gs_in[2].TexCoords;
    Normal = gs_in[2].Normal;
    EmitVertex();
    EndPrimitive();
}
