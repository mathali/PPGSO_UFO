#version 330
layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

out vec2 texCoord;

void main() {
  texCoord = TexCoord;

  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);
}
