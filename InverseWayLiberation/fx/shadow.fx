// Idea from http://www.catalinzima.com/2010/07/my-technique-for-the-shader-based-dynamic-2d-shadows/

#version 120

uniform sampler2D reduce;
uniform float renderTargetSize;
uniform vec4 lightColor;

float GetShadowDistanceH(vec2 TexCoord)
{
	float u = TexCoord.x;
	float v = TexCoord.y;

	u = abs(u - 0.5) * 2.0;
	v = v * 2.0 - 1.0;
	float v0 = v / u;
	v0 = (v0 + 1.0) / 2.0;

	// Horizontal info was stored in the Red component
	return texture2D(reduce, vec2((TexCoord.x < 0.5) ? 0.0 : 1.0, v0)).r;
}

float GetShadowDistanceV(vec2 TexCoord)
{
	float u = TexCoord.y;
	float v = TexCoord.x;

	u = abs(u - 0.5) * 2.0;
	v = v * 2.0 - 1.0;
	float v0 = v / u;
	v0 = (v0 + 1.0) / 2.0;
	
	// Vertical info was stored in the Green component
	return texture2D(reduce, vec2((TexCoord.y < 0.5) ? 0.0 : 1.0, v0)).g;
}

void main()
{
	// Distance of this pixel from the center
	float dist = distance(gl_TexCoord[0].xy, vec2(0.5, 0.5));
	dist -= 2.0 / renderTargetSize;

	// Distance stored in the shadow map
	float shadowMapDistance;
	// Coords in [-1,1]
	float nY = 2.0 * (gl_TexCoord[0].y - 0.5);
	float nX = 2.0 * (gl_TexCoord[0].x - 0.5);

	// We use these to determine which quadrant we are in
	if (abs(nY) < abs(nX))
	{
		shadowMapDistance = GetShadowDistanceH(gl_TexCoord[0].xy);
	}
	else
	{
		shadowMapDistance = GetShadowDistanceV(gl_TexCoord[0].xy);
	}

	// If distance to this pixel is lower than distance from shadowMap,
	// then we are not in shadow
	float light = dist < shadowMapDistance ? 1.0 : 0.0;
	vec4 result = vec4(light, light, light, 1);
	result *= lightColor;

	float x = dist * 2.0;
	result.a = 1.0 / (2 * (x + 0.37)) - 0.37;
	//result.a = 1.0 - x;
	
	gl_FragColor = result;
}
