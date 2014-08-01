// Thanks to http://www.alpha-arts.net/blog/articles/view/30/shader-de-blur

#version 120

uniform sampler2D texture;

void main()
{
	/*/ Distance of this pixel from the center
	float dist = distance(gl_TexCoord[0].xy, vec2(0.5, 0.5)) + 0.01;

	vec2 off = vec2(0.035, 0.0) * dist;*/

	vec2 off = vec2(0.004, 0.0);

	gl_FragColor = gl_Color * (
		texture2D(texture, gl_TexCoord[0].xy + off * 1.0)  * 0.06 + 
		texture2D(texture, gl_TexCoord[0].xy + off * 0.75) * 0.09 +
		texture2D(texture, gl_TexCoord[0].xy + off * 0.5)  * 0.12 +
		texture2D(texture, gl_TexCoord[0].xy + off * 0.25) * 0.15 +
		texture2D(texture, gl_TexCoord[0].xy) * 0.16 +
		texture2D(texture, gl_TexCoord[0].xy - off * 1.0)  * 0.06 +
		texture2D(texture, gl_TexCoord[0].xy - off * 0.75) * 0.09 +
		texture2D(texture, gl_TexCoord[0].xy - off * 0.5)  * 0.12 +
		texture2D(texture, gl_TexCoord[0].xy - off * 0.25) * 0.15);
}