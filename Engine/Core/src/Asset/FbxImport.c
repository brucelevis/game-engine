#pragma once

#include "Asset/FbxImport.h"

#include <stdio.h>
#include <string.h>




#include "Common/stretchy_buffer.h"
#include "Common/common_macros.h"
#include "debug_macros.h"




void init_asset_importer(AssetImporter* importer, AssetTracker* tracker) {
	arena_init(&importer->mem);
	size_t importer_mem_size = MEGABYTES(100);
	void* start = arena_alloc(&importer->mem, importer_mem_size);
	stack_alloc_init(&importer->stack, start, importer_mem_size);
	importer->stack_allocs_count = 0;

	importer->tracker = tracker;

	

}

void destroy_asset_importer(AssetImporter* importer) {
	stack_reset(&importer->stack);
	arena_free(&importer->mem);
}


static AssetID fbx_convert_geo2static_mesh_and_export(AssetImporter* importer, FBX_Geometry_Object* geo, Vec3f pos, Vec3f scale, Vec3f rotation, char* filename, int filename_str_len) {


	AssetID id;
	id.id = next_asset_id(importer->tracker);
	id.type = AssetType_StaticMesh;


	char file_str[256];
	uint32_t str_size = filename_str_len + ASSET_FILE_EXTENSION_LENGTH;
	snprintf(file_str, str_size, "%s%s", filename, ASSET_FILE_EXTENSION);

	track_asset(importer->tracker, id, file_str, str_size);


	FILE* file;
	errno_t err;

	err = fopen_s(&file, file_str, "wb");  // write binary

	if (err == 0) {
		debug_print("Writing to %s,", file_str);
	} else {
		assert_fail();
		debug_print("Fail writing to %s,", file_str);
	}
	//void* buffer[256];


	// Write asset id
	fwrite(cast(const void*) &id.id, sizeof(id.id), 1, file);

	// Write type of asset
	AssetType type = AssetType_StaticMesh;
	fwrite(cast(const void*) &type, sizeof(type), 1, file);

	// Write the transform
	fwrite(cast(const void*) &pos, sizeof(pos), 1, file);
	fwrite(cast(const void*) &scale, sizeof(scale), 1, file);
	fwrite(cast(const void*) &rotation, sizeof(rotation), 1, file);
	
	int vertex_count = geo->index_count * 3;
	int index_count = geo->index_count;

	fwrite(cast(const void*) &vertex_count, sizeof(vertex_count), 1, file);
	fwrite(cast(const void*) &index_count, sizeof(index_count), 1, file);
	
	// TODO: compress output buffers
	//z_stream defstream;
	//defstream.zalloc = Z_NULL;
	//defstream.zfree = Z_NULL;
	//defstream.opaque = Z_NULL;


	//defstream.avail_in = (uInt)mesh->vertex_count * sizeof(Vec3f); // size of uncompressed input buffer
	//defstream.next_in = (Bytef *)mesh->pos; // input char array
	//defstream.avail_out = (uInt)sizeof(b); // size of output
	//defstream.next_out = (Bytef *)b; // output char array


	//deflateInit(&defstream, Z_BEST_COMPRESSION);
	//deflate(&defstream, Z_FINISH);
	//deflateEnd(&defstream);


	// Write the positions straight to the file
	for (int i = 0; i < geo->index_count; i++) {
		fwrite(cast(const void*) &geo->pos[geo->indices[i].x], sizeof(Vec3f), 1, file);
		fwrite(cast(const void*) &geo->pos[geo->indices[i].y], sizeof(Vec3f), 1, file);
		fwrite(cast(const void*) &geo->pos[geo->indices[i].z], sizeof(Vec3f), 1, file);
	}

	// Write indices straight to the file
	for (int i = 0; i < geo->index_count; i++) {
		fwrite(cast(const void*) &Vec3i((3 * i), (3 * i) + 1, (3 * i) + 2), sizeof(Vec3i), 1, file);
	}

	// Write normals straight to file
	for (int i = 0; i < geo->index_count; i++) {

		//fwrite(cast(const void*) &geo->normal[(3 * i) + 0], sizeof(Vec3f), 1, file);
		//fwrite(cast(const void*) &geo->normal[(3 * i) + 1], sizeof(Vec3f), 1, file);
		//fwrite(cast(const void*) &geo->normal[(3 * i) + 2], sizeof(Vec3f), 1, file);

		fwrite(cast(const void*) &geo->normal[0], sizeof(Vec3f), 1, file);
		fwrite(cast(const void*) &geo->normal[1], sizeof(Vec3f), 1, file);
		fwrite(cast(const void*) &geo->normal[2], sizeof(Vec3f), 1, file);
	}

	if (geo->uv_count > 0) {

		for (int i = 0; i < geo->index_count; i++) {

			//fwrite(cast(const void*) &geo->texcoords[geo->uv_indices[i].x], sizeof(Vec2f), 1, file);
			//fwrite(cast(const void*) &geo->texcoords[geo->uv_indices[i].y], sizeof(Vec2f), 1, file);
			//fwrite(cast(const void*) &geo->texcoords[geo->uv_indices[i].z], sizeof(Vec2f), 1, file);

			fwrite(cast(const void*) &geo->texcoords[0], sizeof(Vec2f), 1, file);
			fwrite(cast(const void*) &geo->texcoords[1], sizeof(Vec2f), 1, file);
			fwrite(cast(const void*) &geo->texcoords[2], sizeof(Vec2f), 1, file);
		}
	}



	err = fclose(file);
	if (err == 0) {
		debug_print("Finished writing to %s\n", file_str);
	} else {
		assert_fail();
		debug_print("Cannot close to %s\n", file_str);
	}





	return id;
}

