const float epsilon = 0.0000005;
const int MaxKernelSize = 8;
vec2 offset[MaxKernelSize] = {
	vec2(-1.0, -1.0), vec2(-1.0, 0.0), vec2(-1.0, 1.0),
	vec2(0.0, -1.0), vec2(0.0, 1.0),
	vec2(1.0, -1.0), vec2(1.0, 0.0), vec2(1.0, 1.0)
};
uniform vec2 texSize;
uniform sampler2D texDepth;
uniform bool isShow;

void main()
{
	gl_FragDepth = gl_FragCoord.z;
	vec4 sample = texture2D(texDepth, gl_FragCoord.xy / texSize);
	float sum;
	ivec2 ps = ivec2(int(gl_FragCoord.x), int(gl_FragCoord.y));
	sum = 8.0 * sample.x;
	int i;
	for(i = 0; i < MaxKernelSize; ++i)
	{
		vec2 pos = gl_FragCoord.xy + offset[i];
		if(pos.x > 0.0 && pos.x < texSize.x && pos.y > 0.0 && pos.y < texSize.y)
		{
			vec4 tmp = texture2D(texDepth, pos / texSize);
			sum -= tmp.x;
		}
	}
	sum = abs(sum);
	
	if(sum > 0.006 || (ps.x % 32 == 0 && ps.y % 32 == 0)) 
	{
	     if(!isShow)gl_FragColor = vec4(vec3(sample.x),1.0);
		 else gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else 
	{
	     if(!isShow) gl_FragColor = vec4(1.0, 1.0, 1.0 ,0.0);
		 else gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}