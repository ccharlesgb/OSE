uniform sampler2D texture;

void main()
{
	vec2 pos = gl_TexCoord[0].xy;
	pos.x = pos.x - 0.5f;
	pos.y = pos.y - 0.5f;
	float dist = (pos.x * pos.x) + (pos.y * pos.y);
	dist = dist * 5.f;
	float brightness = 1.f/(dist+1.f) - (0.5f * (dist * dist));
	vec4 pixel = gl_Color * texture2D(texture, gl_TexCoord[0].xy);
	pixel.rgb = pixel.rgb * brightness;

	gl_FragColor = pixel;
}
