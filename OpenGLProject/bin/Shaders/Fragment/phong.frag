// classic Phong fragment
#version 410

in vec4 vPosition;
in vec3 vNormal;

uniform vec3 cameraPosition;

uniform vec3 AmbientColour;
uniform vec3 LightColour;
uniform vec3 LightDirection;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float specularPower;

out vec4 FragColour;

void main() {
	// normalise both the normal and light direction
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);


	// lambert term (negate light direction)
	float lambertTerm = max(0, min( 1, dot( N, -L ) ) );

	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect( L, N );

	// calculate specular term
	float specularTerm = pow( max( 0, dot( R, V ) ), specularPower);
	
	// calculate each colour property
	vec3 ambient = AmbientColour * Ka;
	vec3 diffuse = LightColour * Kd * lambertTerm;
	vec3 specular = LightColour * Ks * specularTerm;
	

	// output lambert as grayscale
	FragColour = vec4 ( ambient + diffuse + specular, 1);
}