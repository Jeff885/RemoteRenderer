const float epsilon = 0.0000005;
uniform sampler2D preTex;
uniform vec2 preSize;

ivec2 offset[4] = {
 ivec2(-1, -1), ivec2(-1, 1), 
 ivec2(1, -1), ivec2(1, 1)
};

void main()
{
   ivec2 size = ivec2(int(preSize.x), int(preSize.y));
   ivec2 pos = ivec2(int(gl_FragCoord.x), int(gl_FragCoord.y));	
   ivec2 prePos = 2 * pos;
   if(prePos.x >= 0 && prePos.x < size.x && prePos.y >= 0 && prePos.y < size.y)
   {
	  vec2 tmpPos = vec2(float(prePos.x) + 0.5, float(prePos.y) + 0.5);
	  vec4 sample = texture2D(preTex, tmpPos / preSize);
	  if(sample.w > epsilon)
	  {
		gl_FragColor = sample;
		return;
	  }
   }
   int i;
   float sum = 0.0;
   int cnt = 0;
   for(i = 0; i < 4; i++)
   {
     ivec2 tmp = prePos + offset[i];
	 if(tmp.x >= 0 && tmp.x < size.x && tmp.y >= 0 && tmp.y < size.y)
	 {
		vec2 tmpPos = vec2(float(tmp.x) + 0.5, float(tmp.y) + 0.5);
	    vec4 sample = texture2D(preTex, tmpPos / preSize);
		if(sample.w > epsilon)
		{
			sum += sample.x;
			++cnt;
		}
	 }
   }
   if(cnt > 0)
   {
		gl_FragColor = vec4(vec3(sum / float(cnt)), 1.0);
		return;
   }
   gl_FragColor = vec4(1.0, 1.0, 1.0, 0.0);
}