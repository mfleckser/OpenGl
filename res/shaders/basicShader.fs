#version 300 es

precision mediump float;

in vec2 texCoord0;
in vec3 normal0;

uniform sampler2D texture;
uniform vec3 lightDirection;
uniform int width;
uniform int height;

out vec4 FragColor;

void main() {
	//if(gl_FragCoord.x > 0 && gl_FragCoord.y > 0) {
		//FragColor = vec4(1,0,0,1);
		FragColor = texture2D(texture, vec2(texCoord0.x, 1.0f - texCoord0.y));
	//}
}
//  * clamp(dot(-lightDirection, normal0), 0.5, 1.0)