AssetID export_static_mesh(AssetImporter* importer, StaticMesh* mesh, Vec3f pos, Vec3f scale, Vec3f rotation, char* filename, int filename_str_len) {

	AssetID id;
	id.id = next_asset_id(importer->tracker);
	id.type = AssetType_StaticMesh;

	

	char file_str[256];
	uint32_t str_size = filename_str_len + ASSET_FILE_EXTENSION_LENGTH;
	snprintf(file_str, str_size, "%s%s", filename, ASSET_FILE_EXTENSION);

	track_asset(importer->tracker, id, file_str, str_size);


	FILE* file;
	errno_t err;

	err = fopen_s(&file, file_str, "wb");  // write binary

	if (err == 0) {
		debug_print("Writing to %s,", file_str);
	} else {
		assert_fail();
		debug_print("Fail writing to %s,", file_str);
	}
	//void* buffer[256];

	

	// Write asset id
	fwrite(cast(const void*) &id.id, sizeof(id.id), 1, file);

	AssetType type = AssetType_StaticMesh;
	fwrite(cast(const void*) &type, sizeof(type), 1, file);

	fwrite(cast(const void*) &pos, sizeof(pos), 1, file);
	fwrite(cast(const void*) &scale, sizeof(scale), 1, file);
	fwrite(cast(const void*) &rotation, sizeof(rotation), 1, file);

	fwrite(cast(const void*) &mesh->vertex_count, sizeof(mesh->vertex_count), 1, file);
	fwrite(cast(const void*) &mesh->index_count, sizeof(mesh->index_count), 1, file);
	//fwrite(cast(const void*) &mesh->normal_count, sizeof(mesh->normal_count), 1, file);
	//fwrite(cast(const void*) &mesh->uv_count, sizeof(mesh->uv_count), 1, file);
	//fwrite(cast(const void*) &mesh->uv_index_count, sizeof(mesh->uv_index_count), 1, file);

	// TODO: compress output buffers
	//z_stream defstream;
	//defstream.zalloc = Z_NULL;
	//defstream.zfree = Z_NULL;
	//defstream.opaque = Z_NULL;


	//defstream.avail_in = (uInt)mesh->vertex_count * sizeof(Vec3f); // size of uncompressed input buffer
	//defstream.next_in = (Bytef *)mesh->pos; // input char array
	//defstream.avail_out = (uInt)sizeof(b); // size of output
	//defstream.next_out = (Bytef *)b; // output char array

	
	//deflateInit(&defstream, Z_BEST_COMPRESSION);
	//deflate(&defstream, Z_FINISH);
	//deflateEnd(&defstream);

	if (mesh->pos) {
		fwrite(cast(const void*) mesh->pos, mesh->vertex_count * sizeof(Vec3f), 1, file);
	}

	if (mesh->indices) {
		fwrite(cast(const void*) mesh->indices, mesh->index_count * sizeof(Vec3i), 1, file);
	}

	if (mesh->normal) {
		fwrite(cast(const void*) mesh->normal, mesh->vertex_count * sizeof(Vec3f), 1, file);
	}

	if (mesh->texcoords) {
		fwrite(cast(const void*) mesh->texcoords, mesh->vertex_count * sizeof(Vec2f), 1, file);
	}

	if (mesh->lightmap_texcoords) {
		//fwrite(cast(const void*) mesh->lightmap_texcoords, mesh->vertex_count * sizeof(Vec2f), 1, file);
	}





	err = fclose(file);
	if (err == 0) {
		debug_print("Finished writing to %s\n", file_str);
	} else {
		assert_fail();
		debug_print("Cannot close to %s\n", file_str);
	}
		

	
		
		
	return id;
	
}

static void write_scene_node(AssetImporter* importer, AssetImport_SceneNode* node, FILE* file, bool parent_is_root) {

	

	uint32_t node_size =
		+ sizeof(node->id)
		+ sizeof(node->name_length)
		+ node->name_length
		+ sizeof(node->translation)
		+ sizeof(node->scale)
		+ sizeof(node->rotation)
		+ sizeof(node->children_count)
		+ node->children_count * sizeof(uint64_t)
		+ sizeof(node->mesh_count)
		+ node->mesh_count * sizeof(uint32_t);


	// Node size
	fwrite(cast(const void*) &node_size, sizeof(uint32_t), 1, file);
	
	// Node id
	fwrite(cast(const void*) &node->id, sizeof(node->id), 1, file);
	// Length of node name
	fwrite(cast(const void*) &node->name_length, sizeof(node->name_length), 1, file);
	// The node name
	fwrite(cast(const void*) node->name, node->name_length, 1, file);


	// Transform
	Vec3f export_pos = node->translation;
	Vec3f export_rotation = node->rotation;
	Vec3f export_scale = node->scale;


	// TODO: better way to check is root without strcmp
	bool is_root = strcmp(node->name, "Root") == 0;
	
	

	
	

	
	export_pos.x = node->translation.data[importer->global_settings.coord_axis];
	export_pos.y = node->translation.data[importer->global_settings.up_axis];
	export_pos.z = node->translation.data[importer->global_settings.front_axis];

	export_scale.x = node->scale.data[importer->global_settings.coord_axis];
	export_scale.y = node->scale.data[importer->global_settings.up_axis];
	export_scale.z = node->scale.data[importer->global_settings.front_axis];

	export_rotation.x = node->rotation.data[importer->global_settings.coord_axis];
	export_rotation.y = node->rotation.data[importer->global_settings.up_axis];
	export_rotation.z = node->rotation.data[importer->global_settings.front_axis];

	

	
	if (parent_is_root) {
		
		//Vec3f axes[3] = { Vec3f_Right, Vec3f_Up, Vec3f_Forward };
		//
		//Vec3f coord_axis = axes[importer->global_settings.coord_axis] * importer->global_settings.coord_axis_sign;
		//Vec3f front_axis = axes[importer->global_settings.front_axis] * importer->global_settings.front_axis_sign;
		//
		//Vec3f test_up_axis = cross(front_axis, coord_axis);
		//Vec3f original_up_axis = axes[ABS(importer->global_settings.original_up_axis)] * copysignf_(1, importer->global_settings.original_up_axis);
		//Vec3f up_axis = axes[importer->global_settings.up_axis] * importer->global_settings.up_axis_sign;

		
		
		//if (test_up_axis == up_axis) {
		//	// This is right hand
		//	// do nothing
		//	//DEBUG_BREAK;
		//} else {
			// left hand to right hand
			//export_pos.x *= -1;
			////DEBUG_BREAK;
			//
			//Mat4x4f rx = rotate(DEG2RAD(export_rotation.x), Vec3f_Right);
			//Mat4x4f ry = rotate(DEG2RAD(export_rotation.y), Vec3f_Up);
			//Mat4x4f rz = rotate(DEG2RAD(export_rotation.z), Vec3f_Forward);
			//
			//Mat4x4f lh = ry * rx * rz;
			//Mat4x4f rh = lh;
			//
			//rh.mat2d[0][2] *= -1;
			//rh.mat2d[1][2] *= -1;
			//rh.mat2d[2][0] *= -1;
			//rh.mat2d[2][1] *= -1;
			//
			//export_rotation = rotation_mat_to_euler(rh);
		//}
		


		// Scale by scale factor
		export_pos = export_pos * importer->global_settings.unit_scale_factor;;
		export_scale = export_scale  * importer->global_settings.unit_scale_factor;
		
		debug_print("root object\n");
	}


	if (is_root) {
		parent_is_root = true;
	} else {
		parent_is_root = false;
	}
	
	debug_print("%s, POS:[%f, %f, %f]\tROT:[%f, %f, %f]\tSCALE:[%f,%f,%f]\n", node->name,
		export_pos.x, export_pos.y, export_pos.z,
		export_rotation.x, export_rotation.y, export_rotation.z,
		export_scale.x, export_scale.y, export_scale.z);
	

	fwrite(cast(const void*) &export_pos, sizeof(node->translation), 1, file);
	fwrite(cast(const void*) &export_scale, sizeof(node->scale), 1, file);
	fwrite(cast(const void*) &export_rotation, sizeof(node->rotation), 1, file);
	// amount of children this node has
	fwrite(cast(const void*) &node->children_count, sizeof(node->children_count), 1, file);


	// Write ids of child nodes
	AssetImport_SceneNode* child = node->first_child;
	while (child != NULL) {
		fwrite(cast(const void*) &child->id, sizeof(child->id), 1, file);
		child = child->next_sibling;
	}

	// Mesh count
	fwrite(cast(const void*) &node->mesh_count, sizeof(node->mesh_count), 1, file);
	// Mesh indices to the scene mesh list
	for (int j = 0; j < node->mesh_count; j++) {
		fwrite(cast(const void*) &node->meshes[j], sizeof(node->meshes[j]), 1, file);
	}
	// Recursivly write the node tree
	if (node->first_child != NULL) {
		write_scene_node(importer, node->first_child, file, parent_is_root);
		if (node->first_child->next_sibling != NULL) {
			AssetImport_SceneNode* ptr = node->first_child->next_sibling;
			while (ptr != NULL) {
				write_scene_node(importer, ptr, file, parent_is_root);
				ptr = ptr->next_sibling;
			}
		}
	}

}

