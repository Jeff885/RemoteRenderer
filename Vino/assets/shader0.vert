//edge

#pragma STDGL invariant(all)

precision highp float;

attribute vec3 aPosition;

varying highp float edge;

void main()
{
	gl_Position = vec4(aPosition.xy, 0.0, 1.0);
	edge = aPosition.z;
	gl_PointSize = 1.0;
}