varying vec3 normal;

void main()
{
	gl_Position = ftransform();
	normal =  normalize(gl_NormalMatrix* gl_Normal);
}