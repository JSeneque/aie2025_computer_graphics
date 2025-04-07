// classic Phong fragment
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;
in vec3 vTangent;
in vec3 vBiTangent;

uniform vec3 CameraPosition;

uniform vec3 AmbientColour;
uniform vec3 LightColour;
uniform vec3 LightDirection;

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

	N = TBN * (textureNormal * 2 - 1);
	
	float lambertTerm = CalculateLambertTerm(N, L);
	float specularTerm = CalculateSpecularTerm(N, L);


	// doing the * 2 - 1 put the value into a proper range -1 to 1
	
	// output lambert as grayscale
	FragColour = vec4 ( CalculateAmbientColour(textureDiffuse) + CalculateDiffuse(lambertTerm, textureDiffuse) + CalculateSpecular(specularTerm, textureSpecular), 1);
	//FragColour = vec4(N, 1);
	//FragColour = vec4 (lambertTerm, lambertTerm, lambertTerm, 1);
}