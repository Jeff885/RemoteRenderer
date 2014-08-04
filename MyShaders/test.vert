#version 130


uniform sampler2D  color;
uniform vec2 size;

varying vec4  testColor;

void main()
{
  vec2 pos = vec2(gl_Vertex.x, gl_Vertex.y);
  testColor = texture2D(color, pos / size);
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}