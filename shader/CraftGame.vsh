#version 330 core

// Our vertex consists just of a Vec2f (aka vec2) which represents position
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_color;
layout(location = 2) in vec2 i_uv;

uniform mat4 u_projection;
uniform mat4 u_view;

// forward color to fragment shader (will be linear interpolated)
out VertexData {
	vec2 uv;
	vec3 color;
} outData;

void main() {

  mat4 pipeline = u_projection * u_view;

  outData.uv = i_uv;
  outData.color = i_color;

  // write gl_Position -> make OpenGL happy
  gl_Position = pipeline * vec4(i_pos, 1);
}