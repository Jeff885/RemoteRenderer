//pull

#pragma STDGL invariant(all)

precision highp float;
precision highp int;

const float ZERO = 1e-6;

uniform sampler2D curTex;
uniform	sampler2D preTex;
uniform vec2 curSize;

uniform ivec2 offset1[4];
uniform ivec2 offset2[4];

vec2 preSize;

ivec2 icSize;
ivec2 ipSize;

vec3 pack_depth(const in float depth)
{
    const vec3 bit_shift = vec3(256.0*256.0, 256.0, 1.0);
    const vec3 bit_mask  = vec3(0.0, 1.0/256.0, 1.0/256.0);
    vec3 res = fract(depth * bit_shift);
    res -= res.xxy * bit_mask;
    return res;
}

float unpack_depth(const in vec3 rgb_depth)
{
    const vec3 bit_shift = vec3(1.0/(256.0*256.0), 1.0/256.0, 1.0);
    float depth = dot(rgb_depth, bit_shift);
    return depth;
}

bool isEven(int val)
{
	int t = val / 2;
	if(val == t * 2) 
	{
		return true; 
	}
	else return false;
}

float getDoddSample(ivec2 pos)
{
	vec4 sample;
	float sum = 0.0;
	int cnt = 0;
	int i;
	for(i = 0; i < 4; ++i)
	{
		ivec2 tmp = pos + offset1[i];
		if(tmp.x >=0 && tmp.x < icSize.x && tmp.y >= 0 && tmp.y < icSize.y)
		{
			vec2 cpos = vec2(float(tmp.x) + 0.5, float(tmp.y) + 0.5);
			sample = texture2D(curTex,  cpos / curSize);
			if(sample.a > ZERO)
			{
				sum += unpack_depth(sample.rgb);
				++cnt;
			}
			else
			{
				vec2 preTmp = vec2(float(tmp.x / 2) + 0.5, float(tmp.y / 2) + 0.5);
				sample = texture2D(preTex, preTmp / preSize);
				sum += unpack_depth(sample.rgb);
				++cnt;
			}
		}
	}
	if(cnt > 0) return (sum / float(cnt));
	else return 0.0;
}

void main()
{
	vec2 fragCoord = gl_FragCoord.xy;
	vec4 sample;
	icSize = ivec2(int(curSize.x), int(curSize.y));
	ipSize = ivec2((icSize.x + 1)/ 2, (icSize.y + 1) / 2);
	preSize = vec2(float(ipSize.x), float(ipSize.y));
	ivec2 pos = ivec2(int(fragCoord.x),int(fragCoord.y));
	sample = texture2D(curTex, (vec2(pos) + vec2(0.5, 0.5))/ curSize);
	if(sample.a > ZERO)
	{
		gl_FragColor = sample;
	}
	else if(isEven(pos.x) && isEven(pos.y))
	{
		vec2 preTmp = vec2(float(pos.x / 2) + 0.5, float(pos.y / 2) + 0.5);
		sample = texture2D(preTex, preTmp / preSize);
		gl_FragColor = sample;
	}
	else if((!isEven(pos.x)) && (!isEven(pos.y)))
	{
		float tmp = getDoddSample(pos);
		gl_FragColor = vec4(pack_depth(tmp), 1.0);
	}
	else
	{
		int i;
		float sum = 0.0;
		int cnt = 0;
		for(i = 0; i < 4; ++i)
		{
			ivec2 tmp = pos + offset2[i];
			if(tmp.x >=0 && tmp.x < icSize.x && tmp.y >= 0 && tmp.y < icSize.y)
			{
				vec2 cpos = vec2(float(tmp.x) + 0.5, float(tmp.y) + 0.5);
				sample = texture2D(curTex,  cpos / curSize);
				if(sample.a > ZERO)
				{
					sum += unpack_depth(sample.rgb);
					++cnt;
				}
				else if(isEven(tmp.x) && isEven(tmp.y))
				{
					vec2 preTmp = vec2(float(tmp.x / 2) + 0.5, float(tmp.y / 2) + 0.5);
					sample = texture2D(preTex, preTmp / preSize);
					sum += unpack_depth(sample.rgb);
					++cnt;
				}
			}
		}
		if(cnt > 0)
		{
			sum /= float(cnt);
			gl_FragColor = vec4(pack_depth(sum), 1.0);
		}
		else gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
	}
	
}

