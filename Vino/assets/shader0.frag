//edge

#pragma STDGL invariant(all)

precision highp float;

const float ZERO = 1e-6;

varying highp float edge;

vec3 pack_depth(const in float depth)
{
    const vec3 bit_shift = vec3(256.0*256.0, 256.0, 1.0);
    const vec3 bit_mask  = vec3(0.0, 1.0/256.0, 1.0/256.0);
    vec3 res = fract(depth * bit_shift);
    res -= res.xxy * bit_mask;
    return res;
}

void main()
{
	gl_FragColor = vec4(pack_depth(edge), 1.0);
}