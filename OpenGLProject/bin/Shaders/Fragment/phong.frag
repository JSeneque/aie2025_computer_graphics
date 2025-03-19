// classic Phong fragment
#version 410

in vec3 vNormal;

uniform vec3 AmbientColour;
uniform vec3 LightColour;
uniform vec3 LightDirection;

out vec4 FragColour;

void main() {
	// normalise both the normal and light direction
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);

	// lambert term (negate light direction)
	float lambertTerm = max(0, min( 1, dot( N, -L ) ) );

	// calculate diffuse
	vec3 diffuse = LightColour * lambertTerm;
	vec3 ambient = AmbientColour;

	// output lambert as grayscale
	FragColour = vec4 ( ambient + diffuse, 1);
}