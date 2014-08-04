const float epsilon = 0.000055;

const float dz2 = 90000.0;

const int R = 3;

const int MaxSizeKernel = 25;

uniform sampler2D texDepth;
uniform sampler2D texLow;
uniform vec2 texSize;

ivec2 iTexSize;

float near = 1.0;
float far = 10000.0;

ivec2 offset[MaxSizeKernel] =
{
	ivec2(-2, -2), ivec2(-2, -1), ivec2(-2, 0), ivec2(-2, 1), ivec2(-2, 2),
	ivec2(-1, 2),ivec2(-1, -1), ivec2(-1, 0), ivec2(-1, 1),ivec2(-1, 2),
	ivec2(0, 2),ivec2(0, -1), ivec2(0, 0), ivec2(0, 1), ivec2(0, 2),
	ivec2(1, 2),ivec2(1, -1), ivec2(1, 0), ivec2(1, 1),ivec2(1, 2),
	ivec2(2, -2), ivec2(2, -1), ivec2(2, 0), ivec2(2, 1), ivec2(2, 2)
};

int kernel[MaxSizeKernel] =
{
	1, 4, 7, 4, 1,
	4, 16, 26, 16, 4,
	7, 26, 41, 26, 7,
	4, 16, 26, 16, 4,
	1, 4, 7, 4, 1
};

float getDCoef(float x, float y, int r);
void main()
{
	gl_FragDepth = gl_FragCoord.z;
	gl_FragColor = texture2D(texLow, gl_FragCoord.xy / texSize);
	/*iTexSize = ivec2(int(texSize.x), int(texSize.y));
	ivec2 iPos = ivec2(int(gl_FragCoord.x), int(gl_FragCoord.y));
	float d0 = texture2D(texDepth, gl_FragCoord.xy / texSize).x;
	
	d0 = far * near / (far - (far - near) * d0);
	
	int i;
	vec3 sum = vec3(0.0, 0.0, 0.0);
	float norm = 0.0;
	for(i = 0; i < MaxSizeKernel; ++i)
	{
		ivec2 tiPos = iPos + offset[i];
		if(tiPos.x >= 0 && tiPos.x < iTexSize.x && tiPos.y >= 0 && tiPos.y < iTexSize.y)
		{
			vec2 tPos = vec2(float(tiPos.x) + 0.5, float(tiPos.y) + 0.5);
			
			float td = texture2D(texDepth, tPos / texSize);
			td = far * near / (far - (far - near) * td);
			
			float coef = getDCoef((td - d0)*(td - d0), dz2, R);
			//float coef = 1.0 / (epsilon + abs(td - d0) / 15000.0);
			coef *= float(kernel[i]);
			norm += coef;
			sum += texture(texLow, tPos / texSize).rgb * coef;
		}
	}
	sum = sum / norm;
	gl_FragColor = vec4(sum, 1.0);*/
}

float getDCoef(float x, float y, int r)
{
	float t = 1 - (x / y);
	if(t < epsilon)  t= epsilon;
	float res = 1.0;
	int i;
	for(i = 0; i < r; ++i)
		res= res * t;
	return res;
}