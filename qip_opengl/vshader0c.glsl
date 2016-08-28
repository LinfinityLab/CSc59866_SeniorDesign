attribute vec2  a_Position;	// attribute variable: position vector
attribute vec3  a_Color;	// attribute variable: color vector
varying   vec4  v_Color;	// varying variable for passing color to fragment shader
uniform   mat4  u_ModelMatrix;	// uniform variable for passing rotation matrix
uniform   int   u_ReverseColor;	// reverse color flag

void main() {
	gl_Position = gl_ModelViewProjectionMatrix * u_ModelMatrix * vec4(a_Position, 0, 1);
	if (u_ReverseColor == 1)
		v_Color = vec4(1.0-a_Color.r, 1.0-a_Color.g, 1.0-a_Color.b, 1.0);
	else	v_Color	= vec4(a_Color, 1);
}
