#version 120

uniform float renderTargetSize;
uniform vec4 lightColor;

void main()
{
	// Distance entre le pixel et le centre
	float dist = length((gl_FragCoord.st / renderTargetSize) - vec2(0.5, 0.5));
	dist = 1 - 4.0 * (dist * dist);
	
	// Crée la lumière si on n'est pas dans une ombre
	float light = gl_TexCoord[0].s * dist;
	gl_FragColor = vec4(light, light, light, 1);
	gl_FragColor *= lightColor;
}