AssetID export_asset_scene(AssetImporter* importer, AssetImport_Scene* scene, char* filename, int filename_str_len) {
	AssetID id;
	id.id = next_asset_id(importer->tracker);
	id.type = AssetType_Scene;



	char file_str[256];
	uint32_t str_size = filename_str_len + ASSET_FILE_EXTENSION_LENGTH;
	snprintf(file_str, str_size, "%s%s", filename, ASSET_FILE_EXTENSION);

	track_asset(importer->tracker, id, file_str, str_size);



	



	FILE* file;
	errno_t err;

	err = fopen_s(&file, file_str, "wb");  // write binary

	if (err == 0) {
		debug_print("Writing to %s,", file_str);
	} else {
		assert_fail();
		debug_print("Fail writing to %s,", file_str);
	}
	//void* buffer[256];

	// Write asset id
	fwrite(cast(const void*) &id.id, sizeof(id.id), 1, file);

	// Write type
	AssetType type = AssetType_Scene;
	fwrite(cast(const void*) &type, sizeof(type), 1, file);

	

	fwrite(cast(const void*) &scene->node_count, sizeof(scene->node_count), 1, file);
	fwrite(cast(const void*) &scene->mesh_count, sizeof(scene->mesh_count), 1, file);
	fwrite(cast(const void*) &scene->material_count, sizeof(scene->material_count), 1, file);
	fwrite(cast(const void*) &scene->light_count, sizeof(scene->light_count), 1, file);
	fwrite(cast(const void*) &scene->camera_count, sizeof(scene->camera_count), 1, file);
	fwrite(cast(const void*) &scene->anim_count, sizeof(scene->anim_count), 1, file);
	fwrite(cast(const void*) &scene->texture_count, sizeof(scene->texture_count), 1, file);


	// Write asset ids to file

	for (int i = 0; i < scene->mesh_count; i++) {
		AssetID asset = scene->mesh_infos[i];
		fwrite(cast(const void*) &asset.id, sizeof(asset.id), 1, file);
	}

	for (int i = 0; i < scene->material_count; i++) {
		AssetID asset = scene->material_infos[i];
		fwrite(cast(const void*) &asset.id, sizeof(asset.id), 1, file);
	}

	for (int i = 0; i < scene->light_count; i++) {
		AssetID asset = scene->light_infos[i];
		fwrite(cast(const void*) &asset.id, sizeof(asset.id), 1, file);
	}

	for (int i = 0; i < scene->camera_count; i++) {
		AssetID asset = scene->camera_infos[i];
		fwrite(cast(const void*) &asset.id, sizeof(asset.id), 1, file);
	}

	for (int i = 0; i < scene->anim_count; i++) {
		AssetID asset = scene->animation_infos[i];
		fwrite(cast(const void*) &asset.id, sizeof(asset.id), 1, file);
	}

	for (int i = 0; i < scene->texture_count; i++) {
		AssetID asset = scene->texture_infos[i];
		fwrite(cast(const void*) &asset.id, sizeof(asset.id), 1, file);
	}

	
	write_scene_node(importer, scene->root, file, false);
	


	err = fclose(file);
	if (err == 0) {
		debug_print("Finished writing to %s\n", file_str);
	} else {
		assert_fail();
		debug_print("Cannot close to %s\n", file_str);
	}





	return id;
}


static bool is_null_node(void* buffer) {
	assert(buffer != NULL);
	char* c = (char*)buffer;
	return c[0] == '\0' &&
		c[1] == '\0' &&
		c[2] == '\0' &&
		c[3] == '\0' &&
		c[4] == '\0' &&
		c[5] == '\0' &&
		c[6] == '\0' &&
		c[7] == '\0' &&
		c[8] == '\0' &&
		c[9] == '\0' &&
		c[10] == '\0' &&
		c[11] == '\0' &&
		c[12] == '\0';
}

static inline int fbx_convert_type_array_char_to_size(char type) {
	switch (type) {
		case 'd': {return sizeof(double);}
		case 'f': {return sizeof(float);}
		case 'l': {return sizeof(int64_t);}
		case 'i': {return sizeof(int32_t);}
		case 'b': {return 1;}
		default:  {assert_fail(); return 0; }
	}
}


