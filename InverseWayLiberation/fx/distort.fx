// Idea from http://www.catalinzima.com/2010/07/my-technique-for-the-shader-based-dynamic-2d-shadows/

#version 120

uniform sampler2D texture;

float BlackOrDistance(vec2 TexCoord)
{
	// Calcule la distance
	float dist = distance(TexCoord.xy, vec2(0.5, 0.5));

	// Transforme tout en noir
	if (texture2D(texture, TexCoord.xy).a == 1.0)
		return dist;
	else
		return 1.0;
}

vec4 Distort(vec2 TexCoord)
{
	// Translate u and v into [-1 , 1] domain
	float u0 = TexCoord.x * 2.0 - 1.0;
	float v0 = TexCoord.y * 2.0 - 1.0;
 
	// Then, as u0 approaches 0 (the center), v should also approach 0
	v0 = v0 * abs(u0);
	// Convert back from [-1,1] domain to [0,1] domain
	v0 = (v0 + 1.0) / 2.0;
	// We now have the coordinates for reading from the initial image
	vec2 newCoords = vec2(TexCoord.x, v0);
 
	// Read for both horizontal and vertical direction and store them in separate channels
	float horizontal = BlackOrDistance(newCoords.xy);
	float vertical = BlackOrDistance(newCoords.yx);

	// Change la couleur
	return vec4(horizontal, vertical, 0, 1);
}

void main()
{
	gl_FragColor = Distort(gl_TexCoord[0].xy);
}
