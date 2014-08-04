const float epsilon = 0.000005;
uniform sampler2D rDepth;
uniform vec2 size;
uniform float near;
uniform float far;

void main()
{
	gl_FragDepth = gl_FragCoord.z;
	vec4 sample = texture2D(rDepth, gl_FragCoord.xy / size);
	float pdepth = (far * (sample.x - near)) / ((far - near) * sample.x);
	gl_FragColor = vec4(vec3(pdepth), 1.0);
	
	/*if(sample.w > epsilon)
	{
		float pdepth = (far * (sample.x - near)) / ((far - near) * sample.x);
		gl_FragColor = vec4(vec3(pdepth), 1.0);
	}
	else gl_FragColor = vec4(1.0, 1.0, 1.0, 0.0);*/
}