#version 330
layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

out vec2 texCoord;

out vec4 normal;
out vec3 norm;

out vec3 FragPosition;
out vec3 FragPosition2;

void main() {
  texCoord = TexCoord;

  norm = mat3(transpose(inverse(ModelMatrix))) * Normal;
  normal = normalize(ModelMatrix * vec4(Normal, 0.0f));

  gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);

  FragPosition = vec3(ModelMatrix * vec4(Position, 0.0f));

  FragPosition2 = normalize(vec3(ModelMatrix[3]) * Position);
}
