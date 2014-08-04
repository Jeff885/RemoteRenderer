const float epsilon = 0.000005;

uniform sampler2D curTex;

uniform sampler2D posTex;

uniform vec2 curSize;


ivec2 size;

vec2 posSize;

ivec2 offset1[4] = {
	ivec2(-1, -1), ivec2(-1, 1),
	ivec2(1, -1), ivec2(1, 1)
};
ivec2 offset2[4] ={
	ivec2(0, -1), ivec2(-1, 0),
	ivec2(0, 1), ivec2(1, 0)
};

float getDoddSample(ivec2 pos);

void main()
{
	vec4 sample;
	gl_FragDepth = gl_FragCoord.z;
	size = ivec2(int(curSize.x), int(curSize.y));
	ivec2 psize = ivec2((size.x + 1) / 2, (size.y + 1) / 2);
	posSize = ivec2(float(psize.x), float(psize.y));
	
	ivec2 pos = ivec2(int(gl_FragCoord.x),int(gl_FragCoord.y));
	sample = texture2D(curTex, gl_FragCoord.xy / curSize);
	if(sample.w > epsilon)
	{
		gl_FragColor = vec4(vec3(sample.x), 1.0);
		return;
	}
	if(pos.x | 1 ==0 && pos.y | 1 == 0)
	{
		vec2 posTmp = vec2(float(pos.x / 2) + 0.5, float(pos.y / 2) + 0.5);
		sample = texture2D(posTex, posTmp / posSize);
		gl_FragColor = vec4(vec3(sample.x), 1.0);
		return;
	}
	if(pos.x | 1 !=0 && pos.y | 1 !=0)
	{
		float avg = getDoddSample(pos);
		gl_FragColor = vec4(vec3(avg), 1.0);
		return;
	}
	int i;
	float sum = 0.0;
	int cnt = 0;
	for(i = 0; i < 4; ++i)
	{
		ivec2 tmp = pos + offset2[i];
		if(tmp.x >=0 && tmp.x < size.x && tmp.y >= 0 && tmp.y < size.y)
		{
			vec2 cpos = vec2(float(tmp.x) + 0.5, float(tmp.y) + 0.5);
			sample = texture2D(curTex,  cpos / curSize);
			if(sample.w > epsilon)
			{
				sum += sample.x;
				++cnt;
			}
			else if(tmp.x | 1 ==0 && tmp.y | 1 ==0)
			{
				vec2 posTmp = vec2(float(tmp.x / 2) + 0.5, float(tmp.y / 2) + 0.5);
				sample = texture2D(posTex, posTmp / posSize);
				sum += sample.x;
				++cnt;
			}
			else
			{
				sum += getDoddSample(tmp);
				++cnt;
			}
		}
	}
	if(cnt > 0) gl_FragColor = vec4(vec3(sum / float(cnt)), 1.0);
	else gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
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
		if(tmp.x >=0 && tmp.x < size.x && tmp.y >= 0 && tmp.y < size.y)
		{
			vec2 cpos = vec2(float(tmp.x) + 0.5, float(tmp.y) + 0.5);
			sample = texture2D(curTex,  cpos / curSize);
			if(sample.w > epsilon)
			{
				sum += sample.x;
				++cnt;
			}
			else
			{
				vec2 posTmp = vec2(float(tmp.x / 2) + 0.5, float(tmp.y / 2) + 0.5);
				sample = texture2D(posTex, posTmp / posSize);
				sum += sample.x;
				++cnt;
			}
		}
	}
	if(cnt > 0) return (sum / float(cnt));
	return 0.0;
}

