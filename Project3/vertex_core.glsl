#version 440

layout (location = 0) in vec3 vertex_position;  // Regular 3D vertex position
layout (location = 1) in vec3 vertex_color;     // Regular vertex color
layout (location = 2) in vec2 vertex_texcoord;  // Regular vertex texture coordinates
layout (location = 3) in vec3 vertex_normal;    // Regular vertex normal
layout (location = 4) in vec4 vertex_text;      // Text vertex: <vec2 pos, vec2 tex>

out vec3 vs_position;
out vec3 vs_color;
out vec2 vs_texcoord;
out vec3 vs_normal;
out vec2 TextCoords;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 TextProjectionMatrix;  // Projection matrix specifically for text rendering
uniform bool isTextRendering;       // Uniform to indicate if we're rendering text

void main() {
    if (isTextRendering) {
        // Text rendering path
        gl_Position = TextProjectionMatrix * vec4(vertex_text.xy, 0.0, 1.0);
        TextCoords = vertex_text.zw;

        // Dummy values for outputs used in regular rendering
        vs_position = vec3(0.0);
        vs_color = vec3(0.0);
        vs_texcoord = vec2(0.0);
        vs_normal = vec3(0.0);
    } else {
        // Regular 3D rendering path
        vs_position = (ModelMatrix * vec4(vertex_position, 1.0)).xyz;
        vs_color = vertex_color;
        vs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1.0);
        vs_normal = mat3(ModelMatrix) * vertex_normal;

        gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.0);
    }
}
