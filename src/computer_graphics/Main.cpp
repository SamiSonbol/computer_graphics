#include <iostream>
#include <vector>

#include "computer_graphics/Math.h"
#include "computer_graphics/Shader.h"
#include "computer_graphics/Mesh.h"
#include "computer_graphics/UI.h"

int main() {

	vec2 screen_size; 
	GLFWwindow* window = INIT_GLAD_GLFW_WINDOW(screen_size, vec3(0.478f, 0.647f, 0.702f));
	
	std::string normal_mapping_and_displacement_mapping_and_geometry = SHADERS_DIR"/normal_mapping_and_displacement_mapping_and_geometry";
	std::string normal_mapping_and_displacement_mapping_and_tesselation = SHADERS_DIR"/normal_mapping_and_displacement_mapping_and_tesselation";
	Shader shader(normal_mapping_and_displacement_mapping_and_tesselation);
	glUseProgram(shader.program);
	Shader::graphics_vectors_container vectors_container = shader.create_standard_shader_vectors();
	Shader::graphics_booleans_container booleans_container = shader.create_standard_shader_booleans();
	Shader::graphics_floats_container floats_container = shader.create_standard_shader_floats();
	shader.initialize(screen_size, vectors_container, booleans_container, floats_container);

	UI user_interface(window, (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	Mouse mouse = Mouse(window, RESOURCES_DIR"/cursor_textures/test.png");
	bool plot = false;
	user_interface.shader_debug_mode(vectors_container, booleans_container, floats_container, plot);

	Mesh mesh = Mesh::from_obj_folder(RESOURCES_DIR"/3D models/cottage.obj", RESOURCES_DIR"/texture_maps/default");

	/*since each object has its own own data buffers and textures inside *Mesh*, we dont need multiple vertex arrays, instead we simply bind said buffers to the shader every time we want to draw said object
	we do that by using the *Shader::draw_mesh_elements* function, where this function binds the *Mesh* data and draws it directly*/
	unsigned int vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader.program);

		shader.update(vectors_container, booleans_container, floats_container);
		user_interface.new_frame();
		mouse.update(window);
		mouse.plot_point(plot, window, vectors_container, screen_size, mesh, mesh.colors, mesh.positions);
		

		shader.bind_and_draw_mesh_elements(mesh, booleans_container.gamma_correction, GL_PATCHES);

		user_interface.update();
		user_interface.render();

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