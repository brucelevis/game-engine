#pragma once


#include "Common/Arena.h"
#include "Common/StackAllocator.h"

#include "Core/ECS/Component/Camera.h"
#include "Core/ECS/Component/Transform.h"
#include "Core/ECS/Component/StaticMesh.h"



#define ECS_MEMORY MEGABYTES(40)

typedef enum ComponentType {
	ComponentType_None,
	ComponentType_Transform,
	ComponentType_Camera,
	ComponentType_StaticMesh,
} ComponentType;

typedef struct ComponentManager {
	TransformManager transform_manager;
	CameraManager camera_manager;
	StaticMeshManager static_mesh_manager;
} ComponentManager;


typedef struct Entity {
	int entity_id;
} Entity;



typedef struct EntityManager {
	enum EntityCountList {
		// TODO: the size should not be fixed. for now we are just testing
		size = 10
	};
	ComponentManager comp_manager;
	int last_entity_index = 0;

	Arena arena;
	StackAllocator stack_mem;

	// TODO: eventually use something better
	Entity entity_list[size];
	
} EntityManager;

void init_entity_manager(EntityManager* manager);
void destroy_entity_manager(EntityManager* manager);
Entity* create_entity(EntityManager* manager);
void add_component(EntityManager* manager, Entity* entity, ComponentType type);


void attach_child_entity(EntityManager* manager, Entity entity, Entity child);


Camera* get_camera(EntityManager* manager, Entity entity);
StaticMesh* get_static_mesh(EntityManager* manager, Entity entity);
