uniform sampler2D texColor;
uniform vec2 texSize;

void main()
{
	gl_FragDepth = gl_FragCoord.z;
	vec2 pos = vec2(gl_FragCoord.x, gl_FragCoord.y);
	gl_FragColor = texture2D(texColor, pos / texSize);
}