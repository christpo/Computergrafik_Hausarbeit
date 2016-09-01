#version 110

varying vec2 Texcoord1;
varying vec2 Texcoord2;
varying vec2 Texcoord3;

varying vec3 Normal;
varying vec3 Position;

uniform sampler2D Tex1;
uniform sampler2D Tex2;
uniform sampler2D Mix;

float sat( float a){

	return clamp( a, 0.0, 0.1);
}
 
void main(void) {

	vec4 texel1 = texture2D( Tex1, Texcoord1);
	vec4 texel2 = texture2D( Tex2, Texcoord2);
	vec4 texel3 = texture2D( Mix, Texcoord3);

	vec4 color = mix(texel1, texel2, texel3);


	vec3 N = normalize(Normal);
	vec3 L = normalize(gl_LightSource[0].position.xyz - Position);
	vec3 E = normalize(-Position);
	vec3 R = normalize(reflect(-L,N));

	vec4 DiffuseComponent = gl_LightSource[0].diffuse * max( dot(N,L), 0.0);
	

	
	DiffuseComponent *= color;

	gl_FragColor = DiffuseComponent;

}