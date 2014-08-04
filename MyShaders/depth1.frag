void main()
{
   gl_FragDepth = gl_FragCoord.z;
   //gl_FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
   if(gl_FragCoord.z < 0.95) gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
   else gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}