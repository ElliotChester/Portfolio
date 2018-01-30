// Rename this file: "Wk10-1 Vertex.vert"
//vertex shader
#version 430 core 

// IN variables must be defined in main program which compiles the shaders
in vec3 VertexPosition;
in vec3 VertexColor;

// OUT variables sent from vertex to fragment shader
out vec3 Color; 

void main()
{
	Color = VertexColor;

	// gl_Position is a BUILT-IN out variable
	gl_Position = vec4(VertexPosition,1.0);
}