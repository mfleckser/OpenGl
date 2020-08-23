#version 300 es

precision mediump float;

in vec2 texCoord0;
in vec3 normal0;

uniform sampler2D texture;
uniform vec3 lightDirection;
uniform bool isSky;
uniform bool isGround;

out vec4 FragColor;

void main() {
	//if(isSky || gl_FrontFacing || isGround) {
		FragColor = texture2D(texture, vec2(texCoord0.x, 1.0f-texCoord0.y));
	//} else {
		//FragColor = vec4(0,0,0,0);
	//}
}
//  * clamp(dot(-lightDirection, normal0), 0.5, 1.0)
