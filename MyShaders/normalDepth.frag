
varying vec3 normal;

void main()
{
   gl_FragDepth = gl_FragCoord.z;
   gl_FragColor = vec4(normal, gl_FragCoord.z);
}