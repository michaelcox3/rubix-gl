// the location = 0 should match with the index of glVertexAttribPointer
// gl_position must be a vec4, even though our component size is only 2
// defining as vec4 will make opengl cast the 2 size component to a 4
#shader vertex
#version 330 core
		
layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
}	

