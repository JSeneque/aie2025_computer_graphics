// classic Phong fragment
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

uniform vec3 cameraPosition;

uniform vec3 AmbientColour;
uniform vec3 LightColour;
uniform vec3 LightDirection;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float specularPower;

uniform sampler2D diffuseTex;

out vec4 FragColour;

float CalculateLambertTerm(vec3 N, vec3 L) {
	return max(0, min( 1, dot( N, -L ) ) );
}

vec3 CalculateSpecular(float SpecularTerm) {
	return LightColour * Ks * SpecularTerm;
}

vec3 CalculateAmbientColour (vec3 T) {
	return AmbientColour * Ka * T;
}

vec3 CalculateDiffuse(float lambertTerm, vec3 T) {
	return LightColour * Kd * lambertTerm * T;
}

float CalculateSpecularTerm(vec3 N, vec3 L) {
	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect( L, N );

	return pow( max( 0, dot( R, V ) ), specularPower);
}

void main() {

	float lambertTerm = CalculateLambertTerm(normalize(vNormal), normalize(LightDirection));
	float specularTerm = CalculateSpecularTerm(normalize(vNormal), normalize(LightDirection));

	// what is the texture colour
	vec3 textureColour = texture(diffuseTex, vTexCoords).rgb;

	// output lambert as grayscale
	FragColour = vec4 ( CalculateAmbientColour(textureColour) + CalculateDiffuse(lambertTerm, textureColour) + CalculateSpecular(specularTerm), 1);
}