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

// constants
vec3 ambient_intensity = vec3(0.1, 0.1, 0.1);

void main() {// create direction vectors (pointing form the vertex to the light / camera)
  vec3 light_direction_vector = (pass_ViewMatrix * vec4(light_position, 1.0)).xyz - pass_Vertex_Position;
  vec3 camera_direction_vector = (pass_ViewMatrix * vec4(pass_Camera_Position, 1.0)).xyz - pass_Vertex_Position;

  // normalize vectors
  vec3 normal_vector = normalize(pass_Normal);
  light_direction_vector = normalize(light_direction_vector);
  camera_direction_vector = normalize(camera_direction_vector);

  // light - camera vector
  vec3 h = normalize(light_direction_vector + camera_direction_vector);

  // calculate color
  //out_color = vec4(abs(normalize(pass_Normal)), 1.0);
  //out_color = vec4(planet_color, 1.0);
  vec3 ambient_color = ambient_intensity * planet_color;
  vec3 diffuse_color = max(dot(normal_vector, light_direction_vector), 0) * planet_color;
  out_color = vec4(ambient_color + diffuse_color, 1.0);
}
// vec3 diffuse_color = max(dot(normal_vector, light_direction_vector), 0) * planet_color;
