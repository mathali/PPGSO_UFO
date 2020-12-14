#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// Direction of light
uniform vec3 LightDirection;

uniform vec3 ViewPosition;

uniform vec3 Ambient;

// (optional) Transparency
uniform float Transparency;

// (optional) Texture offset
uniform vec2 TextureOffset;

uniform vec3 Ks;
uniform vec3 Kd;
uniform vec3 Ka;

uniform vec3 SpecularStrength;

uniform bool BeamFlag;
uniform vec3 BeamPos;
uniform vec3 BeamColor;
uniform vec3 BeamDirection;
uniform float BeamCutoff;
uniform vec3 diffuse_strength;
uniform int object;

// The vertex shader will feed this input
in vec2 texCoord;

// Wordspace normal passed from vertex shader
in vec4 normal;

in vec3 FragPosition;

in vec3 FragPosition2;

in vec3 norm;


// The final color
out vec4 FragmentColor;

void main() {
  vec3 LDir = normalize(LightDirection - FragPosition);
  vec3 VDir = normalize(ViewPosition - FragPosition);
  vec3 RDir = normalize(reflect(-LDir, norm));
  vec3 Spec = pow(max(dot(VDir, RDir), 0.0), 2) * Ks;
  vec3 Specular = SpecularStrength * Spec ;

  vec3 diffuse = max(dot(vec4(norm, 0.0f), vec4(LightDirection, 1.0f)), 0.0f) * Kd * diffuse_strength;

  vec3 amb =  Ambient * Ka;

  FragmentColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset) * vec4((diffuse + amb + Specular), 1.0f);
  FragmentColor.a = 1.0f;

  if(BeamFlag){
    vec3 LDir = normalize(BeamPos - FragPosition2);
    float beamSpecStrength = 0.5f;
    float diffuse_strength = 5000.0f;
    vec3 VDir = normalize(ViewPosition - FragPosition2);
    vec3 RDir = normalize(reflect(-LDir, norm));
    vec3 Spec = pow(max(dot(VDir, RDir), 0.0), 2) * Ks;

    float dist = distance(BeamPos, FragPosition2);
    float attenuation = (1.0/(10.0f + 100.0f * dist +  5000.0f * dist * dist));

    vec3 Specular = Spec * attenuation * beamSpecStrength;
    vec3 diffuse = max(dot(vec4(norm, 1.0f), vec4(normalize(LDir), 1.0f)), 0.0f) * Kd * attenuation * diffuse_strength;

    FragmentColor += vec4((diffuse + Specular) * BeamColor, 1.0f);
  }
}
