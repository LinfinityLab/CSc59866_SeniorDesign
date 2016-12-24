#version 130


in	vec2	  v_TexCoord;	// varying variable for passing texture coordinate from vertex shader
uniform	sampler2D u_Sampler;	// uniform variable for the texture image

void main() {
	gl_FragColor = texture2D(u_Sampler, v_TexCoord);
}
