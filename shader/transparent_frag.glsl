#version 330

uniform vec3 vertexColor;

out vec4 FragmentColor;

uniform vec3 OverallColor;

void main() {
  FragmentColor = vec4(vertexColor + OverallColor, 1.0f);
  FragmentColor.a = 0.3;
}