static void fbx_process_objects_node(AssetImporter* importer, FBX_Node* node, FBX_ImportData* fbx_import) {
	FBX_Node* node_to_process = node;

	FBX_Node* obj_node = node_to_process->first_child;

	
	int obj_count = node_to_process->child_count - 1;
	// Loop over all child nodes of the object node
	int geo_index = 0;
	int obj_index = 0;
	while (obj_node != NULL) {

		// Hit a geometry node
		if (strcmp(obj_node->name, "Geometry") == 0) {

			assert(obj_node->properties[0].type_code == 'L');
			uint64_t id = obj_node->properties[0].primative.L_data;

			FBX_Object object;
			object.type = FBX_Object_Type_Geometry;
			object.geo = (FBX_Geometry_Object*)stack_alloc(&importer->stack, sizeof(FBX_Geometry_Object), 4);

			
			map_put(&fbx_import->fbx_object_map, id, object);


			object.geo->vertex_count = 0;
			object.geo->index_count = 0;
			object.geo->normal_count = 0;
			object.geo->uv_count = 0;
			object.geo->uv_index_count = 0;
			object.geo->pos = NULL;
			object.geo->normal = NULL;
			object.geo->texcoords = NULL;
			object.geo->indices = NULL;
			object.geo->uv_indices = NULL;





			FBX_Node* geo_node = obj_node->first_child;
			// Loop over child nodes of the geometry node
			while (geo_node != NULL) {

				if (strcmp(geo_node->name, "Properties60") == 0) {
					FBX_Node* property_node = geo_node->first_child;
					assert_fail();

				} else if (strcmp(geo_node->name, "Properties70") == 0) {
					FBX_Node* property_node = geo_node->first_child;

					while (property_node != NULL) {
						const int num_properties = property_node->num_properties;

						for (int i = 0; i < num_properties; i++) {
							FBX_Property* p = &property_node->properties[i];
							switch (p->type_code) {
								case 'S': {
									//p->special.str_data
									break;
								}

							}

						}

						property_node = property_node->next_sibling;
					}


					// Parse properties
				} else if ((strcmp(geo_node->name, "Vertices") == 0)) {
					// parse vertices
					double* vertices = geo_node->properties->array.d_data;
					const int vertex_count = geo_node->properties->array.array_length / 3;
					object.geo->pos = NULL;
					object.geo->vertex_count = vertex_count;
					stb_sb_add(object.geo->pos, vertex_count);

					for (int i = 0; i < vertex_count; i++) {
						int base_vertex_index = 3 * i;
						object.geo->pos[i] = Vec3f(vertices[base_vertex_index], vertices[base_vertex_index + 1], vertices[base_vertex_index + 2]);
					}

				} else if ((strcmp(geo_node->name, "PolygonVertexIndex") == 0)) {
					// parse indices
					int32_t* indices = geo_node->properties->array.i_data;
					const int array_count = geo_node->properties->array.array_length;


					// Since we may have indices that are more than 3,
					// example quads, we cant know for sure what the size of the indices is before hand
					// So we'll make a guess. this will allow us to predefine the size of the array,
					// and it'll grow if there are more indices then estimated
					const int estimated_index_count = array_count / 3;
					int current_index_count = 0;
					// Pre size the array
					object.geo->indices = NULL;

					// TODO: performance of prefilling
					//sb_add(static_mesh.indices, estimated_index_count);

					// buffer of polygon indices
					int polygon[16] = { 0 };
					// how many indices in this polygon do we have 3 = tri, 4 = quad, etc...
					int polygon_indices_count = 0;

					for (int i = 0; i < array_count; i++) {
						bool has_seen_last_index_in_polygon = false;
						int polygon_index = 0;
						polygon_indices_count = 0;
						while (!has_seen_last_index_in_polygon) {
							polygon_indices_count++;
							int index = indices[i];
							if (index < 0) {
								// indices that are negative mean that this is the last index in the polygon
								// to convert to the actual index value we need to 
								// negate and subtract 1 or faster is to bitwise negate
								index = ~index;
								has_seen_last_index_in_polygon = true;
							} else {
								i++;
							}

							if (polygon_index < 16) {
								polygon[polygon_index] = index;
							}


							polygon_index++;
						}
						switch (polygon_indices_count) {

							// TODO: since we will have multiple models per fbx file, our static mesh wont cut it anymore
							// it may know contain multiple meshes

							case 3: {
								// Simple tri, just copy

								sb_push(object.geo->indices, Vec3i(polygon[0], polygon[1], polygon[2]));

								current_index_count++;
								break;
							}
							case 4: {
								// Quad to tris

								sb_push(object.geo->indices, Vec3i(polygon[0], polygon[1], polygon[2]));
								sb_push(object.geo->indices, Vec3i(polygon[2], polygon[3], polygon[0]));
								current_index_count += 2;

								break;
							}

						}
						// TODO Convert polygon to tri mesh
						//assert(polygon_indices_count <= 4);


					}
					object.geo->index_count = current_index_count;

				} else if ((strcmp(geo_node->name, "Edges") == 0)) {
					// edges
				} else if ((strcmp(geo_node->name, "LayerElementNormal") == 0)) {
					// parse normals
					FBX_Node* normal_node_children = geo_node->first_child;
					while (normal_node_children != NULL) {

						if (strcmp(normal_node_children->name, "Normals") == 0) {
							// Parse normals

							double* normals = normal_node_children->properties->array.d_data;
							const int normal_count = normal_node_children->properties->array.array_length / 3;
							object.geo->normal = NULL;
							object.geo->normal_count = normal_count;

							stb_sb_add(object.geo->normal, normal_count);

							for (int i = 0; i < normal_count; i++) {
								int base_normal_index = 3 * i;
								object.geo->normal[i] = Vec3f(normals[base_normal_index], normals[base_normal_index + 1], normals[base_normal_index + 2]);


							}
						}

						normal_node_children = normal_node_children->next_sibling;

					}
				} else if ((strcmp(geo_node->name, "LayerElementMaterial") == 0)) {
					// materials
				}

				else if ((strcmp(geo_node->name, "LayerElementUV") == 0)) {
					// uvs

					FBX_Node* uv_node_child = geo_node->first_child;
					while (uv_node_child != NULL) {

						if (strcmp(uv_node_child->name, "UV") == 0) {
							// Parse normals

							double* uvs = uv_node_child->properties->array.d_data;
							const int uvs_count = uv_node_child->properties->array.array_length / 2;
							object.geo->uv_count = uvs_count;
							object.geo->texcoords = NULL;


							stb_sb_add(object.geo->texcoords, uvs_count);

							for (int i = 0; i < uvs_count; i++) {
								int base_uv_index = 2 * i;

								object.geo->texcoords[i] = Vec2f(uvs[base_uv_index], uvs[base_uv_index + 1]);


							}

						}

						if (strcmp(uv_node_child->name, "UVIndex") == 0) {
							// Parse normals

							int32_t* uv_indices = uv_node_child->properties->array.i_data;
							const int uv_indices_count = uv_node_child->properties->array.array_length / 3;
							object.geo->uv_index_count = uv_indices_count;
							object.geo->uv_indices = NULL;


							stb_sb_add(object.geo->uv_indices, uv_indices_count);

							for (int i = 0; i < uv_indices_count; i++) {
								int base_uv_index = 3 * i;

								object.geo->uv_indices[i] = Vec3i(uv_indices[base_uv_index], uv_indices[base_uv_index + 1], uv_indices[base_uv_index + 2]);
							}

						}

						uv_node_child = uv_node_child->next_sibling;
					}
				}

				else if ((strcmp(geo_node->name, "Layer") == 0)) {

				}


				geo_node = geo_node->next_sibling;
			}
			geo_index++;
		}

		else if (strcmp(obj_node->name, "Model") == 0) {

			assert(obj_node->properties[0].type_code == 'L');
			uint64_t id = obj_node->properties[0].primative.L_data;

			

			FBX_Object object;
			object.type = FBX_Object_Type_Model;
			object.model = (FBX_Model_Object*)stack_alloc(&importer->stack, sizeof(FBX_Model_Object), 4);

			object.model->name_length = obj_node->properties[1].special.length;
			object.model->name = obj_node->properties[1].special.str_data;
			//strcpy_s(object.model->name, object.model->name_length, obj_node->properties[1].special.str_data);

			// Set defaults, some times, some models dont have the data in the binary, and it assumes tha values are default if not specified
			object.model->local_translation = Vec3f(0, 0, 0);
			object.model->local_rotation = Vec3f(0, 0, 0);
			object.model->local_scaling = Vec3f(1, 1, 1);

			map_put(&fbx_import->fbx_object_map, id, object);

			FBX_Node* model_node = obj_node->first_child;

			// Loop over child nodes of the model node
			while (model_node != NULL) {

				if (strcmp(model_node->name, "Properties70") == 0) {
					FBX_Node* property_node = model_node->first_child;

					while (property_node != NULL) {
						const int num_properties = property_node->num_properties;

						for (int i = 0; i < num_properties; i++) {
							FBX_Property* p = &property_node->properties[i];
							switch (p->type_code) {
								case 'S': {

									if (strcmp(p->special.str_data, "Lcl Translation") == 0) {
										assert(num_properties > 4);
										object.model->local_translation.x = (float)property_node->properties[num_properties - 3].primative.D_data;
										object.model->local_translation.y = (float)property_node->properties[num_properties - 2].primative.D_data;
										object.model->local_translation.z = (float)property_node->properties[num_properties - 1].primative.D_data;
										i = num_properties; // jump out of loop, cant' use break because we are in switch
									} else if (strcmp(p->special.str_data, "Lcl Rotation") == 0) {
										assert(num_properties > 4);
										object.model->local_rotation.x = (float)property_node->properties[num_properties - 3].primative.D_data;
										object.model->local_rotation.y = (float)property_node->properties[num_properties - 2].primative.D_data;
										object.model->local_rotation.z = (float)property_node->properties[num_properties - 1].primative.D_data;
										i = num_properties; // jump out of loop, cant' use break because we are in switch
									} else if (strcmp(p->special.str_data, "Lcl Scaling") == 0) {
										assert(num_properties > 4);
										object.model->local_scaling.x = (float)property_node->properties[num_properties - 3].primative.D_data;
										object.model->local_scaling.y = (float)property_node->properties[num_properties - 2].primative.D_data;
										object.model->local_scaling.z = (float)property_node->properties[num_properties - 1].primative.D_data;
										i = num_properties; // jump out (float)of loop, cant' use break because we are in switch
									}
									break;
								}

							}

						}

						property_node = property_node->next_sibling;
					}
				}
				model_node = model_node->next_sibling;
			}


		}

		


		obj_index++;
		obj_node = obj_node->next_sibling;

	}
}

