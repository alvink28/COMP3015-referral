#version 460

layout (location = 0) in vec3 VertexPosition;	
layout (location = 1) in vec3 VertexNormal;	
layout (location = 2) in vec2 VertexTexCoord;

out vec2 TexCoord;

out vec3 Position;
out vec3 Normal;

// Transformation matrices 
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
 
void main(){
	TexCoord = VertexTexCoord;

	//transfrom normal from model coordinates to view coordinates
	Normal = normalize( NormalMatrix * VertexNormal);

	//transform vertex position from model coordinates to view coordinates
	Position = (ModelViewMatrix * vec4(VertexPosition, 1.0)).xyz;

	gl_Position = MVP * vec4(VertexPosition, 1.0);
}

