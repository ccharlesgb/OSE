uniform sampler2D texture;
uniform sampler2D blend_channel;
uniform sampler2D other_texture;
uniform float pos_x;
uniform float pos_y;
uniform float scale;

void main()
{
	vec2 offset;
	offset.x = pos_x;
	offset.y = pos_y;
	vec2 ScaledTexCoord = (gl_TexCoord[0].xy / scale) + offset;
    float factor = texture2D(blend_channel, ScaledTexCoord).x;
	gl_FragColor = (texture2D(texture, gl_TexCoord[0].xy) * factor) + (texture2D(other_texture, gl_TexCoord[0].xy) * (1.0-factor));
}
