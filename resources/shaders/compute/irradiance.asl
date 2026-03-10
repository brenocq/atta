perFrame mat4 uProjection;
perFrame mat4 uView;
perFrame samplerCube uEnvironmentMap;

perVertex vec3 localPos;

vec4 vertex(vec3 iPos, vec3 iNormal, vec2 iUV) {
    localPos = iPos;
    localPos += iNormal / 10000.0f + vec3(iUV, 0) / 10000.0f;
    return uProjection * uView * vec4(localPos, 1.0);
}

const float PI = 3.14159265359;

void fragment(out vec4 color) {
    vec3 N = normalize(localPos);

    vec3 irradiance = vec3(0.0);

    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, N));
    up         = normalize(cross(N, right));

    float sampleDelta = 0.025;
    float nrSamples = 0.0;
    for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta) {
        for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta) {
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

            irradiance += texture(uEnvironmentMap, sampleVec).rgb * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));

    color = vec4(irradiance, 1.0);
}
