#pragma once

#include <stdbool.h>


#include <SDL.h>
#include "../Math/Mat.h" // TODO: figure out better way to do this. dont want ../.. stuff
#include "../Math/Vec.h"
#include "../ObjFile.h"
#include "../TextureData.h"

#include "../Core/Camera.h"


typedef struct Shader {
	ObjModel* model;// TODO: change this to vbo style. or use a static mesh
	SurfaceData* texture;
	Mat4x4f* transform;
	Vec4f pos[3];
	Vec2f uv[3];
	Vec3f normals[3];
} Shader;





typedef struct SoftwareRenderer {
	SDL_Renderer* renderer;
	SDL_Surface* surface;
	SDL_Window* sdl_window;
	Vec2i window_size;
	float* zbuffer;
	int zbuffer_size;
	Camera camera;
	Shader shader;

	// TODO: find a better way to store these things
	ObjModel model;
	SurfaceData texture;

} SoftwareRenderer;




static Vec4f vertex_shader(Shader* shader, int face_id, int vertex_id);
static bool fragment_shader(Shader* shader, Vec3f bary, Vec4f frag_coord, Vec4f* output_color);

bool init_software_renderer(SDL_Window* window, SoftwareRenderer* renderer, Vec2i size);
static bool init_z_buffer(SoftwareRenderer* renderer);
static void clear_z_buffer(SoftwareRenderer* renderer);
static void init_shader(SoftwareRenderer* renderer);
bool destroy_software_renderer(SoftwareRenderer* renderer);
void software_render(SoftwareRenderer* r);

void software_debug_render(SoftwareRenderer* r);


