#version 150

// inputs
in vec2 pass_TexCoord;

// outout: color of position
out vec4 out_color;

// uploaded uniforms
uniform sampler2D fullScreenQuad_texture;

void main(){
    out_color = texture(fullScreenQuad_texture, pass_TexCoord);
}