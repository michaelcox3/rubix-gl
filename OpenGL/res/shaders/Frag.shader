#shader fragment
#version 330 core
		
layout(location = 0) out vec4 FragColor;

// in vec3 color;
in vec2 texCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, texCoord);
	FragColor = texColor * u_Color;
}	