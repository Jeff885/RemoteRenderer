#version 130



varying vec4 testColor;

void main()
{
	gl_FragDepth = gl_FragCoord.z;
	//gl_FragColor = texture2D(color, pos / size);
	gl_FragColor = testColor;
}