static void fbx_process_connections_node(AssetImporter* importer, FBX_Node* node, FBX_ImportData* fbx_import) {
	FBX_Node* connection_node = node->first_child;

	while (connection_node != NULL) {
		const int num_properties = connection_node->num_properties;

		char* connection_type = connection_node->properties[0].special.str_data;
		uint64_t child_object_id = connection_node->properties[1].primative.L_data;
		uint64_t parent_object_id = connection_node->properties[2].primative.L_data;

		if (strcmp(connection_type, "OO") == 0) {
			// object to object connection
			assert(num_properties == 3);

			assert(connection_node->properties[0].type_code == 'S');
			assert(connection_node->properties[1].type_code == 'L');
			assert(connection_node->properties[2].type_code == 'L');
			
			MapResult<FBX_Object> child_result = map_get(&fbx_import->fbx_object_map, child_object_id);
			if (!child_result.found) {
				// TODO: This means we havent parsed this object node. it could be a material
				//assert_fail();
				connection_node = connection_node->next_sibling;
				continue;
			}

			
			FBX_Object child_object = child_result.value;
				
			

			// Process parent node
			AssetImport_SceneNode* parent_scene_node;
			// Root parent
			if (parent_object_id == 0) {
				// Since our map doesnt allow for keys that are zero, we need to check before we get it in the map
				// an id with 0, means it is the root object
				if (fbx_import->export_scene.root == NULL) {
					fbx_import->export_scene.root = cast(AssetImport_SceneNode*) stack_alloc(&importer->stack, sizeof(AssetImport_SceneNode), 4);
					
					init_scene_node(fbx_import->export_scene.root, parent_object_id, "Root", 4);
					fbx_import->export_scene.node_count++;
					
					set_scene_node_transform(fbx_import->export_scene.root,
						Vec3f_Zero,
						Vec3f(1, 1, 1),
						Vec3f_Zero);
				}
				
				
				parent_scene_node = fbx_import->export_scene.root;



			} else {
				// this is a parent node, but is not the root node,
				// may be a transitive node(a node that has a child and a parent)
				// A -> B -> C, this is the B node
				MapResult<FBX_Object> parent_result = map_get(&fbx_import->fbx_object_map, parent_object_id);
				
				if (!parent_result.found) {
					assert_fail();
					continue;
				}

				

				
				MapResult<AssetImport_SceneNode*> result = map_get(&fbx_import->scene_node_cache_map, parent_object_id);
				
				// Check to see if we have already seen this node and added to our scene graph
				if (result.found) {
					// This node was already parsed and has been added to our scene graph
					parent_scene_node = result.value;
				} else {


					FBX_Object parent_object = parent_result.value;
					// At this point we have a child object and parent object
					assert(parent_object.type == FBX_Object_Type_Model);
					// There is no transform data on the geometry nodes
					// so we need to go into the parent object which is a model, in order to get the transform data.
					FBX_Model_Object* parent_model = parent_object.model;


					// This is the first time we seen this object
					// we need to create a scene node for it, and add it to our scene
					parent_scene_node = cast(AssetImport_SceneNode*) stack_alloc(&importer->stack, sizeof(AssetImport_SceneNode), 4);
					// Create the node, and add the transform
					init_scene_node(parent_scene_node, parent_object_id, parent_model->name, parent_model->name_length);
					fbx_import->export_scene.node_count++;
					set_scene_node_transform(fbx_import->export_scene.root,
						parent_model->local_translation,
						parent_model->local_scaling,
						parent_model->local_rotation);

					// Add it to our cache map, so that next time when we check, we'll know if it has been seen
					map_put(&fbx_import->scene_node_cache_map, parent_object_id, parent_scene_node);
				}
			}


			
				
			

			// Process child node
			switch (child_object.type) {
				case FBX_Object_Type_Geometry: {
					
					FBX_Geometry_Object* geo = child_object.geo;
					
					// Note we should not create a node for this geometry, we simply just attach the geometry to the parent model node
					// Geometry objects dont have any transform/name data attached to them
					// that data is attached to the parent model object
					// So we'll grab the name from the parent in the scene node

					AssetID asset_id = fbx_convert_geo2static_mesh_and_export(importer,
						geo,
						parent_scene_node->translation,
						parent_scene_node->scale,
						parent_scene_node->rotation,
						parent_scene_node->name,
						parent_scene_node->name_length);

					sb_push(fbx_import->export_scene.mesh_infos, asset_id);
					uint32_t last_index = sb_count(fbx_import->export_scene.mesh_infos) - 1;

					
					
					// Since nodes like this can be deeply linked
					sb_push(parent_scene_node->meshes, last_index);
					parent_scene_node->mesh_count++;
					fbx_import->export_scene.mesh_count++;
					
					// Free the geo mesh
					stb_sb_free(geo->pos);
					stb_sb_free(geo->indices);
					stb_sb_free(geo->normal);
					stb_sb_free(geo->texcoords);
					stb_sb_free(geo->uv_indices);



					break;
				}

				case FBX_Object_Type_Model: {
					// This is a model which may have may a geometry as a child
					// This may also be a link between a geometry and another parent model
					AssetImport_SceneNode* child_scene_node;

					MapResult<AssetImport_SceneNode*> result = map_get(&fbx_import->scene_node_cache_map, child_object_id);
					if (result.found) {
						child_scene_node = result.value;
					} else {
						child_scene_node = cast(AssetImport_SceneNode*) stack_alloc(&importer->stack, sizeof(AssetImport_SceneNode), 4);
						fbx_import->export_scene.node_count++;
						map_put(&fbx_import->scene_node_cache_map, child_object_id, child_scene_node);
					}

					add_child_to_scene_node(parent_scene_node, child_scene_node);

					init_scene_node(child_scene_node, child_object_id, child_object.model->name, child_object.model->name_length);
					set_scene_node_transform(child_scene_node,
						child_object.model->local_translation,
						child_object.model->local_scaling,
						child_object.model->local_rotation);

					

					
					break;
				}
			}

		}

		else if (strcmp(connection_type, "OP") == 0) {
			// object to property connection
			//DEBUG_BREAK;
		}

		else if (strcmp(connection_type, "PO") == 0) {
			// property to object connection
			//DEBUG_BREAK;
		}

		else if (strcmp(connection_type, "PP") == 0) {
			// property to property connection
			//DEBUG_BREAK;
		}
		

		

		connection_node = connection_node->next_sibling;
	}

	
}

