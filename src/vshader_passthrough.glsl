#version 130


in  vec2	a_Position;	// attribute variable: position vector
in  vec2	a_TexCoord;	// attribute variable: texture coordinate
out vec2	v_TexCoord;	// varying variable for passing texture coordinate to fragment shader


void main() {

	gl_Position = vec4(a_Position, 0, 1);
	v_TexCoord = a_TexCoord;
}
