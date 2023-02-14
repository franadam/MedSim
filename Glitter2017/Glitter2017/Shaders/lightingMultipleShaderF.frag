#version 330 core

out vec4 FragColor;
  
in vec3 FragPos; 
in vec3 Normal;
in vec2 TexCoords;

//Material
uniform sampler2D texture_specular;
uniform sampler2D texture_diffuse; 

// Direct Light
struct DirLight {
    vec3 direction;
  
	vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

// Point Light
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

	vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

// Spot Light
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
	vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_POINT_LIGHTS 8  
#define MATERIAL_SHININESS 32  

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

//Camera
uniform vec3 viewPos;  

// Skybox
uniform samplerCube skybox;

// Function
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir); 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{	
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	// phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    

    // phase 3: spot light
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    vec4 color = vec4(result, 1.0);
	    	    	
	// Reflection
	vec3 R = 2 * dot(viewDir,norm) * norm - viewDir;

	// Refraction
	float ratio = 0.62;
	vec3 T = (ratio * norm * viewDir - sqrt(1 - (ratio)^2 * (1 - (norm * viewDir)^2))) * norm - ratio * viewDir;
		
	vec4 reflection = vec4(texture(skybox, R).rgb, 1.0);
	vec4 refraction = vec4(texture(skybox, T).rgb, 1.0);

	vec4 ref = mix(reflection,refraction, 0.3);
	
	FragColor = vec4(result, 0.1);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
   
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);	
	vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), MATERIAL_SHININESS); // Blinn-Phong
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), MATERIAL_SHININESS); // Phong
   
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular, TexCoords));
   
    return ambient + (diffuse + specular) * light.color;
}  


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), MATERIAL_SHININESS);

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    

    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular, TexCoords));
  	
    ambient  *= attenuation;
	diffuse  *= attenuation;
    specular *= attenuation;

	vec3 color = ambient + (diffuse + specular)  * light.color;

	color = pow(color, vec3(1.0/2.2)); //gamma correction 

    return color ;
} 

/*
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), MATERIAL_SHININESS);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
	// spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
   
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular, TexCoords));
   
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + (diffuse + specular) * light.color;
}
*/