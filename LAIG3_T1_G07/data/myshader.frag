uniform sampler2D texturemap;

void main(void)
{
	gl_FragColor = texture2D(texturemap,gl_TexCoord[0].st);
}
