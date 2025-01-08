#include <iostream>
#include <vector>
#include "Math.h"
#include "Shader.h"
#include "Mesh.h"
#include <stb/stb_image.h>

void checkOpenGLError(const std::string& operation) {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cerr << "OpenGL error after " << operation << ": " << err << std::endl;
	}
};

int main() {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
	if (!primary_monitor) {

		std::cerr << "Failed to get primary monitor\n";
		glfwTerminate();
		return -1;

	};

	const GLFWvidmode* video_mode = glfwGetVideoMode(primary_monitor);
	if (!video_mode) {

		std::cerr << "Failed to get video mode\n";
		glfwTerminate();
		return -1;

	};
	 
	vec2 screen_size (video_mode->width, video_mode->height);
	GLFWwindow* window = glfwCreateWindow(screen_size.x, screen_size.y, "TEST", NULL, NULL);
	if (window == NULL) {

		std::cout << "failed";
		glfwTerminate();
		return -1;

	};
	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, screen_size.x, screen_size.y);
	glEnable(GL_DEPTH_TEST);

	Mouse mouse = Mouse(window);
	//////

	Texture river_pebbles = Texture("test_texture.png", "uTexture", GL_TEXTURE0, 0);
	Texture river_pebbles_normal_map = Texture("test_texture.png", "uNormal_map", GL_TEXTURE1, 1);
	Mesh mesh = Mesh(river_pebbles);

	std::vector<float> tangents;
	std::vector<float> bitangents;

	vec4 material_properties = vec4(0.3, 0.7, 0.1, 10);

	unsigned int vertex_array, positions_buffer, normals_buffer, colors_buffer, indices_buffer, texture_coordinates_buffer, tangents_buffer, bitangents_buffer;

	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	
	Shader shader = Shader(vertex_shader, geometry_shader, fragment_shader);
	glUseProgram(shader.program);

	std::vector<vec3> matrix_vectors = shader.create_standard_matrix_vectors();
	std::vector<vec3> light_vectors = shader.create_standard_light_vectors();
	std::vector<Texture> textures = { river_pebbles, river_pebbles_normal_map};

	bind_buffer(GL_ARRAY_BUFFER, &positions_buffer, mesh.positions, GL_STATIC_DRAW, 0, 3, true);
	bind_buffer(GL_ARRAY_BUFFER, &normals_buffer, mesh.normals, GL_STATIC_DRAW, 1, 3, true);
	bind_buffer(GL_ARRAY_BUFFER, &colors_buffer, mesh.colors, GL_STATIC_DRAW, 2, 3, true);
	bind_buffer(GL_ELEMENT_ARRAY_BUFFER, &indices_buffer, mesh.indices, GL_STATIC_DRAW, -1, 1, true);
	bind_buffer(GL_ARRAY_BUFFER, &texture_coordinates_buffer, mesh.texture_coordinates, GL_STATIC_DRAW, 3, 2, true);
	bind_buffer(GL_ARRAY_BUFFER, &tangents_buffer, tangents, GL_STATIC_DRAW, 4, 3, true);
	bind_buffer(GL_ARRAY_BUFFER, &bitangents_buffer, bitangents, GL_STATIC_DRAW, 5, 3, true);

	shader.setup(screen_size, matrix_vectors, light_vectors, material_properties, textures);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader.program);
		glBindVertexArray(vertex_array);

		mouse.move(matrix_vectors, light_vectors, material_properties);
		shader.update(matrix_vectors, light_vectors, material_properties);

		//glDrawArrays(GL_TRIANGLES, 0, mesh.positions.size() / 3);
		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_SHORT, 0);
		//checkOpenGLError("glDrawElements");

		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();

	};

	glDeleteVertexArrays(1, &vertex_array);
	glDeleteBuffers(1, &positions_buffer);
	shader.delete_textures();
	glDeleteProgram(shader.program);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;

};