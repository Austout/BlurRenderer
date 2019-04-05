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

uniform vec3 lightPos[20];
uniform vec3 lightColor[20];
uniform vec3 viewPos;
uniform float intensity[20];


in vec2 texCoord;
in vec3 FragPos;
in vec3 Normal;

void main(void)
{
	float ambient = 0.5f;
	vec4 color = texture(mTexture, texCoord);
	vec3 finalSpecDiff = vec3(0, 0, 0);
	int i = 0;
	while (intensity[i] != -1.0f) {
		float specularStrength = 0.9f;
		float distance = length(lightPos[i] - FragPos);
		//diffuse and ambient calculations
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lightPos[i] - FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColor[i];

		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		vec3 specular = specularStrength * spec * lightColor[i];
		finalSpecDiff += (diffuse + specular) *(intensity[i] / distance);
		i++;
	}
	

	vec3 result = (ambient + finalSpecDiff) * vec3(color);

	float brightness = dot(result, vec3(1, 1, 1));
	if (color.x >= 1.0f && color.y >= 1.0f && color.z >= 1.0f)
		BrightColor = color;
	else if (brightness > 1.0)
		BrightColor = vec4(result, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

	FragColor = vec4(result, 1.0);
}
