#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 offset;

out vec2 v_texCoord;
out float v_distToCamera;
out vec3 v_diffuse;
out vec3 v_fragPos;
out vec3 v_normal;

uniform mat4 u_M;  	// model
uniform mat4 u_MVP;  	// model-view-projection matrix
uniform vec3 u_scale;

uniform vec3 u_camPos;
// uniform vec3 u_camDir;

void main() 
{
	vec4 pos = u_M * vec4((position*u_scale)+offset, 1.0);
    gl_Position = u_MVP * vec4((position*u_scale)+offset, 1.0);
    v_texCoord = texCoord;
    v_distToCamera = length(gl_Position.xyz);
	
	//u_camPos might not be the right thing not sure...
	//v_fragPos = vec3(u_M * vec4(u_camPos, 1.0));
	v_fragPos = vec3(u_M * vec4((position*u_scale)+offset, 1.0));
	//v_fragPos = pos.xyz;

	vec3 lightDir = normalize(u_camPos - pos.xyz);
	float diff = max(dot(normal, lightDir), 0.0);
	v_diffuse = diff * vec3(1.0, 1.0, 1.0);

	//Not sure this'll work...
	v_normal = normal;
	//v_normal = mat3(transpose(inverse(u_M))) * normal;  
}


    
#shader fragment
#version 330 core

layout(std140) uniform PointLightsBlock
{
    vec4 posAndDist[200];
	vec4 colorAndIntensity[200];
	int count;
} pointLightBlock;

layout(location = 0) out vec4 color;
in vec2 v_texCoord;
in float v_distToCamera;
in vec3 v_diffuse;
in vec3 v_fragPos;
in vec3 v_normal;

uniform vec4 u_color;
uniform sampler2D u_texture;

float customDistV3(vec3 v1, vec3 v2) {
	vec3 distVec = v1 - v2;
	float dist = sqrt(dot(distVec, distVec));
	return dist;
}

void main() {
    //vec4 texColor = texture(u_texture, v_texCoord);
    //color = u_color * texColor;
    //color.w = 1;

    	vec4 texColor = texture(u_texture, v_texCoord);
    	float distMod = v_distToCamera;
    	float maxDist = 64;
	if (distMod > maxDist)
        	distMod = maxDist;
    	distMod = maxDist / distMod;
    	distMod = distMod - 1;
    	if (distMod > 1)
        	distMod = 1;
	color = u_color * texColor * distMod;
	color.w = 1;
	
	//Camera centric light...
	//color.x *= (v_diffuse.x + 0.5);
	//color.y *= (v_diffuse.y + 0.5);
	//color.z *= (v_diffuse.z + 0.5);



	int count = pointLightBlock.count;
	vec3 colorToAdd = vec3(0, 0, 0);
	for(int i = 0; i < count; i++) {
		vec4 posAndDist = pointLightBlock.posAndDist[i];
		vec3 lightPos;
		lightPos.x = posAndDist[0];
		lightPos.y = posAndDist[1];
		lightPos.z = posAndDist[2];
		float dist = posAndDist[3];
		vec4 colorAndIntensity = pointLightBlock.colorAndIntensity[i];
		vec3 lightColor;
		lightColor.x = colorAndIntensity[0];
		lightColor.y = colorAndIntensity[1];
		lightColor.z = colorAndIntensity[2];
		float intensity = colorAndIntensity[3];

		//vec3 lightDir = normalize(lightPos - FragPos);
		//float diff = max(dot(normal, lightDir), 0.0);
		
		//float diff = dist / distance(lightPos - FragPos);
		//colorToAdd += diff * vec3(1.0, 1.0, 1.0);

		//vec3 norm = normalize(v_normal);
		//vec3 lightDir = normalize(lightPos - v_fragPos);
		//float diff = max(dot(norm, lightDir), 0.0);
		//vec3 diffuse = diff * lightColor;
		//diffuse *= dist / diff; //customDistV3(lightPos, v_fragPos);

		
		//vec3 diffuse = (dist / customDistV3(v_fragPos, lightPos)) * lightColor;
		//float difference = length(v_fragPos) - length(lightPos);
		//if (difference < 0)
		//	difference = -difference;
		//vec3 diffuse = (1 / difference) * lightColor;
		
		colorToAdd += diffuse;
	}
	//if (count != 0)
	//	colorToAdd /= count;

	//color += (colorToAdd, 0);
	color.x *= colorToAdd.x + 0.5;
	color.y *= colorToAdd.y + 0.5;
	color.z *= colorToAdd.z + 0.5;
}