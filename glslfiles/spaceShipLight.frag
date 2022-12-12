#version 400

in vec2 ex_TexCoord;  // exiting texture coord
in vec3 ex_Normal;    // exiting normal transformed by the normal matrix
in vec3 FragmentPos; 
uniform sampler2D DiffuseMap;


//Variables for directional light

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 



struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  


struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};


uniform DirLight dirLight;
uniform SpotLight spotLight;
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, ex_TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, ex_TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, ex_TexCoord));
    return (ambient + diffuse + specular);
}  


vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, ex_TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, ex_TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, ex_TexCoord));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}


void main(void)
{
	
}