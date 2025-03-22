perFrame mat4 projection;
perFrame mat4 view;
perDraw mat4 model;
perDraw int entityId;

vec4 vertex(vec3 inPos, vec3 inNormal, vec2 inUV) { return projection * view * model * vec4(inPos, 1.0f); }

void fragment(out int eid) { eid = entityId; }
