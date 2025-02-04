#include <iostream>
#include <vector>

#include "computer_graphics/Math.h"
#include "computer_graphics/Shader.h"
#include "computer_graphics/Mesh.h"
#include "computer_graphics/UI.h"

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
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
	GLFWwindow* window = glfwCreateWindow(screen_size.x, screen_size.y, PROJECT_NAME, NULL, NULL);
	if (window == NULL) {

		std::cout << "failed";
		glfwTerminate();
		return -1;

	};
	glfwMakeContextCurrent(window);
	
	gladLoadGL();
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	glViewport(0, 0, screen_size.x, screen_size.y);
	glEnable(GL_DEPTH_TEST);

	
	UI user_interface(window, (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	/*Texture river_pebbles(RESOURCES_DIR"/diffuse_maps/abra.png", "uTexture", GL_TEXTURE0, 0);
	Texture river_pebbles_normal(RESOURCES_DIR"/normal_maps/abra_normal.png", "uNormal_map", GL_TEXTURE1, 1);
	Texture river_pebbles_displacement(RESOURCES_DIR"/displacement_maps/abra_displacement.png", "uDisplacement_map", GL_TEXTURE2, 2);*/
	
	/*Texture river_pebbles(RESOURCES_DIR"/diffuse_maps/emma.png", "uTexture", GL_TEXTURE0, 0);
	Texture river_pebbles_normal(RESOURCES_DIR"/normal_maps/emma_normal.png", "uNormal_map", GL_TEXTURE1, 1);
	Texture river_pebbles_displacement(RESOURCES_DIR"/displacement_maps/emma_displacement.png", "uDisplacement_map", GL_TEXTURE2, 2);*/

	/*Texture river_pebbles(RESOURCES_DIR"/diffuse_maps/osna.png", "uTexture", GL_TEXTURE0, 0);
	Texture river_pebbles_normal(RESOURCES_DIR"/normal_maps/osna_normal.png", "uNormal_map", GL_TEXTURE1, 1);
	Texture river_pebbles_displacement(RESOURCES_DIR"/displacement_maps/osna_displacement.png", "uDisplacement_map", GL_TEXTURE2, 2);*/

	Texture river_pebbles(RESOURCES_DIR"/diffuse_maps/mountain_range.png", "uTexture", GL_TEXTURE0, 0);
	Texture river_pebbles_normal(RESOURCES_DIR"/normal_maps/osna_normal.png", "uNormal_map", GL_TEXTURE1, 1);
	Texture river_pebbles_displacement(RESOURCES_DIR"/displacement_maps/mountain_range_displacement.png", "uDisplacement_map", GL_TEXTURE2, 2);

	/*Texture river_pebbles(RESOURCES_DIR"/diffuse_maps/rocky_rivers.png", "uTexture", GL_TEXTURE0, 0);
	Texture river_pebbles_normal(RESOURCES_DIR"/normal_maps/osna_normal.png", "uNormal_map", GL_TEXTURE1, 1);
	Texture river_pebbles_displacement(RESOURCES_DIR"/displacement_maps/rocky_rivers_displacement.png", "uDisplacement_map", GL_TEXTURE2, 2);*/

	/*Texture river_pebbles(RESOURCES_DIR"/diffuse_maps/ganges_river.png", "uTexture", GL_TEXTURE0, 0);
	Texture river_pebbles_normal(RESOURCES_DIR"/normal_maps/ganges_river_normal.png", "uNormal_map", GL_TEXTURE1, 1);
	Texture river_pebbles_displacement(RESOURCES_DIR"/displacement_maps/ganges_river_displacement.png", "uDisplacement_map", GL_TEXTURE2, 2);*/
	Mesh mesh(river_pebbles, vec2(200, 200));

	unsigned int vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	
	//std::vector<unsigned int> compile_info = { compile_shader(GL_VERTEX_SHADER, vertex_shader), compile_shader(GL_GEOMETRY_SHADER, geometry_shader), compile_shader(GL_FRAGMENT_SHADER, fragment_shader) };
	std::vector<unsigned int> compile_info = { compile_shader(GL_VERTEX_SHADER, vertex_shader), compile_shader(GL_TESS_CONTROL_SHADER, tesselation_control_shader), compile_shader(GL_TESS_EVALUATION_SHADER, tesselation_evaluation_shader), compile_shader(GL_FRAGMENT_SHADER, fragment_shader) };
	Shader shader(compile_info);
	glUseProgram(shader.program);

	Shader::graphics_vectors_container vectors_container = shader.create_standard_shader_vectors();
	Shader::graphics_booleans_container booleans_container = shader.create_standard_shader_booleans();
	Shader::graphics_floats_container floats_container = shader.create_standard_shader_floats();

	std::vector<Texture> textures = { river_pebbles, river_pebbles_normal, river_pebbles_displacement };
	shader.bind_mesh_buffers(mesh);
	shader.initialize(screen_size, vectors_container, booleans_container, floats_container, textures);
	user_interface.shader_debug_mode(vectors_container, booleans_container, floats_container);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glPatchParameteri(GL_PATCH_VERTICES, 3);
	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader.program);
		glBindVertexArray(vertex_array);

		shader.update(vectors_container, booleans_container, floats_container);
		user_interface.new_frame();

		//glDrawArrays(GL_TRIANGLES, 0, mesh.positions.size());
		//glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_SHORT, 0);
		glDrawElements(GL_PATCHES, mesh.indices.size(), GL_UNSIGNED_SHORT, 0);

		user_interface.update();
		user_interface.render();

		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();

	};

	glDeleteVertexArrays(1, &vertex_array);
	shader.delete_all();
	user_interface.destroy();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;

};