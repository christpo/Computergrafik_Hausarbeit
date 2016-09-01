#version 110

#define QUANT 3.0

varying vec3 Position;
varying vec3 Normal;
varying vec2 Texcoord;

uniform vec3 DiffColor;
uniform vec3 SpecColor;
uniform vec3 AmbientColor;
uniform float SpecExp;

uniform sampler2D DiffuseTexture;

float QuantizeValue( float f){

	f = floor( f * QUANT);
	f /= QUANT;
	return f;
}

float CalcOutLine( vec3 N , vec3 E){

	float Outline = abs( dot(N, E));

	float a1 = 3.14 * 80.0; 
	float a2 = 180.0;
	float a3 = a1 / a2;
	float angle = cos( a3);

	if( Outline < angle){

		Outline = 0.0;
	}
	else{

		Outline = 1.0;
	}

	return Outline;
}

 
void main(void) {

  vec3 N = normalize(Normal);
  vec3 L = normalize(gl_LightSource[0].position.xyz - Position);
  vec3 E = normalize(-Position);
  vec3 R = normalize(reflect(-L,N));

  float DiffFactor = max( dot(N,L), 0.0);
  DiffFactor = QuantizeValue( DiffFactor);
  DiffFactor = max( DiffFactor, 0.3);
  vec3 DiffuseComponent = gl_LightSource[0].diffuse.xyz * DiffColor * DiffFactor;

  float SpecFactor = pow( max( dot(E,R), 0.0), SpecExp);
  SpecFactor = QuantizeValue(SpecFactor);
  vec3 SpecularComponent = gl_LightSource[0].specular.xyz * SpecColor * SpecFactor;

  vec4 DiffuseTexColor = texture2D(DiffuseTexture, Texcoord);
  DiffuseComponent *= DiffuseTexColor.xyz;

  vec3 AmbientComponent = gl_LightSource[0].ambient.xyz *  AmbientColor * DiffuseTexColor.xyz;
 
  float Outline = CalcOutLine(N,E);

  gl_FragColor = Outline * vec4(DiffuseComponent + SpecularComponent + AmbientComponent, 0);
}