#version 130

const float epsilon = 0.000001;

uniform bool flag;
varying vec4 testColor;

varying float DepthBool;

void main()
{
	if(1.0 - DepthBool < epsilon) discard;
	gl_FragDepth = gl_FragCoord.z;
	//if(flag) gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	//else gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
	gl_FragColor = testColor;
}