// the location = 0 should match with the index of glVertexAttribPointer
// gl_position must be a vec4, even though our component size is only 2
// defining as vec4 will make opengl cast the 2 size component to a 4
#shader vertex
#version 330 core
		
layout(location = 0) in vec4 aPosition; // the position variable has attribute position 0
// layout(location = 1) in vec3 aColor;
layout(location = 1) in vec2 aTexCoord; // the textureCoords variable has attirbute position 1

// out vec3 color;
out vec2 texCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * aPosition;
	// color = aColor;
	texCoord = aTexCoord;
}

