uniform sampler2D texture;
uniform sampler2D blend_channel;
uniform sampler2D other_texture;
uniform float pos_x;
uniform float pos_y;

void main()
{
	vec2 offset;
	offset.x = pos_x;
	offset.y = pos_y;
	offset = offset * 512;
    float factor = texture2D(blend_channel, (gl_TexCoord[0].xy / 2) + offset).x;

	gl_FragColor = (texture2D(texture, gl_TexCoord[0].xy) * factor) + (texture2D(other_texture, gl_TexCoord[0].xy) * (1.f-factor));
}
