#version 120

varying	vec2	  v_TexCoord;	// varying variable for passing texture coordinate from vertex shader
uniform	sampler2D u_Sampler;	// uniform variable for the texture image
uniform float     u_thr;	// threshold value

void main() {
	vec3 clr = texture2D(u_Sampler, v_TexCoord).rgb;
	float r = step(u_thr, clr.r);
	float g = step(u_thr, clr.g);
	float b = step(u_thr, clr.b);
	gl_FragColor = vec4(r, g, b, 1.0);

}
