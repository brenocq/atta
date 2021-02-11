struct CameraRay
{
	vec3 origin;
	vec3 direction;
};

const uint CAMERA_TYPE_PROJECTIVE = 0;
const uint CAMERA_TYPE_ORTHOGRAPHIC = 1;

CameraRay generateRayProjectiveCamera(mat4 proj, mat4 viewMat, vec2 xy)
{
	CameraRay ray;
	ray.origin = transpose(inverse(viewMat))[3].xyz;
	ray.direction = normalize(vec3(proj*transpose(viewMat)*vec4(xy,1,0)));

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
