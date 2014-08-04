uniform sampler2D tex;
uniform sampler2D atex;
uniform vec2 size;

void main()
{
	gl_FragDepth = gl_FragCoord.z;
	gl_FragColor = texture2D(tex, gl_FragCoord.xy / size);
}