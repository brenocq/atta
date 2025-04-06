perFrame mat4 uView;
perFrame mat4 uProjection;

perVertex vec4 vColor;

vec4 vertex(vec3 iPos, vec4 iColor) {
    vec4 pos = uProjection * uView * vec4(iPos, 1.0);
    vColor = iColor;
    gl_PointSize = 5.0;
    return pos;
}

void fragment(out vec4 color) { color = vColor; }

