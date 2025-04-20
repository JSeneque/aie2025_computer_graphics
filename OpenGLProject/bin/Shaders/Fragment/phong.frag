// classic Phong fragment
#version 410

struct PointLight {
	vec3 position;
	vec3 colour;
	float intensity;
};

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;
in vec3 vTangent;
in vec3 vBiTangent;

uniform vec3 CameraPosition;

uniform vec3 AmbientColour;
uniform vec3 LightColour;
uniform vec3 LightDirection;

const int MAX_LIGHTS = 4;
uniform int numLights;
uniform vec3 PointLightColour[MAX_LIGHTS];
uniform vec3 PointLightPosition[MAX_LIGHTS];

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float specularPower;

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;

out vec4 FragColour;

float CalculateSpecularTerm(vec3 N, vec3 L) {
	vec3 V = normalize(CameraPosition - vPosition.xyz);
	vec3 R = reflect( L, N );

	// calculate how much of the reflective light hitting the eye
	return pow( max( 0, dot( R, V ) ), specularPower);
}

float CalculateLambertTerm(vec3 N, vec3 L) {
	return max(0, min( 1, dot( N, -L ) ) );
}

vec3 CalculateSpecular(float SpecularTerm, vec3 T) {
	return LightColour * Ks * T * SpecularTerm;
}

vec3 CalculateAmbientColour (vec3 T) {
	return AmbientColour * Ka * T;
}

vec3 CalculateDiffuse(float lambertTerm, vec3 T) {
	return LightColour * Kd * lambertTerm * T;
}

void main() 
{
	// normalise the normal
	vec3 N = normalize(vNormal);
	// normalise the light direction
	vec3 L = normalize(LightDirection);
	// normalise tangent
	vec3 T = normalize(vTangent);
	// normalise bi-tangent
	vec3 B = normalize(vBiTangent);
	
	// create the tangent space matrix
	mat3 TBN = mat3(T, B, N);
	
	// what is the texture colour
	vec3 textureDiffuse = texture(diffuseTex, vTexCoords).rgb;
	vec3 textureSpecular = texture(specularTex, vTexCoords).rgb;
	vec3 textureNormal = texture(normalTex, vTexCoords).rgb;

	N = TBN * (textureNormal * 2.0 - 1.0);
	
	float lambertTerm = CalculateLambertTerm(N, L);
	float specularTerm = CalculateSpecularTerm(N, L);

	// === Directional Light ===
	vec3 directionalLighting = CalculateAmbientColour(textureDiffuse) + CalculateDiffuse(lambertTerm, textureDiffuse) + CalculateSpecular(specularTerm, textureSpecular);
	
	// === Point Light ===
	vec3 pointLightContribution = vec3( 0.0) ;
	
	for (int i = 0; i < numLights; ++i)
	{
		// create a vector between the point position and the vertex
		vec3 lightDir = PointLightPosition[i] - vPosition.xyz;
		// normalise the vector
		vec3 L = normalize(lightDir);
		
		float lambert = CalculateLambertTerm(N, L);
		float specular = CalculateSpecularTerm(N, L);
		
		vec3 diffuse = PointLightColour[i] * Kd * lambert * textureDiffuse;
		vec3 specularCol = PointLightColour[i] * Ks * specular * textureSpecular;
		
		pointLightContribution += diffuse + specular;
	}

	
	// === Final Output ===
	vec3 finalColour = directionalLighting + pointLightContribution;
	FragColour = vec4 (finalColour, 1.0);

}