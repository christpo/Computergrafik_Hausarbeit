#version 110

varying vec3 Position;
varying vec3 Normal;
varying vec2 Texcoord;
 
void main(void) {

	Normal = gl_NormalMatrix * gl_Normal;
	Position = (gl_ModelViewMatrix * gl_Vertex).xyz;
	Texcoord = gl_MultiTexCoord0.xy;

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}