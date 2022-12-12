#version 400

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ViewMatrix;

in  vec3 in_Position;  // Position coming in
in  vec2 in_TexCoord;  // texture coordinate coming in
in  vec3 in_Normal;    // vertex normal used for lighting

out vec2 ex_TexCoord;  // exiting texture coord
out vec3 ex_Normal;    // exiting normal transformed by the normal matrix
out vec3 FragmentPos; 



void main(void)
{
    FragmentPos = vec3(ModelViewMatrix * vec4(in_Position, 1.0));

	ex_Normal = NormalMatrix*in_Normal; 
	
	ex_TexCoord = in_TexCoord;
	
	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(FragmentPos, 1.0);
		
	


}