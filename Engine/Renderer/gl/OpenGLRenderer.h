#pragma once




#include <stdbool.h>
#include <SDL.h>

#include "../../Math/Vec.h"
#include "../../Math/Mat.h"




#include "../../Common/StackAllocator.h"

#include "../../Core/Scene.h"
#include "../../Core/StaticMesh.h"

#include "GLShader.h"

#define GLEW_STATIC
#include "glew.h"
#include  "SDL_opengl.h"


#define DEBUG_GRID_SIZE 100

#define SHADOW_WIDTH_RES 1024
#define SHADOW_HEIGHT_RES 1024





typedef struct OpenGLRenderer {

	Scene* render_scene;



	SDL_GLContext gl_context;
	SDL_Window* sdl_window;
	

	void* renderer_memory;
	size_t renderer_memory_size;
	StackAllocator stack_allocator;



	// Shadow map
	GLShader shadowmap_shader;
	Mat4x4f light_space_mat;
	int shadow_width_res;
	int shadow_height_res;
	GLuint shadow_fbo;
	GLuint shadow_map;

	// IBL cubemaps
	GLuint env_cubemap;
	GLuint hdr_skymap_id;
	GLuint irradiance_map_id;
	GLuint prefilter_map;
	GLuint brdf_lut;

	// IBL shaders
	
	GLShader equi_rect_shader;
	GLShader skybox_shader;
	GLShader irradiance_conv_shader;
	GLShader prefilter_shader;
	GLShader brdf_shader;

	// IBL Buffers
	GLuint capture_FBO;
	GLuint capture_RBO;
	GLuint skybox_VAO;
	GLuint skybox_VBO;
	GLuint skybox_EBO;
	

	GLuint albedo_map_id;
	GLuint normal_map_id;
	GLuint metallic_map_id;
	GLuint roughness_map_id;
	GLuint ao_map_id;
	
	GLShader main_shader;
	
	GLuint VAO;
	GLuint VBO; // id to the Buffer that stores our vertices
	GLuint EBO; // id to the Buffer that stores our indices

	

	// Debug
	GLShader debug_shader;
	StaticMesh grid_mesh;
	int axes_pos_offset;
	bool show_debug_grid;
	bool show_debug_axes;
	bool draw_lines;

	GLuint grid_debug_VAO;
	GLuint grid_debug_VBO;
} OpenGLRenderer;


void set_scene_for_opengl_renderer(OpenGLRenderer* opengl, Scene* scene);
static void load_shaders(OpenGLRenderer* opengl);

bool init_opengl_renderer(SDL_Window* window, OpenGLRenderer* opengl, void* parition_start, size_t partition_size);
bool destroy_opengl_renderer(OpenGLRenderer* opengl);





void init_gl_debug(OpenGLRenderer* opengl);
void destroy_gl_debug(OpenGLRenderer* opengl);

static void opengl_render_scene(OpenGLRenderer* opengl, Vec2i viewport_size, bool light_pass);
void opengl_render(OpenGLRenderer* opengl, Vec2i viewport_size, bool render_debug);
void opengl_debug_render(OpenGLRenderer* opengl, Vec2i viewport_size);
void opengl_swap_buffer(OpenGLRenderer* opengl);