struct CameraRay
{
	vec3 origin;
	vec3 direction;
};

const uint CAMERA_TYPE_PROJECTIVE = 0;
const uint CAMERA_TYPE_ORTHOGRAPHIC = 1;

CameraRay generateRayProjectiveCamera(mat4 proj, mat4 viewMat, vec2 xy)
{
	mat4 projInv = inverse(proj);
	mat4 viewInv = inverse(viewMat);
	
	CameraRay ray;
	ray.origin = viewInv[3].xyz;
	ray.direction = normalize((viewInv*projInv*vec4(xy,1,1)).xyz);

	return ray;
}

CameraRay generateRayOrthographicCamera(mat4 viewMat, vec2 xy)
{
	CameraRay ray;
	ray.origin = vec3(1,1,0);
	ray.direction =  vec3(0, -1, 0);

	return ray;
}

CameraRay generateCameraRay(uint cameraType, mat4 viewMat, vec2 xy)
{
	switch(cameraType)
	{
	case CAMERA_TYPE_PROJECTIVE:
		//return generateRayProjectiveCamera(viewMat, xy);
		break;
	case CAMERA_TYPE_ORTHOGRAPHIC:
		return generateRayOrthographicCamera(viewMat, xy);
		break;
	default:
		//return generateRayProjectiveCamera(viewMat, xy);
	}
}