static void fbx_process_node(AssetImporter* importer, FBX_Node* node, FBX_ImportData* fbx_import) {
	
	// Skip nodes that are not objects
	if (strcmp(node->name, "Objects") == 0) {
		fbx_process_objects_node(importer, node, fbx_import);
	} else if (strcmp(node->name, "Connections") == 0) {
		fbx_process_connections_node(importer, node, fbx_import);
	}
	else if (strcmp(node->name, "GlobalSettings") == 0) {
		


		FBX_Node* settings_node = node->first_child;


		while (settings_node != NULL) {

			if (strcmp(settings_node->name, "Properties70") == 0) {
				FBX_Node* property_node = settings_node->first_child;

				while (property_node != NULL) {
					const int num_properties = property_node->num_properties;

					

					for (int i = 0; i < num_properties; i++) {
						FBX_Property* p = &property_node->properties[i];
						switch (p->type_code) {
							case 'S': {

								if (strcmp(p->special.str_data, "UpAxis") == 0) {
									assert(num_properties == 5);
									fbx_import->global_settings.up_axis = property_node->properties[4].primative.I_data;
									i = num_properties; // jump out of loop, cant' use break because we are in switch
								} else if (strcmp(p->special.str_data, "UpAxisSign") == 0) {
									assert(num_properties == 5);
									fbx_import->global_settings.up_axis_sign = property_node->properties[4].primative.I_data;
									i = num_properties; // jump out of loop, cant' use break because we are in switch
								}
								else if (strcmp(p->special.str_data, "FrontAxis") == 0) {
									assert(num_properties == 5);
									fbx_import->global_settings.front_axis = property_node->properties[4].primative.I_data;
									i = num_properties; // jump out of loop, cant' use break because we are in switch
								}
								else if (strcmp(p->special.str_data, "FrontAxisSign") == 0) {
									assert(num_properties == 5);
									fbx_import->global_settings.front_axis_sign = property_node->properties[4].primative.I_data;
									i = num_properties; // jump out of loop, cant' use break because we are in switch
								}

								else if (strcmp(p->special.str_data, "CoordAxis") == 0) {
									assert(num_properties == 5);
									fbx_import->global_settings.coord_axis = property_node->properties[4].primative.I_data;
									i = num_properties; // jump out of loop, cant' use break because we are in switch
								}

								else if (strcmp(p->special.str_data, "CoordAxisSign") == 0) {
									assert(num_properties == 5);
									fbx_import->global_settings.coord_axis_sign = property_node->properties[4].primative.I_data;
									i = num_properties; // jump out of loop, cant' use break because we are in switch
								}

								else if (strcmp(p->special.str_data, "OriginalUpAxis") == 0) {
									assert(num_properties == 5);
									fbx_import->global_settings.original_up_axis = property_node->properties[4].primative.I_data;
									i = num_properties; // jump out of loop, cant' use break because we are in switch
								}

								else if (strcmp(p->special.str_data, "OriginalUpAxisSign") == 0) {
									assert(num_properties == 5);
									fbx_import->global_settings.original_up_axis_sign = property_node->properties[4].primative.I_data;
									i = num_properties; // jump out of loop, cant' use break because we are in switch
								}
								else if (strcmp(p->special.str_data, "UnitScaleFactor") == 0) {
									assert(num_properties == 5);
									fbx_import->global_settings.unit_scale_factor = property_node->properties[4].primative.D_data;
									i = num_properties; // jump out of loop, cant' use break because we are in switch
								}

								else if (strcmp(p->special.str_data, "OriginalUnitScaleFactor") == 0) {
									assert(num_properties == 5);
									fbx_import->global_settings.original_unit_scale_factor = property_node->properties[4].primative.D_data;
									i = num_properties; // jump out of loop, cant' use break because we are in switch
								}
								break;
							}

						}

					}

					property_node = property_node->next_sibling;
				}
			}
			settings_node = settings_node->next_sibling;
		}

	}
	
}


