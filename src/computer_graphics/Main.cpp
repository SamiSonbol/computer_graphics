#include <iostream>
#include <vector>

#include "computer_graphics/Math.h"
#include "computer_graphics/Shader.h"
#include "computer_graphics/Mesh.h"
#include "computer_graphics/UI.h"
#include "computer_graphics/Point_Cloud.h"

int main() {

	vec2 screen_size;
	GLFWwindow* window = INIT_GLAD_GLFW_WINDOW(screen_size, vec3(0.478f, 0.647f, 0.702f));

	std::string POINTS_SHADER = SHADERS_DIR"/normal_mapping_and_displacement_mapping_and_geometry_POINTS";
	std::string TRIANGLE_PATCHES_SHADER = SHADERS_DIR"/normal_mapping_and_displacement_mapping_and_tesselation_TRIANGLE_PATCHES";
	Shader shader(POINTS_SHADER);
	glUseProgram(shader.program);
	shader.default_uniforms_maps_initialization(screen_size);

	UI user_interface(window, (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	Mouse mouse = Mouse(window, RESOURCES_DIR"/cursor_textures/test.png");
	bool plot = false;
	user_interface.shader_debug_mode(shader, plot);

	//Mesh mesh = Mesh::from_procedural_folder(vec2(300, 300), RESOURCES_DIR"/texture_maps/mountain_range", Mesh::ADD_ALL_VERTICES);
	//Mesh mesh = Mesh::from_OBJ_folder(RESOURCES_DIR"/3D models/cat.obj", RESOURCES_DIR"/texture_maps/cat", Mesh::ADD_ALL_VERTICES);
	Mesh mesh = Mesh::from_LAS_folder(RESOURCES_DIR"/LAS files/Manawa.las", RESOURCES_DIR"/texture_maps/default", Mesh::ADD_ALL_VERTICES);
	std::pair<vec3, vec3> bounds = get_min_max(mesh.positions);
	shader.create_uniform_float(bounds.first.z, "min_height");
	shader.create_uniform_float(bounds.second.z, "max_height");

	/*since each object has its own own data buffers and textures inside *Mesh*, we dont need multiple vertex arrays, instead we simply bind said buffers to the shader every time we want to draw said object
	we do that by using the *Shader::draw_mesh_elements* function, where this function binds the *Mesh* data and draws it directly*/
	unsigned int vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	shader.bind_mesh_buffers_and_textures(mesh, GL_STATIC_DRAW, shader.get_reference_bool_uniform("gamma_correction"));
	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.update_uniforms();
		user_interface.new_frame();
		mouse.update(window);
		
		//shader.bind_and_draw_mesh_elements(mesh, GL_POINTS, GL_STATIC_DRAW, booleans_container.gamma_correction);
		shader.draw_mesh_elements(mesh, GL_POINTS);

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