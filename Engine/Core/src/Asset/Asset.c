#pragma once

#include "Asset/Asset.h" 

#include <stdio.h>

#include "Common/common_macros.h"
#include "debug_macros.h"

void init_asset_tracker(AssetTracker* tracker) {
	map_init(&tracker->track_map);
	map_grow(&tracker->track_map, 16);
	arena_init(&tracker->mem);


	FILE* file;
	errno_t err;
	const char* track_file = ASSET_TRACKER_FILE;


	// try reading first
	// if the file doesnt exist, then we create it
	err = fopen_s(&file, track_file, "rb");


	if (err == 0) {
		// File exists
		// construct the tracker data from the file
		void* buffer[256];
		
		fread(buffer, sizeof(tracker->last_asset_id), 1, file);
		tracker->last_asset_id = *cast(uint64_t*)buffer;

		fread(buffer, sizeof(tracker->assets_tracked), 1, file);
		tracker->assets_tracked = *cast(uint64_t*)buffer;


		for (int i = 0; i < tracker->assets_tracked; i++) {
			AssetTrackData track_data;

			// Read the id
			fread(buffer, sizeof(uint64_t), 1, file);
			AssetID id;
			id.id = *cast(uint64_t*)buffer;

			// Read the filename length
			fread(buffer, sizeof(uint32_t), 1, file);
			track_data.filename_length = *cast(uint32_t*)buffer;


			// Read the filename str, and copy it our own memory storage
			fread(buffer, track_data.filename_length, 1, file);
			track_data.filename = cast(char*) arena_alloc(&tracker->mem, track_data.filename_length);
			snprintf(track_data.filename, track_data.filename_length, "%s", buffer);
			map_put(&tracker->track_map, id.id, track_data);
			
		}

	



		err = fclose(file);
		if (err == 0) {
			debug_print("Closed %s\n", track_file);
		} else {
			assert_fail();
			debug_print("Cannot close to %s\n", track_file);
		}

	} else {
		// File doesnt exist, create it for the first time


		err = fopen_s(&file, track_file, "wb");

		debug_print("First time writing to %s,", track_file);

		if (err == 0) {
			tracker->last_asset_id = 1;
			tracker->assets_tracked = 0;

			fwrite(cast(const void*) &tracker->last_asset_id, sizeof(tracker->last_asset_id), 1, file);
			fwrite(cast(const void*) &tracker->assets_tracked, sizeof(tracker->assets_tracked), 1, file);
		} else {
			assert_fail();
		}
		
		

		err = fclose(file);
		if (err == 0) {
			debug_print("Finished writing to %s\n", track_file);
		} else {
			assert_fail();
			debug_print("Cannot close to %s\n", track_file);
		}

	}

}

void destroy_asset_tracker(AssetTracker* tracker) {
	update_tracker_file(tracker);
	map_destroy(&tracker->track_map);
	arena_free(&tracker->mem);
}

void track_asset(AssetTracker* tracker, AssetID id, char* filename, uint32_t filename_length) {
	MapResult<AssetTrackData> result = map_get(&tracker->track_map, id.id);

	
	AssetTrackData track_data;
	if (result.found) {
		// This asset is already being, tracked and will be overrided
		track_data = result.value;
	} else {
		// New asset that hasnt been tracked before	
		tracker->assets_tracked++;
	}

	track_data.filename_length = filename_length;
	// We need to copy the filename to our own memory
	track_data.filename = cast(char*) arena_alloc(&tracker->mem, filename_length);
	snprintf(track_data.filename, filename_length, "%s", filename);

	
	map_put(&tracker->track_map, id.id, track_data);
	
}

uint64_t next_asset_id(AssetTracker* tracker) {
	return tracker->last_asset_id++;
}

void update_tracker_file(AssetTracker* tracker) {
	// File doesnt exist, create it for the first time


	FILE* file;
	errno_t err;
	const char* track_file = ASSET_TRACKER_FILE;

	err = fopen_s(&file, track_file, "wb");

	
	if (err == 0) {

		fwrite(cast(const void*) &tracker->last_asset_id, sizeof(tracker->last_asset_id), 1, file);
		fwrite(cast(const void*) &tracker->assets_tracked, sizeof(tracker->assets_tracked), 1, file);

		size_t map_size = tracker->track_map.size;
		// Go over our track map, and write the tracked assets to file
		for (int i = 0; i < map_size; i++) {
			CompactMapItem<AssetTrackData> track_item = tracker->track_map.map[i];
			// Dont write the item, if the asset id/key is either 0, or a tombstone(deleted value)
			if (track_item.key != 0 && track_item.key != TOMBSTONE) {
				// Write the asset id
				fwrite(cast(const void*) &track_item.key, sizeof(track_item.key), 1, file);
				// Write the length of the filename
				fwrite(cast(const void*) &track_item.value.filename_length, sizeof(track_item.value.filename_length), 1, file);
				// Write the actual filename string
				fwrite(cast(const void*) track_item.value.filename, track_item.value.filename_length, 1, file);
			}
		}
		

	} else {
		assert_fail();
	}

	err = fclose(file);
	if (err == 0) {
		debug_print("Finished writing to %s\n", track_file);
	} else {
		assert_fail();
		debug_print("Cannot close to %s\n", track_file);
	}
}


void init_scene_node(AssetImport_SceneNode* node, uint64_t id, char* name, uint32_t name_length) {
	node->id = id;
	node->name = name;
	node->name_length = name_length;
	//node->parent = NULL;
	node->children_count = 0;
	node->first_child = NULL;
	node->next_sibling = NULL;
	node->mesh_count = 0;
	node->meshes = NULL;
}

void set_scene_node_name(AssetImport_SceneNode* node, char* name, uint32_t name_length) {
	node->name = name;
	node->name_length = name_length;
}

void add_child_to_scene_node(AssetImport_SceneNode* node, AssetImport_SceneNode* child) {
	// Attach the parent to the child node
	//child->parent = node;
	if (node->children_count == 0) {
		node->first_child = child;
	} else {
		// Find the end of the child list
		AssetImport_SceneNode* ptr = node->first_child;
		while (ptr->next_sibling != NULL) {
			ptr = ptr->next_sibling;
		}
		ptr->next_sibling = child;	
	}

	node->children_count++;
}

void set_scene_node_transform(AssetImport_SceneNode* node, Vec3f pos, Vec3f scale, Vec3f rot) {
	//Transform transform;
	//transform.position = pos;
	//transform.scale = scale;
	//transform.rotation = euler_to_quat(rot);
	//node->transform = trs_mat_from_transform(&transform);

	node->translation = pos;
	node->scale = scale;
	node->rotation = rot;

}
