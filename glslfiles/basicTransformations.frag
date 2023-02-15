#version 400



in  vec2 ex_TexCoord; //texture coord arriving from the vertex
in  vec3 ex_Normal;  //normal arriving from the vertex

out vec4 out_Color;   //colour for the pixel



in vec3 ex_PositionEye;
in vec3 ex_SpotLightPos; 
in vec3 ex_SpotLightDir;
in vec3 spotLight_normal;
in vec3 FragPos;
in vec3 ex_LightDir;


uniform vec4 light_ambient;
uniform vec4 light_diffuse;
uniform vec4 light_specular;

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;
float spot_cutoff = 0.5;
float spot_exponent = 10;





uniform sampler2D DiffuseMap;



void main(void)
{

	vec4 color = light_ambient * material_ambient;
	//out_Color = texture(DiffuseMap, ex_TexCoord); //show texture values

	//out_Color = vec4(ex_Normal,1.0); //Show normals

	//out_Color = vec4(ex_TexCoord,0.0,1.0); //show texture coords

	//Calculate lighting
	vec3 n, L, L_lightDir;
	float NdotL, NdotL_LightDir;
	
	n = normalize(ex_Normal);
	L_lightDir = normalize(ex_LightDir);
	//spotLightN = normalize(spotLight_normal);
	L = normalize(ex_SpotLightPos - ex_PositionEye);
	
	vec3 v = normalize(-ex_PositionEye);
	vec3 r = normalize(-reflect(L, n));//For specular lighting
	vec3 r_second = normalize(-reflect(L_lightDir, n));
	
	float RdotV = max(0.0, dot(r, v));
	float R_secondDotV = max(0.0, dot(r_second, v));


	NdotL = max(dot(n, L),0.0);

	NdotL_LightDir = max(dot(n, L_lightDir),0.0);
	
	//color = vec4(ex_Normal, 1.0);
		if(NdotL > 0.0)
	{
		float LdotSpotLightDir = dot(L, normalize(ex_SpotLightDir));
		if(LdotSpotLightDir > spot_cutoff)
		{
			vec4 col = (light_diffuse * material_diffuse * NdotL);
			//col += light_specular * material_specular * pow(RdotV, material_shininess);

			col *= pow(LdotSpotLightDir, spot_exponent);
			color += col;
		}


		if(NdotL_LightDir > 0.0) 
	{
		color += (light_ambient * material_diffuse * NdotL_LightDir);
	}

		color += material_specular * light_specular * pow(R_secondDotV, material_shininess);





	}else{
		
				if(NdotL_LightDir > 0.0) 
	{
		color += (light_ambient * material_diffuse * NdotL_LightDir);
	}

		color += material_specular * light_specular * pow(R_secondDotV, material_shininess);
	
	
	
	
	}

	//out_Color = vec4(ex_Normal, 1.0);
			//out_Color = color;  //show just lighting

		//out_Color = texture(DiffuseMap, ex_TexCoord); //show texture only
		out_Color = color * texture(DiffuseMap, ex_TexCoord); //show texture and lighting
}