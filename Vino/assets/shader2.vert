//warpTest

#pragma STDGL invariant(all)

precision highp float;

const float ZERO = 1e-5;

attribute vec2 aPosition;


varying vec3 color;

varying float flag;

uniform sampler2D depthTex;
uniform sampler2D colorTex;
uniform vec2 depthSize;
uniform mat4 wpMat;
uniform mat4 MVP;

float unpack_depth(const in vec3 rgb_depth)
{
    const vec3 bit_shift = vec3(1.0/(256.0*256.0), 1.0/256.0, 1.0);
    float depth = dot(rgb_depth, bit_shift);
    return depth;
}

void main()
{
	vec4 sample = texture2D(depthTex, aPosition / depthSize);
	
	color = texture2D(colorTex, aPosition / depthSize).rgb;
	
	
	float dp = unpack_depth(sample.rgb);
	flag = 1.0;
	if(dp < ZERO) 
	{
		dp = 1.0;
		flag = 0.0;
	}
	
	vec4 pos = vec4(aPosition, dp, 1.0);
	
	pos = wpMat * pos;
	
	pos = vec4(pos.x / pos.w, pos.y / pos.w, pos.z / pos.w, 1.0);
	
	gl_Position = MVP * pos;
	
	gl_PointSize = 2.0;
}