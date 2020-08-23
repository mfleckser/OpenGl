#version 300 es

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out vec2 texCoord0;
out vec3 normal0;

uniform mat4 transform;
uniform bool isSky;

void main() {
	vec4 pos = transform * vec4(position, 1.0);
	if(isSky) {
		gl_Position = pos.xyww;
	} else {
		gl_Position = pos;
	}
	texCoord0 = texCoord;
	normal0 = (transform * vec4(normal, 0)).xyz;
}
