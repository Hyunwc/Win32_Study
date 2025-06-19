#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform vec4 u_rect;
uniform vec4 u_color;
uniform float u_radius;

out vec4 fragColor;				
				
float getDistance(vec2 p, vec2 size, float radius)
{
	size -= vec2(radius);
	vec2 d = abs(p) - size;
	return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - radius;
}

void main()
{
	float d = getDistance(gl_FragCoord.xy - u_rect.xy, u_rect.zw, u_radius);
	float a = clamp(abs(d), 0.0, 1.0);
	fragColor = vec4(u_color.rgb, u_color.a * a); 
	//fragColor = vec4(a); 
}	

// 셰이더 주의사항
// cpu에서 gpu데이터 넘길 때, 값이 제대로 전달되는지(attr, uniform)
// cpu에서 값이 제대로 설정되었는가?
// 셰이더에서 값을 제대로 사용하는가?(확인불가)

