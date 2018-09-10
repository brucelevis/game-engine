#pragma once

#include "Entities/Camera.h"
#include "Entities/Skybox.h"
#include "Entities/Lights.h"

#include "StaticMesh.h"
#include "TextureData.h"



typedef enum SceneNodeType {
	SceneNodeType_Root,
	SceneNodeType_Camera,
	SceneNodeType_StaticMesh,
} SceneNodeType;


typedef struct SceneNodeArray {
	int size;
	int capacity;
	struct SceneNode* nodes;
} SceneNodeArray;


typedef struct SceneNode {
	SceneNodeType type;

	struct SceneNode* parent;
	SceneNodeArray children;

	union {
		Camera camera;
		StaticMesh mesh;
	};
	

} SceneNode;









typedef struct Scene {
	int scene_id;
	
	// TODO: implement proper tree with root and child transforms 
	//SceneNode root;
	// LinearAllocator* scene_mem;

	DirectionalLight test_light;


	Camera main_camera;
	


	HDR_SkyMap hdr_skymap;

	StaticMesh mesh_test;
	StaticMesh mesh_test2;

	SimpleTexture albedo_map;
	SimpleTexture normal_map;
	SimpleTexture metallic_map;
	SimpleTexture roughness_map;
	SimpleTexture ao_map;
	
	StaticMesh flat_plane;
} Scene;


