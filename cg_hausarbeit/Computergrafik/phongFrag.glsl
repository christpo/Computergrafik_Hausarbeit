#version 110

varying vec3 Position;
varying vec3 Normal;
varying vec2 Texcoord;

uniform vec3 DiffColor;
uniform vec3 SpecColor;
uniform vec3 AmbientColor;
uniform float SpecExp;

uniform sampler2D DiffuseTexture;
 
void main(void) {

  vec3 N = normalize(Normal);
  vec3 L = normalize(gl_LightSource[0].position.xyz - Position);
  vec3 E = normalize(-Position);
  vec3 R = normalize(reflect(-L,N));

  vec3 DiffuseComponent = gl_LightSource[0].diffuse.xyz * DiffColor * max( dot(N,L), 0.0);
  vec3 SpecularComponent = gl_LightSource[0].specular.xyz * SpecColor * pow( max( dot(E,R), 0.0), SpecExp);

  vec4 DiffuseTexColor = texture2D(DiffuseTexture, Texcoord);
  DiffuseComponent *= DiffuseTexColor.xyz;

  vec3 AmbientComponent = gl_LightSource[0].ambient.xyz * AmbientColor * DiffuseTexColor.xyz;
 
  gl_FragColor = vec4(DiffuseComponent + SpecularComponent + AmbientComponent, 0);
}