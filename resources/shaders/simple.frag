#version 150

// inputs
in vec3 pass_Normal;
in vec3 pass_Vertex_Position;
in vec3 pass_Camera_Position;
in mat4 pass_ViewMatrix;

// outout: color of position
out vec4 out_color;

// uploaded uniforms
uniform vec3 planet_color;
uniform vec3 light_position;
uniform vec3 light_color;
uniform float light_intensity;
uniform vec3 ambient_intensity;

void main() {
  vec3 normal_vector = normalize(pass_Normal);
  
  // create direction vectors (pointing form the vertex to the light / camera)
  vec3 light_direction_vector = normalize((pass_ViewMatrix * vec4(light_position, 1.0) - vec4(pass_Vertex_Position, 1.0)).xyz);
  vec3 camera_direction_vector = normalize((pass_ViewMatrix * vec4(pass_Camera_Position, 1.0) - vec4(pass_Vertex_Position, 1.0)).xyz);

  // light - camera vector
  vec3 h = normalize(light_direction_vector + camera_direction_vector);

  // calculate color
  //out_color = vec4(abs(normalize(pass_Normal)), 1.0);
  //out_color = vec4(planet_color, 1.0);
  vec3 ambient_color = ambient_intensity * planet_color * light_color;
  vec3 diffuse_color = max(dot(normal_vector, light_direction_vector), 0) * planet_color * light_intensity * light_color;
  vec3 specular_color = pow(max(dot(h, normal_vector), 0), 16.0) * light_color;
  out_color = vec4(ambient_color + diffuse_color + specular_color, 1.0);
}
