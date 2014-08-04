//show

#pragma STDGL invariant(all)

precision highp float;
precision highp int;


uniform sampler2D showTex;

varying vec2 tCoord;

void main()
{
	gl_FragColor = texture2D(showTex, tCoord);
}