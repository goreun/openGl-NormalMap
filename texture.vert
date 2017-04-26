#version 130

// Input vertex data, different for all executions of this shader.
attribute vec3 VertexPosition;
attribute vec3 VertexNormal;
attribute vec2 VertexTexCoord;
attribute vec3 VertexTangent;
attribute vec3 VertexBitangent;


varying  vec2 UV;
varying  vec3 Position_worldspace;
varying  vec3 EyeDirection_cameraspace;
varying  vec3 LightDirection_cameraspace;

varying  vec3 LightDirection_tangentspace;
varying  vec3 EyeDirection_tangentspace;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform mat3 MV3x3;
uniform vec3 LightPosition;

void main(){


    gl_Position =  MVP * vec4(VertexPosition,1);

    UV = vec2(4,3) * VertexTexCoord;


	 Position_worldspace = (M * vec4(VertexPosition,1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( V * M * vec4(VertexPosition,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( V * vec4(LightPosition,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
	
	
	// model to camera = ModelView
	vec3 vertexTangent_cameraspace = MV3x3 *  VertexTangent;
	vec3 vertexBitangent_cameraspace = MV3x3 * VertexBitangent ;
	vec3 vertexNormal_cameraspace = MV3x3 * VertexNormal;
	
	mat3 TBN = transpose(mat3(
		vertexTangent_cameraspace,
		vertexBitangent_cameraspace,
		vertexNormal_cameraspace	
	)); // You can use dot products instead of building this matrix and transposing it. See References for details.

	LightDirection_tangentspace = TBN * LightDirection_cameraspace;
	EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;
	 
}	
	
