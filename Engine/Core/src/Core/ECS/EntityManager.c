#pragma once

#include "Core/ECS/EntityManager.h"
#include "Common/common_macros.h"
#include "Core/ECS/JobSystem/TransformSystem.h"

#include "Common/stretchy_buffer.h"


Entity create_entity(EntityManager* manager) {
	
	
	int entity_index = manager->entitys_created;
	manager->entitys_created++;
	Entity entity;
	// Note our entites shouldnt start at 0, because our map expects that no key maps to 0
	entity.id = entity_index + 1;
	if (stb_sb_count(manager->entity_list) == manager->entity_count) {
		stb_sb_push(manager->entity_list, entity);
	} else {
		manager->entity_list[manager->entity_count] = entity;
	}
	

	// adds a mapping between the entity's id, and its index
	// while it seems dumb that the key,values are obvious when adding
	// when we delete an entity, we may need to remap its index to something else
	// mapping <entity_id, index into the entity array>
	map_put(&manager->entity_index_map, entity.id, manager->entity_count);
	manager->entity_count++;
	return entity;
	

	
}

void destroy_entity(EntityManager* manager, Entity entity) {
	
	
	// Entity list before deleting [1, 2, 3, 4, 5]
	//                                          ^
	//                                     entity_count
	// Delete entity 3
	// Entity list after swapping [1, 2, 5, 4][5]
	//                                      ^
	//                                entity_count

	// Notice how [5] is still in the list at the end, we could have swapped 3 and 5
	// but it is not needed because we know how many entites we have
	// Note we are not shrinking the entity list array. it just keeps expanding untill the game/engines exits
	// because we are keeping track of how many items we have in the list
	// we differentiate between the array size, and how many entities are in the array




	
	
	// Get index of the entity we are trying to remove
	MapResult<uint64_t> result = map_get(&manager->entity_index_map, entity.id);
	if (!result.found) {
		// This entity has already been deleted 
		return;
	}
	uint64_t index = result.value;
	
	Entity last_entity_in_list_to_swap_with = manager->entity_list[manager->entity_count - 1];
	// Place the last entity in the entity list at the index we are removing from
	manager->entity_list[index] = last_entity_in_list_to_swap_with;
	manager->entity_count--;
	// Update the mapping of the swapped entity to it's new index
	map_put(&manager->entity_index_map, last_entity_in_list_to_swap_with.id, index);
	map_remove(&manager->entity_index_map, entity.id);
	

	// Need to remove it from all the other components

	for (int i = ComponentType_None; i < ComponentType_Count; i++) {
		ComponentType type = (ComponentType)i;
		remove_component(manager, entity, type);
	}

	
	
}

void add_component(EntityManager* manager, Entity entity, ComponentType type) {
	switch (type) {
		case ComponentType_Transform: {
			entity_add_transform_component(&manager->transforms, entity);
			break;
		}
		case ComponentType_Camera: {
			entity_add_camera_component(&manager->cameras, entity);
			break;
		}
		case ComponentType_StaticMesh: {
			entity_add_mesh_component(&manager->static_meshs, entity);
			break;
		}
		case ComponentType_Light: {
			entity_add_light_component(&manager->lights, entity);
			break;
		}
	}
}


void remove_component(EntityManager* manager, Entity entity, ComponentType type) {
	switch (type) {
		case ComponentType_Transform: {
			entity_remove_transform_component(&manager->transforms, entity);
			break;
		}
		case ComponentType_Camera: {
			entity_remove_camera_component(&manager->cameras, entity);
			break;
		}
		case ComponentType_StaticMesh: {
			entity_remove_mesh_component(&manager->static_meshs, entity);
			break;
		}
		case ComponentType_Light: {
			entity_remove_light_component(&manager->lights, entity);
			break;
		}
	}
}


void init_entity_manager(EntityManager* manager) {
	arena_init(&manager->arena);


	manager->entity_count = 0;
	manager->entitys_created = 0;
	manager->entity_list = NULL;
	map_init(&manager->entity_index_map);


	size_t mem_size = ECS_MEMORY;
	void* mem_block = arena_alloc(&manager->arena, mem_size);
	mem_size = manager->arena.end - cast(char*) mem_block;
	stack_alloc_init(&manager->stack_mem, mem_block, mem_size);
	
	
	init_transform_manager(&manager->transforms);
	init_static_mesh_manager(&manager->static_meshs);
	init_camera_manager(&manager->cameras);
	init_light_manager(&manager->lights);

	

}

void destroy_entity_manager(EntityManager* manager) {
	arena_free(&manager->arena);
	stb_sb_free(manager->entity_list);

	
	destroy_transform_manager(&manager->transforms);
	destroy_static_mesh_manager(&manager->static_meshs);
	destroy_camera_manager(&manager->cameras);
	destroy_light_manager(&manager->lights);
}

void attach_child_entity(EntityManager* manager, Entity entity, Entity child) {

	MapResult<uint64_t> result = map_get(&manager->transforms.id_map, entity.id);
	if (!result.found) { return; }
	
	
	uint64_t index = result.value;

	Entity* first_child = &manager->transforms.first_child[index];
	if (first_child->id == NO_ENTITY_ID) {
		// This entity has no children, so we need to add this child to the first child
		first_child->id = child.id;
	} else {
		// This entity already has a first child so we need to look at the siblings
		Entity* next_sibling = &manager->transforms.next_sibling[first_child->id];

		while (next_sibling->id != NO_ENTITY_ID) {
			next_sibling = &manager->transforms.next_sibling[next_sibling->id];
		}
		next_sibling->id = child.id;
		
	}
	MapResult<uint64_t> child_result = map_get(&manager->transforms.id_map, child.id);
	if (!child_result.found) {
		return;
	}

	// Assign the child, the parent entity
	uint64_t child_index = child_result.value;
	
	manager->transforms.parent[child_index] = entity;
}

