//show

#pragma STDGL invariant(all)
precision highp float;

attribute vec2 aPosition;
attribute vec2 texCoord;

varying vec2 tCoord;

void main()
{
	gl_Position = vec4(aPosition, 0.0, 1.0);
	tCoord = texCoord;
}