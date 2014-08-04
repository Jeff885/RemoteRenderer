const float epsilon = 0.00000005;
uniform sampler2D dsEdge;
uniform sampler2D texEdge;
uniform vec2 bufferSize;
uniform int sumx[6];
uniform int sumy[6];

uniform ivec2 offset[2][4];


struct Data
{
	int level;
	int x,y;
	int cnt;
	int type;
	float sum;
	int idx;
};

vec4 getSample(int level, int x, int y);
Data levelAndType(Data dt);
bool check(int level,int x, int y);

int stCnt = 0;
Data stack[15];
float locSum = 0.0;
bool locTag = false;
int cnt = 0;


void main()
{
	gl_FragDepth = gl_FragCoord.z;
	int i;
	for(i = 0; i < 50000; ++i)
	{
		//vec4 sample = texture2D(texEdge, gl_FragCoord.xy / bufferSize);
	}
	gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
	/*ivec2 pos = ivec2(int(gl_FragCoord.x), int(gl_FragCoord.y));
	Data top;
	top.level = 0;
	top.x = pos.x;
	top.y = pos.y;
	top.cnt = 0;
	top.sum = 0.0;
	top.type = -1;
	top.idx = -1;
	stack[++stCnt] = top;
	while(stCnt > 0)
	{
		cnt++;
		//if(cnt > 300) break;
		top = stack[stCnt--];
		if(top.idx == -1)
		{
			top = levelAndType(top);
			vec4 sample = getSample(top.level, top.x, top.y);
			if(sample.a < epsilon)
			{
				if(top.type != -1 && top.level < 5)
				{
					++top.idx;
					stack[++stCnt] = top;
					locSum = 0.0;
					locTag = false;
				}
			}
			else
			{
				locSum = sample.r;
				locTag = true;
			}
		}
		else
		{
			if(locTag)
			{
				top.sum += locSum;
				++top.cnt;
				locSum = 0.0;
				locTag = false;
			}
			if(top.idx == 4)
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
				next.level = top.level;
				next.x = top.x + offset[top.type][top.idx].x;
				next.y = top.y + offset[top.type][top.idx].y;
				next.cnt = 0;
				next.sum = 0.0;
				next.type = -1;
				next.idx = -1;
				++top.idx;
				stack[++stCnt] = top;
				if(check(next.level, next.x, next.y)) stack[++stCnt] = next;
			}
		}
	}
	gl_FragColor = vec4(vec3(locSum), 1.0);*/
}


bool check(int level, int x, int y)
{
	if(level == 0)
	{
		if(x < 0 || x >=int(bufferSize.x)) return false;
		if(y < 0 || y >=int(bufferSize.y)) return false;
		return true;
	}
	else
	{
		if(x < 0 || x >= sumx[level] - sumx[level - 1]) return false;
		if(y < 0 || y >= sumy[level] - sumy[level - 1]) return false;
		return true;
	}
}
vec4 getSample(int level, int x, int y)
{
	vec2 pos;
	if(level == 0)
	{
		pos = vec2(float(x) + 0.5, float(y) + 0.5);
		pos = pos / bufferSize;
		return texture2D(texEdge, pos);
	}
	else
	{
		pos = vec2(float(x + sumx[level-1]) + 0.5, float(y + sumy[level-1]) + 0.5);
		pos = pos / bufferSize;
		return texture2D(dsEdge, pos);
	}
}

Data levelAndType(Data dt)
{
	while(dt.x % 2 == 0 && dt.y % 2 == 0 && dt.level < 5)
	{
		++dt.level;
		dt.x /= 2;
		dt.y /= 2;
	}
	if(dt.x % 2 != 0 && dt.y % 2 != 0) dt.type = 0;
	else if(dt.x % 2 != 0 || dt.y % 2 != 0) dt.type = 1;
	return dt;
}