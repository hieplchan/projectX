$input v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_tex0, 0);  // sampler uniform in stage 0
uniform vec4 u_tint;   // tint color from C++

void main()
{
    vec4 texColor = texture2D(s_tex0, v_texcoord0);
    gl_FragColor = texColor * u_tint;   // apply tint (rgba multiplication)
}
