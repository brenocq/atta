struct GuiUniformBufferObject
{
	float ratio;
	bool debug;
};

struct GuiObjectInfo
{
	vec4 position;
	vec2 size;
	vec4 color;
	float radius;
	int isLetter;
	int textureIndex;
	vec2 offsetLetter;
	vec2 sizeLetter;
};
