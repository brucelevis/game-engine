#pragma once

#ifndef GLEW_STATIC
	#define GLEW_STATIC
#endif // !GLEW_STATIC
#include "glew.h"
#include <stdio.h>
#include  "SDL_opengl.h"

typedef struct GLShader {
	GLuint program;
	GLuint vertex_shader;
	GLuint fragment_shader;
} GLShader;


void inline load_gl_shader(GLShader* shader, const char* vertex_code, const char** fragment_code) {

	shader->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader->vertex_shader, 1, vertex_code, NULL);
	glCompileShader(shader->vertex_shader);


	GLint success;
	GLchar infoLog[512];

	// Print compile errors if any
	glGetShaderiv(shader->vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader->vertex_shader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED, %s\n", infoLog);
	};


	shader->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader->fragment_shader, 1, fragment_code, NULL);
	glCompileShader(shader->fragment_shader);
	// Print compile errors if any
	glGetShaderiv(shader->fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader->fragment_shader, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED, %s\n", infoLog);
	};

	// Shader Program
	shader->program = glCreateProgram();
	glAttachShader(shader->program, shader->vertex_shader);
	glAttachShader(shader->program, shader->fragment_shader);
	glLinkProgram(shader->program);
	// Print linking errors if any

	glGetProgramiv(shader->program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader->program, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED, %s\n", infoLog);
	}

	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(shader->vertex_shader);
	glDeleteShader(shader->fragment_shader);
}



void inline delete_gl_program(GLShader* shader) {
	glUseProgram(0);
	glDeleteProgram(shader->program);
}