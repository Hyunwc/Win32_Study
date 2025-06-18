#version 150

#ifdef GL_ES
precision mediump float;
#endif

// 보간 필요 없음 : uniform
uniform mat4 projMatrix, viewMatrix;

// cpu에게 받은 데이터
in vec4 position;
in vec4 color;
in vec2 texCoord;

out vec4 vColor;
out vec2 vTexCoord;
									
void main() 
{					
	gl_Position = projMatrix * viewMatrix * position;	
	vColor = color; 
	vTexCoord = texCoord;
}

