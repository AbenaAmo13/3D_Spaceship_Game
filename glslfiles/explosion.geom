#version 400
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

   in VS_OUT {
    vec2 ex_TexCoord;
    vec3 normal;
} gs_in[];
  


out vec2 exiting_TexCoord;  


uniform float time;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0);
}


void main() {  
    gl_Position = explode(gl_in[0].gl_Position, gs_in[0].normal);
    exiting_TexCoord = gs_in[0].ex_TexCoord;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position,  gs_in[1].normal);
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, gs_in[2].normal);
    exiting_TexCoord = gs_in[2].ex_TexCoord;
    EmitVertex();
    EndPrimitive();
}