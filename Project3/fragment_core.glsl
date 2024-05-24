in vec3 vs_position;
in vec3 vs_color;
in vec2 bs_texcoord;

out vec4 fs_color;

void main() {
	fs_color = vec4(vs_color, 1.f);
}