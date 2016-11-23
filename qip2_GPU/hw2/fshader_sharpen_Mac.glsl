#version 120

varying	vec2	  v_TexCoord;	// varying variable for passing texture coordinate from vertex shader

uniform int		u_Size;	// blur width value
uniform int		u_Factor;
uniform highp float	u_StepX;
uniform highp float	u_StepY;
uniform	sampler2D	u_Sampler;	// uniform variable for the texture image


void main() 
{

	highp vec3 avg = vec3(0.0);
	vec2 tc  = v_TexCoord;
	int  w2  = u_Size / 2;
	for(int i=-w2; i<=w2; ++i)
		for(int j=-w2; j<=w2; ++j)
			avg += texture2D(u_Sampler, vec2(tc.x + j*u_StepX, tc.y + i*u_StepY)).rgb;
	avg = avg / (u_Size*u_Size);
	vec3 clr = texture2D(u_Sampler, tc).rgb;
	highp vec3 diff = (clr - avg) * u_Factor;
	clr += diff;
	gl_FragColor = vec4(clr, 1.0);
}