// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// interpolated colour received from vertex stage

uniform sampler2D mTexture;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;



in vec2 texCoord;
in vec3 FragPos;
in vec3 Normal;

void main(void)
{
	float ambient = 0.25f;
	vec4 color = texture(mTexture, texCoord);

	float specularStrength = 0.9f;

	//diffuse and ambient calculations
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * vec3(color);

	float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0)
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0); //vec4(result, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	FragColor = color;// vec4(result, 1.0);
}
