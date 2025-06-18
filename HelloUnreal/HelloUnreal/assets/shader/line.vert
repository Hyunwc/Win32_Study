#version 150

#ifdef GL_ES
precision mediump float;
#endif

// 보간 필요 없음 : uniform
uniform mat4 projMatrix, viewMatrix;

// 보간 계산 필요 : Attr
in vec4 position;

// cpu에게 받은 데이터
in vec4 color;
in vec4 texCoord;
in vec4 normal;

// 
out vec4 vColor;
out vec4 vTexCoord;
out vec4 vNormal;
									
void main() 
{					
	gl_Position = projMatrix * viewMatrix * position;	
	vColor = color;
	vTexCoord = texCoord;
	vNormal = normal;
}

// 파이프라인 실행 순서
// cpu -> 번역기(셰이더) ->  gpu(그리기 특화)
// 1. cpu가 gpu에게 명령 하지만 서로 다르기때문에
// 번역기가 필요하다.
// 2. 그림을 그릴 좌표를 지정(vertex shader) -> 레스터라이제이션
// 3. 색상 지정 (fragment shader) -> 픽셸세이더


// cpu -> vert -> frag -> gpu