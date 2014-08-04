const float epsilon = 0.00000005;
uniform sampler2D texEdge;
uniform vec2 bufferSize;
uniform int sumx[6];
uniform int sumy[6];
uniform int plus[6];

uniform ivec2 offset[4];


struct Data
{
	int level;
	int x,y;
	int cnt;
	float sum;
	int idx;
};

Data getData(ivec2 pos);
vec4 getSample(int level, int x, int y);
bool check(int level, int x, int y);

Data stack[10];
int stCnt = 0;
float locSum = 0.0;
bool locTag = false;
void main()
{
	gl_FragDepth = gl_FragCoord.z;
	ivec2 pos = ivec2(int(gl_FragCoord.x), int(gl_FragCoord.y));
	Data top = getData(pos);
	if(top.level == -1) gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
	else
	{
		stack[++stCnt] = top;
		while(stCnt > 0)
		{	
			top = stack[stCnt--];
			if(top.idx == -1)
			{
				vec4 sample = getSample(top.level, top.x, top.y);
				if(sample.a < epsilon)
				{	
					if(top.level > 0)
					{
						++top.idx;
						stack[++stCnt] = top;
						locSum = 0.0;
						locTag = false;
					}
				}
				else 
				{
				    //gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
					locSum = sample.r;
					locTag = true;
				}
			}
			else 
			{
				if(locTag)
				{
					top.sum += locSum;
					++top.cnt ;
					locSum = 0.0;
					locTag = false;
				}
				if(top.idx == 4 )
				{
				  if(top.cnt > 0)
				  {
					locSum = top.sum / float(top.cnt);
					locTag = true;
				  }
				}
				else
				{
					Data next;
					next.sum = 0.0;
					next.cnt = 0;
					next.idx = -1;
					next.level = top.level - 1;
					next.x = 2 * top.x + offset[top.idx].x;
					next.y = 2 * top.y + offset[top.idx].y;
					++top.idx;
					stack[++stCnt] = top;
					if(check(next.level, next.x , next.y)) stack[++stCnt] = next;
				}
			}
		}
		
		float wt = 0.0;
		if(locTag) wt = 1.0;
		gl_FragColor = vec4(vec3(locSum), wt);
	}
}


Data getData(ivec2 pos)
{
	Data tmp;
	tmp.level = -1;
	tmp.sum = 0.0;
	tmp.cnt = 0;
	tmp.idx = -1;
	int lx = -1, ly = -2;
	int i;
	for(i = 0; i < 6; ++i) 
		if(pos.x < sumx[i])
		{
			lx = i;
			break;
		}
	for(i = 0; i < 6; ++i)
		if(pos.y < sumy[i])
		{
			ly = i;
			break;
		}
	if(lx == ly)
	{
		tmp.level = lx;
		tmp.x = pos.x - sumx[lx - 1];
		tmp.y = pos.y - sumy[ly - 1];
	}
	return tmp;
}

vec4 getSample(int level,int x,int y)
{
	vec2 pos = vec2(float(x * plus[level]) + 0.5, float(y * plus[level]) + 0.5);
	return texture2D(texEdge, pos / bufferSize);
}

bool check(int level, int x, int y)
{
	if(x < 0 || x >= sumx[level] - sumx[level-1]) return false;
	if(y < 0 || y >= sumy[level] - sumy[level-1]) return false;
	return true;
}
