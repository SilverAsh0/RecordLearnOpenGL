#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;
//TODO:这里有点问题，好像是法线偏移是在投影空间下进行的,参考官网那节课的教程评论
in VS_OUT {
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.05;

uniform mat4 perspective;

void GenerateLine(int index)
{
    gl_Position = perspective * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = perspective * (gl_in[index].gl_Position +vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0);// 第一个顶点法线
    GenerateLine(1);// 第二个顶点法线
    GenerateLine(2);// 第三个顶点法线
}