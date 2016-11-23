#version 120

varying	vec2	  v_TexCoord;	// varying variable for passing texture coordinate from vertex shader

uniform float     u_Thr1;	// threshold value
uniform float     u_Thr2;	// threshold value
uniform	sampler2D u_Sampler;	// uniform variable for the texture image


void main() {

	vec3 clr = texture2D(u_Sampler, v_TexCoord).rgb;
	clr = clamp(clr, u_Thr1, u_Thr2);
	float scale = 1.0/(u_Thr2-u_Thr1);
	float r = (clr.r - u_Thr1) * scale;
	float g = (clr.g - u_Thr1) * scale;
	float b = (clr.b - u_Thr1) * scale;
	gl_FragColor = vec4(r, g, b, 1.0);
}