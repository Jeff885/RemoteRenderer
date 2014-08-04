#version 130

const float epsilon = 0.000001;

uniform sampler2D  depth;
uniform sampler2D  color;
uniform sampler2D  edge;

uniform vec2 size;
uniform mat4 iMVPW;
uniform mat4 uMVP;

varying vec4 testColor;
varying float DepthBool;

void main()
{
  vec2 pos = vec2(gl_Vertex.x, gl_Vertex.y);
  
  float dp = texture(depth, pos / size).x;
  
  DepthBool = dp;
  
  //testColor = texture2D(color, pos / size);
  
  testColor = vec4(0.9725, 0.5607, 0.1019, 1.0);
  
  float smp = texture2D(edge, pos / size).w;
  
  if(smp > epsilon) testColor = vec4(0.0, 0.0, 0.0, 1.0);
  
  vec4 src = vec4(gl_Vertex.x, gl_Vertex.y, dp, 1.0);
  
  src = iMVPW * src;
  
  src = vec4(src.x / src.w, src.y / src.w, src.z / src.w, 1.0);
  
  gl_Position = gl_ModelViewProjectionMatrix * src;
}