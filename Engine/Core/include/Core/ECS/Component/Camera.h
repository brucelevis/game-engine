#pragma once

#include "Math/Vec.h"
#include "Math/Mat.h"



#include "Common/Map.h"

#include "Core/ECS/Entity.h"

struct EntityManager;


typedef struct Camera {
	
	//Transform transform;

	float near;
	float far;
	float fov;
	float aspect_ratio;
	Mat4x4f view_mat;
	Camera() {
		near = 0.1f;
		far = 10.0f;
		fov = 90.0f;
		aspect_ratio = 1.0f;
		view_mat = Mat4x4f();
	}
} Camera;


void init_camera_params(Camera* camera, float near, float far, float fov, float aspect_ratio);


typedef struct CameraManager {
	CompactMap<uint64_t> id_map;
	Camera* cameras;
	uint64_t count;
} CameraManager;



void init_camera_manager(CameraManager* manager);
void destroy_camera_manager(CameraManager* manager);
void entity_add_camera_component(CameraManager* manager, Entity entity);