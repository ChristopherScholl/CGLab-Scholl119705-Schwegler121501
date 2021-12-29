#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_TexCoord;

out vec2 pass_TexCoord;

void main(){
    gl_Position = vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
    
    // pass texture coordinates
    pass_TexCoord = in_TexCoord;
}