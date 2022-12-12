#version 400

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ViewMatrix;
//For the  spotlight
uniform mat4 ModelMatrix; //Going to be used for fragment position



in  vec3 in_Position;  // Position coming in
in  vec2 in_TexCoord;  // texture coordinate coming in
in  vec3 in_Normal;    // vertex normal used for lighting

uniform vec4 LightPos;  // light position
uniform vec3 SpotLightPos;  // light position
uniform vec3 SpotLightDir;  //light direction





out vec2 ex_TexCoord;  // exiting texture coord
out vec3 ex_Normal;    // exiting normal transformed by the normal matrix
out vec3 ex_PositionEye; 
out vec3 ex_LightDir; 
out vec3 FragPos;
out vec3 ex_SpotLightDir; //light direction vector
out vec3 ex_SpotLightPos; //light position vector
out vec3 spotLight_normal;




void main(void)
{

	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(in_Position, 1.0);
	ex_TexCoord = in_TexCoord;
	ex_Normal = NormalMatrix*in_Normal;
	ex_PositionEye = vec3((ModelViewMatrix * vec4(in_Position, 1.0)));
	ex_LightDir = vec3(ViewMatrix * LightPos);

	ex_SpotLightPos = vec3(ViewMatrix*vec4(SpotLightPos,1.0));
	ex_SpotLightDir = vec3(ViewMatrix*vec4(SpotLightDir,1.0));

	
	//FragPos = vec3(ModelMatrix * vec4(in_Position, 1.0));
	//gl_Position = ProjectionMatrix * ViewMatrix * vec4(FragPos, 1.0);

	//spotLight_normal = mat3(transpose(inverse(ModelMatrix))) * in_Normal;  

	//spotLightPosition = vec3(ViewMatrix * vec4(SpaceShipPos,1.0));
	//spotLightDirection = vec3(ViewMatrix * vec4(ObjectRotation,1.0));

	
	//FragPos = vec3(ModelMatrix * vec4(in_Position, 1.0));

	//gl_Position = ProjectionMatrix * ViewMatrix * vec4(FragPos, 1.0);

	//ex_Normal = NormalMatrix*in_Normal;
	//FragPos = vec3(ModelMatrix * vec4(in_Position, 1.0));
	//gl_Position = ProjectionMatrix * ViewMatrix * vec4(FragPos, 1.0);
	//spotLightDirection = vec3(ViewMatrix * vec4(SpaceShipPos,1.0));
	
	//spotLightDirection = vec3(ViewMatrix * SpaceShipPos);
	//ex_Normal = mat3(transpose(inverse(ModelMatrix))) * in_Normal;  
	//Fragment_Pos = vec3(ModelMatrix * vec4(in_Position, 1.0));
}