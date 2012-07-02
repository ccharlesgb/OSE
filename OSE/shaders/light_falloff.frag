uniform sampler2D texture;

void main()
{
	vec2 pos = gl_TexCoord[0].xy;
	pos.x = pos.x - 0.5;
	pos.y = pos.y - 0.5;
	float dist = (pos.x * pos.x) + (pos.y * pos.y);
	dist = dist * 5.0;
	float brightness = 1.0/(dist+1.0) - (0.5 * (dist * dist));
	vec4 pixel = gl_Color * texture2D(texture, gl_TexCoord[0].xy);
	pixel.rgb = pixel.rgb * brightness;

	gl_FragColor = pixel;
}
