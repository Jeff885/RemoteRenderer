const float epsilon = 0.0000005;
const int MaxKernelSize = 8;
vec2 offset[MaxKernelSize] = {
	vec2(-1.0, -1.0), vec2(-1.0, 0.0), vec2(-1.0, 1.0),
	vec2(0.0, -1.0), vec2(0.0, 1.0),
	vec2(1.0, -1.0), vec2(1.0, 0.0), vec2(1.0, 1.0)
};
uniform vec2 bufferSize;
uniform sampler2D tex;
uniform int showType;
uniform mat4 test;

void main()
{
   vec4 sample = texture2D(tex, gl_FragCoord.xy / bufferSize);
   ivec2 boundary = ivec2(int(bufferSize.x), int(bufferSize.y));
   gl_FragDepth = gl_FragCoord.z;
   if(showType == 0)
   {
	//if(sample.w < 0.5) gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	//else gl_FragColor = vec4(vec3(sample.w),1.0);
	gl_FragColor = vec4(vec3(sample.w),1.0);
	//if(abs(test[1][0] - 100.0) < epsilon) gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
	//else gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
   }
   else if(showType == 1)
	  gl_FragColor = vec4(sample.rgb, 1.0);
   else if(showType == 2)
   {
	  float sum;
	  ivec2 ps = ivec2(int(gl_FragCoord.x), int(gl_FragCoord.y));
	  if(ps.x == 0 || ps.x + 1 == boundary.x || ps.y == 0 || ps.y + 1 == boundary.y) sum = 0.0;
	  else
	  {
		  sum = 8.0* sample.w;
		  int i;
		  for(i = 0 ; i < MaxKernelSize; ++i)
		  {
			  vec2 pos = gl_FragCoord.xy + offset[i];
			  if(pos.x >0.0 && pos.x < bufferSize.x && pos.y > 0.0 && pos.y < bufferSize.y)
			  {
					vec4 tmp = texture2D(tex, pos / bufferSize);
					sum -= tmp.w;
			  }
		  }
		  sum = abs(sum);
	   }
	  //gl_FragColor = vec4(vec3(sum), 1.0);
	  if(sum > 0.00024 )gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	  else gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
   }
   else if(showType == 3)
   {
		if(1 - sample.w < epsilon ) discard;
		vec3 norm = sample.xyz;
		vec2 posx = gl_FragCoord.xy + vec2(1.0, 0.0);
		vec2 posy = gl_FragCoord.xy + vec2(0.0, 1.0);
		if(posx.x > bufferSize.x) 
			posx.x = gl_FragCoord.x - 1.0;
		if(posy.y > bufferSize.y) 
			posy.y = gl_FragCoord.y - 1.0;
		vec3 normx = texture2D(tex, posx / bufferSize).stp;
		vec3 normy = texture2D(tex, posy / bufferSize).stp;
		float delta = min( abs(dot(norm, normx)), abs(dot(norm, normy)));
		delta = 1.0 - delta;
		gl_FragColor = vec4(vec3(delta), 1.0);
   }
   else if(showType == 4)
   {
	    float sum;
	    ivec2 ps = ivec2(int(gl_FragCoord.x), int(gl_FragCoord.y));
	    if(ps.x == 0 || ps.x + 1 == boundary.x || ps.y == 0 || ps.y + 1 == boundary.y) sum = 0.0;
	    else
	    {
			 sum = 8.0* sample.w;
			 int i;
			 for(i = 0 ; i < MaxKernelSize; ++i)
			 {
				  vec2 pos = gl_FragCoord.xy + offset[i];
				  if(pos.x >0.0 && pos.x < bufferSize.x && pos.y > 0.0 && pos.y < bufferSize.y)
				  {
						vec4 tmp = texture2D(tex, pos / bufferSize);
						sum -= tmp.w;
				  }
			  }
			 sum = abs(sum);
	    }
		float delta = 0.0;
		if(1 - sample.w > epsilon )
		{
			vec3 norm = sample.xyz;
			vec2 posx = gl_FragCoord.xy + vec2(1.0, 0.0);
			vec2 posy = gl_FragCoord.xy + vec2(0.0, 1.0);
			if(posx.x > bufferSize.x) 
				posx.x = gl_FragCoord.x - 1.0;
			if(posy.y > bufferSize.y) 
				posy.y = gl_FragCoord.y - 1.0;
			vec3 normx = texture2D(tex, posx / bufferSize).stp;
			vec3 normy = texture2D(tex, posy / bufferSize).stp;
			delta = min( abs(dot(norm, normx)), abs(dot(norm, normy)));
			delta = 1.0 - delta;
		}
	    if(sum > 0.00024 )gl_FragColor = vec4(1.0, delta, 0.0, 1.0);
	    else gl_FragColor = vec4(0.0, delta, 0.0, 1.0);
   }
}