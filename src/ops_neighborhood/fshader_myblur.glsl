#version 130

varying	vec2	    v_TexCoord;	// varying variable for passing texture coordinate from vertex shader

uniform int       u_Wsize;	// blur width value
uniform int       u_Hsize;	// blur height value
uniform float	    u_StepX;
uniform float	    u_StepY;
uniform	sampler2D u_Sampler;	// uniform variable for the texture image


void main()
{
    vec3 avg = vec3(0.0);
    vec2 tc  = v_TexCoord;
    int  w2  = u_Wsize / 2;
    int  h2  = u_Hsize / 2;

    for(int i=-h2; i<=h2; ++i)
    {
        for(int j=-w2; j<=w2; ++j)
        {
            avg += texture2D(u_Sampler, vec2(tc.x + j*u_StepX, tc.y + i*u_StepY)).rgb;
        }
    }

    avg = avg / (u_Wsize * u_Hsize);
    gl_FragColor = vec4(avg, 1.0);
}
