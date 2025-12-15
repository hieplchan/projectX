$input a_position, a_texcoord0
$output v_texcoord0

#include <bgfx_shader.sh>

void main()
{
    // Apply model * view * projection automatically
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));

    // Pass UV through to fragment stage
    v_texcoord0 = a_texcoord0;
}
