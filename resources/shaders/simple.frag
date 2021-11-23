#version 150

//in  vec3 pass_Normal;
out vec4 out_Color;

uniform vec3 planet_Color;
//uniform vec3 light_position;
//uniform vec3 light_color;
//uniform float light_intensity;

void main() {
  //out_Color = vec4(abs(normalize(pass_Normal)), 1.0);
  out_Color = vec4(planet_Color, 1.0);
}
