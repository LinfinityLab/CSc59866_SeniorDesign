#version 130

varying	vec2	    v_TexCoord;			 // varying variable for passing texture coordinate from vertex shader
uniform float	    u_StepX;   			 // step x for input texture
uniform float	    u_StepY;  			 // step y for input texture
uniform	sampler2D u_Sampler;	     // uniform variable for the input texture image
uniform int       u_SizeW_Temp;    // template texture width
uniform int       u_SizeH_Temp;    // template texture height
uniform float	    u_StepX_Temp;    // step x for template texture
uniform float	    u_StepY_Temp;    // step y for template texture
uniform	sampler2D u_Sampler_Temp;  // template texture
uniform float     u_Sqrt_Sum_Temp; // cpu computed sqrt of sum of (temp input)^2

void main() {

		vec4 convolve = vec4(0.0); // convolution value
		vec4 sum_input_sqr  = vec4(0.0);    // sum of input image square
  	vec4 sum_temp_sqr   = vec4(0.0);    // sum of template image square
		vec2 tc    = v_TexCoord;
    int  sizeW = u_SizeW_Temp / 2;
    int  sizeH = u_SizeH_Temp / 2;
    int  count = 0;

		float gg = u_Sqrt_Sum_Temp;
    for(int i=-sizeH; i<=sizeH; ++i) {
        for(int j=-sizeW; j<=sizeW; ++j) {
						// input pixel value
						vec4 in_val = texture2D(u_Sampler, vec2(tc.x + j*u_StepX, tc.y + i*u_StepY));
						// template pixel value
						vec4 temp_val = texture2D(u_Sampler_Temp, vec2(0.5 + j*u_StepX_Temp, 0.5 + i*u_StepY_Temp));
						// convolution value
            convolve += in_val * temp_val;

						sum_input_sqr += in_val * in_val;
            sum_temp_sqr  += temp_val * temp_val;
        }
    }
		// result = convolve / (sqrt(sum of I^2) * sqrt(sum of T^2))
		gl_FragColor = vec4((convolve/sqrt(sum_input_sqr)/sqrt(sum_temp_sqr)).rgb, 1.0);
}
