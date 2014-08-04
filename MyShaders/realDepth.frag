const float epsilon = 0.000005;

uniform sampler2D pDepth;
uniform vec2 size;
uniform float near;
uniform float far;

void main()
{
	gl_FragDepth = gl_FragCoord.z;
	vec4 sample = texture2D(pDepth, gl_FragCoord.xy / size);
	if(sample.w > epsilon)
	{
		float rdepth = far * near / (far - (far - near)* sample.x);
		gl_FragColor = vec4(vec3(rdepth), sample.w);
	}
	else gl_FragColor = sample;
}