static FBX_Node fbx_parse_node(AssetImporter* importer, void* buffer, FILE* file, FBX_ImportData* fbx_import) {
	
	FBX_Node node;
	node.child_count = 0;
	node.first_child = NULL;
	node.next_sibling = NULL;

	long rollback = ftell(file);
	fread(buffer, 4, 1, file);
	node.end_offset = *cast(uint32_t*)buffer;

	if (node.end_offset == 0) {
		fseek(file, rollback, SEEK_SET);
		return node;
	}
	fread(buffer, 4, 1, file);
	node.num_properties = *cast(uint32_t*)buffer;

	fread(buffer, 4, 1, file);
	node.property_list_length = *cast(uint32_t*)buffer;

	fread(buffer, 1, 1, file);
	node.name_length = *cast(uint8_t*)buffer;

	node.name = cast(char*)stack_alloc(&importer->stack, node.name_length + 1, 1);
	fread(node.name, node.name_length, 1, file);
	node.name[node.name_length] = '\0';
	
	importer->stack_allocs_count++;

	


	//if (strcmp(node.name, "FBXHeaderExtension") == 0) {
	//	return node; // skip
	//	
	//} else if (strcmp(node.name, "FileId") == 0) {
	//	return node; // skip
	//}
	//else if (strcmp(node.name, "GlobalSettings") == 0) {
	//	return node; // skip
	//}
	//else if (strcmp(node.name, "Documents") == 0) {
	//	return node; // skip
	//}

	//else if (strcmp(node.name, "References") == 0) {
	//	return node; // skip
	//}

	//else if (strcmp(node.name, "Definitions") == 0) {
	//	return node; // skip
	//}





	int property_count = node.num_properties;
	if (property_count != 0) {

		node.properties = cast(FBX_Property*)stack_alloc(&importer->stack, property_count * sizeof(FBX_Property), 1);
		assert(node.properties != NULL);

		importer->stack_allocs_count++;
		

		for (int i = 0; i < property_count; i++) {
			fread(buffer, 1, 1, file);

			char type = *(char*)buffer;
			node.properties[i].type_code = type;
			switch (type) {
				// Primative type
				case 'Y': {
					fread(buffer, 2, 1, file);
					int16_t value = *cast(int16_t *)buffer;
					node.properties[i].primative.Y_data = value;
					break;
				}
				case 'C': {
					fread(buffer, 1, 1, file);
					char value = *cast(char*)buffer;
					node.properties[i].primative.C_data = value;
					break;
				}

				case 'I': {
					fread(buffer, 4, 1, file);
					int32_t value = *cast(int32_t*)buffer;
					node.properties[i].primative.I_data = value;
					break;
				}

				case 'F': {
					fread(buffer, 4, 1, file);
					float value = *cast(float*)buffer;
					node.properties[i].primative.F_data = value;
					break;
				}

				case 'D': {
					fread(buffer, 8, 1, file);
					double value = *cast(double*)buffer;
					node.properties[i].primative.D_data = value;
					break;
				}
				case 'L': {
					fread(buffer, 8, 1, file);
					int64_t value = *cast(int64_t*)buffer;
					node.properties[i].primative.L_data = value;
					break;
				}
						  // Array types
				case 'd': case 'f': case 'l': case 'i': case 'b': {
					uint32_t type_size = fbx_convert_type_array_char_to_size(type);

					fread(buffer, 4, 1, file);
					node.properties[i].array.array_length = *cast(uint32_t*)buffer;

					fread(buffer, 4, 1, file);
					uint32_t encoding = *cast(uint32_t*)buffer;
					assert(encoding == 1 || encoding == 0);
					node.properties[i].array.encoding = encoding;


					fread(buffer, 4, 1, file);
					node.properties[i].array.compressed_length = *cast(uint32_t*)buffer;

					uint32_t array_size_in_bytes = node.properties[i].array.array_length * type_size;
					switch (encoding) {
						case 0: {

							node.properties[i].array.v_data = stack_alloc(&importer->stack, array_size_in_bytes, 1);
							fread(node.properties[i].array.v_data, array_size_in_bytes, 1, file);
							importer->stack_allocs_count++;
							break;
						}
						case 1: {
							uint32_t compressed_size_in_bytes = node.properties[i].array.compressed_length;
							z_stream infstream;
							infstream.zalloc = Z_NULL;
							infstream.zfree = Z_NULL;
							infstream.opaque = Z_NULL;

							// Prepare the data array with the expected size
							node.properties[i].array.v_data = stack_alloc(&importer->stack, array_size_in_bytes, 1);
							importer->stack_allocs_count++;

							// Alloc some data to be inflated into
							// once we inflated, we'll pop this off the stack
							void* inflate_buffer = stack_alloc(&importer->stack, compressed_size_in_bytes, 1);

							fread(inflate_buffer, compressed_size_in_bytes, 1, file);
							infstream.avail_in = (uInt)compressed_size_in_bytes;
							infstream.next_in = (Bytef *)inflate_buffer; // input char array
							infstream.avail_out = (uInt)array_size_in_bytes; // size of output
							infstream.next_out = (Bytef *)node.properties[i].array.v_data; // output char array


							inflateInit(&infstream);
							inflate(&infstream, Z_NO_FLUSH);
							inflateEnd(&infstream);
							
							stack_pop(&importer->stack);

							break;

						}
					}
					break;
				}


						  // Special types
				case 'R': {
					fread(buffer, 4, 1, file);
					uint32_t length = *cast(uint32_t*)buffer;

					node.properties[i].special.length = length;
					node.properties[i].special.raw_binary_data = stack_alloc(&importer->stack, length + 1, 1);
					fread(cast(char*)node.properties[i].special.raw_binary_data, length, 1, file);
					importer->stack_allocs_count++;
					break;
				}

				case 'S': {
					fread(buffer, 4, 1, file);
					uint32_t length = *cast(uint32_t*)buffer;
					node.properties[i].special.length = length;
					node.properties[i].special.str_data = cast(char*)stack_alloc(&importer->stack, length + 1, 1);
					fread(node.properties[i].special.str_data, length, 1, file);
					node.properties[i].special.str_data[length] = '\0';
					char* debug_str = node.properties[i].special.str_data;
					importer->stack_allocs_count++;
					break;
				}
				default: {
					assert_fail();
					break;
				}


			}


		}

	} else {

	}

	long current_offset = ftell(file);

	
	
	

	while (current_offset < node.end_offset - 13) {
		FBX_Node temp = fbx_parse_node(importer, buffer, file, fbx_import);
		FBX_Node* child_node = cast(FBX_Node*) stack_alloc(&importer->stack, sizeof(temp), 1);
		*child_node = temp;
		importer->stack_allocs_count++;
		

		if (node.child_count == 0) {
			node.first_child = child_node;
		} else {
			FBX_Node* child = node.first_child;
			while (child->next_sibling != NULL) {
				child = child->next_sibling;
			}
			child->next_sibling = child_node;
		}
		node.child_count++;

		current_offset = ftell(file);
	}
	
	// Pass the null record
	rollback = ftell(file);
	fread(buffer, 13, 1, file);
	if (!is_null_node(buffer)) {
		// Roll back
		fseek(file, rollback, SEEK_SET);
	}
	
	



	return node;
}



