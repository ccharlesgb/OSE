uniform sampler2D texture;
uniform sampler2D blend_channel
uniform sampler2D other_texture

void main()
{
	float factor = texture2D(blend_channel, gl_TexCoord[0].xy).w;
	gl_FragColor = (texture2D(texture, gl_TexCoord[0].xy) * factor) + (texture2D(texture, gl_TexCoord[0].xy) * (1-factor);
}
