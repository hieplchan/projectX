$input a_position
$uniform mat4 u_modelViewProj;

void main() {
    gl_Position = u_modelViewProj * vec4(a_position.xy, 0.0, 1.0);
}