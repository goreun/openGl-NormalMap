#version 130 


  in vec2 UV;
  in vec3 Position_worldspace;
  in vec3 EyeDirection_cameraspace;
  in vec3 LightDirection_cameraspace;  
  in vec3 LightDirection_tangentspace;
  in vec3 EyeDirection_tangentspace;


  out vec3 color;

 uniform sampler2D DiffuseTextureSampler;
 uniform sampler2D NormalTextureSampler;
 uniform sampler2D SpecularTextureSampler;
 uniform mat4 V;
 uniform mat4 M;
 uniform mat3 MV3x3;
 uniform vec3 LightPosition_worldspace;
 uniform vec3  LightColor = vec3(1,1,1);
 uniform float LightPower = 2.0f;


void main(){
	
	// Material properties
	vec3 MaterialDiffuseColor =  texture( DiffuseTextureSampler, UV ).rgb;
	vec3 MaterialAmbientColor =  vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = texture( SpecularTextureSampler, UV ).rgb  * 2;

	vec3 TextureNormal_tangentspace = normalize(texture( NormalTextureSampler, UV).rgb*2.0 - 1.0);
	
	// Distance to the light
	float distance = length( LightPosition_worldspace - Position_worldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = TextureNormal_tangentspace;
	//vec3 n = vec3(0.0,0.0,1.0);
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize(LightDirection_tangentspace);
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );

	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_tangentspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	color = MaterialAmbientColor 
	      + MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance)
	      + MaterialSpecularColor *LightColor * LightPower * pow(cosAlpha,40)/ (distance*distance);
	     // color=vec3(1.0,0.0,0.0);
}




