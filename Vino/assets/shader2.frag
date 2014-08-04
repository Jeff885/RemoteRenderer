//warpTest

#pragma STDGL invariant(all)

precision highp float;

varying vec3 color;
varying float flag;

void main()
{
	if(flag > 0.005) gl_FragColor = vec4(color, 0.0);
	else gl_FragColor = vec4(0.153, 0.659, 0.843, 1.0);
}