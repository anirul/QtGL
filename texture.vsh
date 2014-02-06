attribute vec4 position; 
attribute vec4 source_color; 
 
varying vec4 destination_color; 
 
uniform mat4 projection;
uniform mat4 model_view;
 
attribute vec2 tex_coord_input; // New
varying vec2 tex_coord_output; // New

void main(void) { 
    destination_color = source_color; 
    gl_Position = projection * model_view * position;
    tex_coord_output = tex_coord_input; // New
}
