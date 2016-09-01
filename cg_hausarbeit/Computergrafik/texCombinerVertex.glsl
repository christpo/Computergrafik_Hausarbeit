#version 110

varying vec2 Texcoord1;
varying vec2 Texcoord2;
varying vec2 Texcoord3;

varying vec3 Normal;
varying vec3 Position;
 
void main(void) {

	Texcoord1 = gl_MultiTexCoord0.xy;
	Texcoord2 = gl_MultiTexCoord1.xy;
	Texcoord3 = gl_MultiTexCoord2.xy;

	Normal = gl_NormalMatrix * gl_Normal;
	Position = (gl_ModelViewMatrix * gl_Vertex).xyz;

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}