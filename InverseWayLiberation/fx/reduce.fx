// Idea from http://www.catalinzima.com/2010/07/my-technique-for-the-shader-based-dynamic-2d-shadows/

#version 120

uniform sampler2D texture;
uniform float renderTargetWidth;

vec2 Reduce(vec2 TexCoord)
{
	float y = TexCoord.y;
	vec2 max = vec2(1, 1);

	if (TexCoord.x == 0.5)
	{
		for (float i = renderTargetWidth / 2.0; i > 0.0; --i)
		{
			vec2 pos = vec2(i / renderTargetWidth, y);
			vec2 color = texture2D(texture, pos).rg;
			max = min(color, max);

			if (max.x != 1.0 && max.y != 1.0)
				return max;
		}
		return max;
	}
	else
	{
		for (float i = renderTargetWidth / 2.0; i < renderTargetWidth; ++i)
		{
			vec2 pos = vec2(i / renderTargetWidth, y);
			vec2 color = texture2D(texture, pos).rg;
			max = min(color, max);

			if (max.x != 1.0 && max.y != 1.0)
				return max;
		}
		return max;
	}
}

void main()
{
	gl_FragColor = vec4(Reduce(vec2(gl_FragCoord.x, gl_TexCoord[0].y)), 0, 1);
}
