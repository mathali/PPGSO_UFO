#version 330
layout(location = 0) in vec3 Position;
layout(location = 4) in vec3 Color;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

out vec3 vertexColor;

void main() {
  vertexColor = Color;

  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);
}
