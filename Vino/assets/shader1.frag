//push

#pragma STDGL invariant(all)

precision highp float;
precision highp int;

const float ZERO = 1e-4;

uniform sampler2D preTex;
uniform vec2 preSize;
uniform ivec2 offset[4];

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

void main()
{
   vec2 fragCoord = gl_FragCoord.xy;
   ivec2 size = ivec2(int(preSize.x), int(preSize.y));
   ivec2 pos = ivec2(int(fragCoord.x), int(fragCoord.y));	
   ivec2 prePos = 2 * pos;
   if(prePos.x >= 0 && prePos.x < size.x && prePos.y >= 0 && prePos.y < size.y)
   {
	  vec2 tmpPos = vec2(float(prePos.x) + 0.5, float(prePos.y) + 0.5);
	  vec4 sample = texture2D(preTex, tmpPos / preSize);
	  if(sample.a > ZERO)
	  {
		gl_FragColor = sample;
	  }
	  else
	  {
			int i;
			float sum = 0.0;
			int cnt = 0;
			for(i = 0; i < 4; ++i)
			{
				 ivec2 tmp = prePos + offset[i];
				 if(tmp.x >= 0 && tmp.x < size.x && tmp.y >= 0 && tmp.y < size.y)
				 {
					vec2 tmpPos = vec2(float(tmp.x) + 0.5, float(tmp.y) + 0.5);
					vec4 sample = texture2D(preTex, tmpPos / preSize);
					if(sample.a > ZERO)
					{
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
}