//smooth

precision highp float;
precision highp int;

const float epsilon = 0.00005555;
const float MAX = 65535.0;

uniform sampler2D tex;
uniform vec2 size;
uniform ivec2 offset[25];
uniform int kernel[25];

void main()
{
	ivec2 iSize = ivec2(int(size.x), int(size.y));
	ivec2 ipos = ivec2(int(gl_FragCoord.x), int(gl_FragCoord.y));	
	lowp int i;
	float sum = 0.0;
	float norm = 0.0;
	vec2 sample = texture2D(tex, gl_FragCoord.xy / size).rg;
	float ref = sample.r * 255.0 * 256.0 + sample.g * 255.0;
	for(i = 0; i < 25; ++i)
	{
		ivec2 tmp = ipos + offset[i];
		if(tmp.x >= 0 && tmp.x < iSize.x && tmp.y >= 0 && tmp.y < iSize.y)
		{
			vec2 tPos = vec2(float(tmp.x) + 0.5, float(tmp.y) + 0.5);
			vec2 sample = texture2D(tex, tPos / size).rg;
			float ths = sample.r * 255.0 * 256.0 + sample.g * 255.0;
			float coef = 1.0 / (epsilon + abs(ref - ths) / 65535.0);
			coef *= float(kernel[i]);
			sum += coef * ths;
			norm += coef;
		}
	}
	sum = sum / norm;
	int ttmp;
	if(sum > MAX) ttmp = int(MAX);
	else ttmp = int(sum);
	int r = ttmp / 256;
	int g = ttmp - r * 256;
	gl_FragColor = vec4(float(r) / 255.0, float(g) / 255.0, 1.0, 0.0);
}