AssetID import_fbx(AssetImporter* importer, char* filename, bool y_is_up) {
	
	AssetID scene_id;
	scene_id.id = -1;// Underflows to max, this should alert to our map, that this is an error
	scene_id.type = AssetType_None;

	
	

	
	// Init internal fbx import data
	// the data inside fbx import data should be freed at the end of this function call
	FBX_ImportData fbx_import;
	
	map_init(&fbx_import.fbx_object_map);
	map_init(&fbx_import.scene_node_cache_map);
	// For now we'll default the size to 16
	map_grow(&fbx_import.scene_node_cache_map, 16);

	fbx_import.export_scene.root = NULL;
	fbx_import.export_scene.node_count = 0;

	fbx_import.export_scene.mesh_count = 0;
	fbx_import.export_scene.material_count = 0;
	fbx_import.export_scene.light_count = 0;
	fbx_import.export_scene.camera_count = 0;
	fbx_import.export_scene.anim_count = 0;
	fbx_import.export_scene.texture_count = 0;

	fbx_import.export_scene.mesh_infos = NULL;
	fbx_import.export_scene.material_infos = NULL;
	fbx_import.export_scene.light_infos = NULL;
	fbx_import.export_scene.camera_infos = NULL;
	fbx_import.export_scene.animation_infos = NULL;
	fbx_import.export_scene.texture_infos = NULL;
	


	FILE* file;
	errno_t err;

	// TODO: use memory mapped files
	// TODO: since we have a lot of small strings allocated, we can probably use string interning
	err = fopen_s(&file, filename, "rb");

	// TODO: if we are not able to load the file, our engine should handle it properly
	if (err == 0) {
		debug_print("Opening fbx %s,", filename);
	} else {
		debug_print("Cannot open fbx%s\n", filename);
		goto cleanup_import_data;
	}


	void* buffer[256];


	// See https://code.blender.org/2013/08/fbx-binary-file-format-specification/
	fread(buffer, 21, 1, file);
	fread(buffer, 2, 1, file);
	fread(buffer, 4, 1, file);
	unsigned int version = *(cast(unsigned int*)buffer);
	if (version < 7100) {
		debug_print("FBX Version %d not supported\n", version);
		if (file) {

			err = fclose(file);
			if (err == 0) {
				debug_print("Closed obj %s\n", filename);
			} else {
				debug_print("Cannot close obj %s\n", filename);
			}
		}
		goto cleanup_import_data;
	}



	


	
	

	
	
	



	for (;;) {
		//stack_set_reset_point(&importer->stack);
		FBX_Node node = fbx_parse_node(importer, buffer, file, &fbx_import);
		if (node.end_offset == 0) break;
		fbx_process_node(importer, &node, &fbx_import);
		//stack_pop_to_last_reset_point(&importer->stack);
		fseek(file, node.end_offset, SEEK_SET);
	}

	

	// Instead of making a copy, just find the neccessary length to cut of the extension
	//char* stripped_filename = filename;
	//// TODO: implement something more generic
	//// right now it is doing -4 to remove the .fbx extension
	//uint32_t stripped_filename_length = strlen(filename) - 4;

	char* stripped_filename = "Sink";
	uint32_t stripped_filename_length = 5;

	importer->global_settings.up_axis = fbx_import.global_settings.up_axis;
	importer->global_settings.up_axis_sign = fbx_import.global_settings.up_axis_sign;
	importer->global_settings.front_axis = fbx_import.global_settings.front_axis;
	importer->global_settings.front_axis_sign = fbx_import.global_settings.front_axis_sign;
	importer->global_settings.coord_axis = fbx_import.global_settings.coord_axis;
	importer->global_settings.coord_axis_sign = fbx_import.global_settings.coord_axis_sign;
	importer->global_settings.original_up_axis = fbx_import.global_settings.original_up_axis;
	importer->global_settings.original_up_axis_sign = fbx_import.global_settings.original_up_axis_sign;
	importer->global_settings.unit_scale_factor = fbx_import.global_settings.unit_scale_factor;
	importer->global_settings.original_unit_scale_factor = fbx_import.global_settings.original_unit_scale_factor;


	debug_print("\n\nUp axis [%d, %d]\nFront axis[%d, %d]\nCoord Axis[%d, %d]\nOriginal Up Axis[%d, %d]\nScale Factor[%f, %f]\n\n",
		importer->global_settings.up_axis, importer->global_settings.up_axis_sign,
		importer->global_settings.front_axis, importer->global_settings.front_axis_sign,
		importer->global_settings.coord_axis, importer->global_settings.coord_axis_sign,
		importer->global_settings.original_up_axis, importer->global_settings.original_up_axis_sign,
		importer->global_settings.unit_scale_factor, importer->global_settings.original_unit_scale_factor);
	
	scene_id = export_asset_scene(importer, &fbx_import.export_scene, stripped_filename, stripped_filename_length);
	


	
// Clean up memory and any import data
cleanup_import_data:

	if (file) {

		err = fclose(file);
		if (err == 0) {
			debug_print("Closed obj %s\n", filename);
		} else {
			debug_print("Cannot close obj %s\n", filename);
		}
	}

	// We need to free any data from all the nodes
	for (int i = 0; i < fbx_import.scene_node_cache_map.size; i++) {
		CompactMapItem<AssetImport_SceneNode*> node = fbx_import.scene_node_cache_map.map[i];
		if (node.key != 0) {
			sb_free(node.value->meshes);
		}
	}
	


	map_destroy(&fbx_import.fbx_object_map);
	map_destroy(&fbx_import.scene_node_cache_map);

	sb_free(fbx_import.export_scene.mesh_infos);
	sb_free(fbx_import.export_scene.material_infos);
	sb_free(fbx_import.export_scene.light_infos);
	sb_free(fbx_import.export_scene.camera_infos);
	sb_free(fbx_import.export_scene.animation_infos);
	sb_free(fbx_import.export_scene.texture_infos);

	stack_reset(&importer->stack);


	return scene_id;

}


