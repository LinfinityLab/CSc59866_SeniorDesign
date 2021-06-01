#version 120

varying	vec2	  v_TexCoord;	// varying variable for passing texture coordinate from vertex shader

uniform int     u_SizeW;	// blur width value
uniform int     u_SizeH;   // blur height value
uniform float	  u_StepX;
uniform float	  u_StepY;
uniform float     u_Kernel[81];
uniform	sampler2D u_Sampler;	// uniform variable for the texture image

void main()
{
    vec4 avg = vec4(0.0);
    vec2 tc  = v_TexCoord;
    int sizeW = u_SizeW / 2;
    int sizeH = u_SizeH / 2;
    int count = 0;

    for(int i=-sizeH; i<=sizeH; ++i)
    {
        for(int j=-sizeW; j<=sizeW; ++j)
        {
            avg += (texture2D(u_Sampler, vec2(tc.x + j*u_StepX, tc.y + i*u_StepY)) * u_Kernel[count++]);
        }
    }

    gl_FragColor = vec4(clamp(avg.rgb, 0.0, 1.0), 1.0);
}
