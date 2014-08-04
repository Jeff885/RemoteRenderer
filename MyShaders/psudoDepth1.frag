const float epsilon = 0.000005;
uniform sampler2D rDepth;
uniform vec2 size;
uniform float near;
uniform float far;

void main()
{
	gl_FragDepth = gl_FragCoord.z;
	vec4 sample = texture2D(rDepth, gl_FragCoord.xy / size);
	gl_FragColor = vec4(vec3(sample.x), 1.0);
}