perFrame mat4 uView;
perFrame mat4 uProjection;
perDraw mat4 uModel;
perDraw vec4 uColor;

vec4 vertex(vec3 iVertex, vec3 iNormal, vec3 iUV) { return uProjection * uView * uModel * vec4(iVertex, 1.0f); }

vec4 fragment() { return uColor; }
