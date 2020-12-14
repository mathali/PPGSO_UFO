#version 330
uniform sampler2D Texture;

in vec2 texCoord;

out vec4 FragmentColor;

void main() {
  FragmentColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y));
  FragmentColor.a = 1.0f;
}
