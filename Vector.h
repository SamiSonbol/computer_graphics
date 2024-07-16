#pragma once
#include <iostream>
#include <cmath>
#include <vector>


class point {

public:

	float x, y, z, w;

	point(float x, float y, float z, float w = 1) {

		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;

	};

	void dehomogenize() {

		this->x /= this->w;
		this->y /= this->w;
		this->z /= this->w;
		this->w = 1;

	};

};

class vec2 {

public:

	float x, y;

	vec2(float x = 0.0f, float y = 0.0f) {

		this->x = x;
		this->y = y;

	};

	float length() {

		return std::sqrt(this->x * this->x + this->y * this->y);

	};

	float dot(const vec2& vec) {

		return this->x * vec.x + this->y * vec.y;

	};

	vec2 operator-(const vec2& vec) {

		return vec2(this->x - vec.x, this->y - vec.y);

	};

	vec2 operator*(float n) {

		return vec2(this->x * n, this->y * n);

	};

	void normalize() {

		float n = length();

		if (n != 0) {

			this->x = this->x / n;
			this->y = this->y / n;

		};

	};

	std::vector<float> to_GL() {

		std::vector<float> m = { this->x, this->y };

		return m;

	};


};

class vec3 {

public:

	float x, y, z;

	vec3(float x = 0.0f, float y = 0.0f, float z = 1.0f) {

		this->x = x;
		this->y = y;
		this->z = z;

	};

	float length() {

		return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);

	};

	float dot(const vec3& vec) {

		return this->x * vec.x + this->y * vec.y + this->z * vec.z;

	};

	vec3 cross(const vec3& vec) {

		return vec3(this->y * vec.z - this->z * vec.y,
			this->z * vec.x - this->x * vec.z,
			this->x * vec.y - this->y * vec.x);

	};

	vec3 operator-(const vec3& vec) {

		return vec3(this->x - vec.x, this->y - vec.y, this->z - vec.z);

	};

	vec3 operator+(const vec3& vec) {

		return vec3(this->x + vec.x, this->y + vec.y, this->z + vec.z);

	};

	vec3 operator*(float n) {

		return vec3(this->x * n, this->y * n, this->z * n);

	};

	void normalize() {

		float n = length();

		if (n != 0) {

			this->x = this->x / n;
			this->y = this->y / n;
			this->z = this->z / n;

		};

	};

	std::vector<float> to_GL() {

		std::vector<float> m = { this->x, this->y, this->z };

		return m;

	};

};

class vec4 {

public:

	float x, y, z, w;
	vec4(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) {

		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;

	};

	vec4(const vec3& vec, float w = 1.0f) {

		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
		this->w = w;

	};

	float length() {

		return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);

	};

	float dot(const vec4& vec) {

		return this->x * vec.x + this->y * vec.y + this->z * vec.z + this->w * vec.w;

	};

	vec4 operator-(const vec4& vec) {

		return vec4(this->x - vec.x, this->y - vec.y, this->z - vec.z, 1.0);

	};

	vec4 subtract(const point& p) {

		return vec4(this->x - p.x, this->y - p.y, this->z - p.z, 1);

	};

	vec4 operator*(float n) {

		return vec4(this->x * n, this->y * n, this->z * n, 1);

	};

	void normalize() {

		float n = length();

		if (n != 0) {

			this->x = this->x / n;
			this->y = this->y / n;
			this->z = this->z / n;
			this->w = this->w / n;

		};

	};

	std::vector<float> to_GL() {

		std::vector<float> m = { this->x, this->y, this->z, this->w };

		return m;

	};

};

class mat3 {

public:

    float a11, a12, a13;
    float a21, a22, a23;
    float a31, a32, a33;

	mat3(

		float a11 = 1, float a12 = 0, float a13 = 0,
		float a21 = 0, float a22 = 1, float a23 = 0,
		float a31 = 0, float a32 = 0, float a33 = 1) {

		this->a11 = a11;
		this->a12 = a12;
		this->a13 = a13;

		this->a21 = a21;
		this->a22 = a22;
		this->a23 = a23;

		this->a31 = a31;
		this->a32 = a32;
		this->a33 = a33;

	};

	mat3 transpose() {

		return mat3(

			this->a11, this->a21, this->a31,
			this->a12, this->a22, this->a32,
			this->a13, this->a23, this->a33

		);

	};

	mat3 multiply_matrix(const mat3& mat) {

		return mat3(

			this->a11 * mat.a11 + this->a12 * mat.a21 + this->a13 * mat.a31, this->a11 * mat.a12 + this->a12 * mat.a22 + this->a13 * mat.a32, this->a11 * mat.a13 + this->a12 * mat.a23 + this->a13 * mat.a33,
			this->a21 * mat.a11 + this->a22 * mat.a21 + this->a23 * mat.a31, this->a21 * mat.a12 + this->a22 * mat.a22 + this->a23 * mat.a32, this->a21 * mat.a13 + this->a22 * mat.a23 + this->a23 * mat.a33,
			this->a31 * mat.a11 + this->a32 * mat.a21 + this->a33 * mat.a31, this->a31 * mat.a12 + this->a32 * mat.a22 + this->a33 * mat.a32, this->a31 * mat.a13 + this->a32 * mat.a23 + this->a33 * mat.a33

		);

	};

	mat3 multiply_number(float n) {

		return mat3(

			this->a11 * n, this->a12 * n, this->a13 * n,
			this->a21 * n, this->a22 * n, this->a23 * n,
			this->a31 * n, this->a32 * n, this->a33 * n

		);

	};

	vec3 multiply_vector(const vec3& vec) {

		return vec3(

			this->a11 * vec.x + this->a12 * vec.y + this->a13 * vec.z,
			this->a21 * vec.x + this->a22 * vec.y + this->a23 * vec.z,
			this->a31 * vec.x + this->a32 * vec.y + this->a33 * vec.z

		);

	};

};

class mat4 {

public:

    float a11, a12, a13, a14;
    float a21, a22, a23, a24;
    float a31, a32, a33, a34;
    float a41, a42, a43, a44;

	mat4(

		float a11 = 1, float a12 = 0, float a13 = 0, float a14 = 0,
		float a21 = 0, float a22 = 1, float a23 = 0, float a24 = 0,
		float a31 = 0, float a32 = 0, float a33 = 1, float a34 = 0,
		float a41 = 0, float a42 = 0, float a43 = 0, float a44 = 1) {

		this->a11 = a11;
		this->a12 = a12;
		this->a13 = a13;
		this->a14 = a14;

		this->a21 = a21;
		this->a22 = a22;
		this->a23 = a23;
		this->a24 = a24;

		this->a31 = a31;
		this->a32 = a32;
		this->a33 = a33;
		this->a34 = a34;

		this->a41 = a41;
		this->a42 = a42;
		this->a43 = a43;
		this->a44 = a44;

	};

	mat4 transpose() {

		return mat4(

			this->a11, this->a21, this->a31, this->a41,
			this->a12, this->a22, this->a32, this->a42,
			this->a13, this->a23, this->a33, this->a43,
			this->a14, this->a24, this->a34, this->a44

		);

	};

	mat4 multiply_matrix(const mat4& mat) {

		return mat4(

			this->a11 * mat.a11 + this->a12 * mat.a21 + this->a13 * mat.a31 + this->a14 * mat.a41,
			this->a11 * mat.a12 + this->a12 * mat.a22 + this->a13 * mat.a32 + this->a14 * mat.a42,
			this->a11 * mat.a13 + this->a12 * mat.a23 + this->a13 * mat.a33 + this->a14 * mat.a43,
			this->a11 * mat.a14 + this->a12 * mat.a24 + this->a13 * mat.a34 + this->a14 * mat.a44,

			this->a21 * mat.a11 + this->a22 * mat.a21 + this->a23 * mat.a31 + this->a24 * mat.a41,
			this->a21 * mat.a12 + this->a22 * mat.a22 + this->a23 * mat.a32 + this->a24 * mat.a42,
			this->a21 * mat.a13 + this->a22 * mat.a23 + this->a23 * mat.a33 + this->a24 * mat.a43,
			this->a21 * mat.a14 + this->a22 * mat.a24 + this->a23 * mat.a34 + this->a24 * mat.a44,

			this->a31 * mat.a11 + this->a32 * mat.a21 + this->a33 * mat.a31 + this->a34 * mat.a41,
			this->a31 * mat.a12 + this->a32 * mat.a22 + this->a33 * mat.a32 + this->a34 * mat.a42,
			this->a31 * mat.a13 + this->a32 * mat.a23 + this->a33 * mat.a33 + this->a34 * mat.a43,
			this->a31 * mat.a14 + this->a32 * mat.a24 + this->a33 * mat.a34 + this->a34 * mat.a44,

			this->a41 * mat.a11 + this->a42 * mat.a21 + this->a43 * mat.a31 + this->a44 * mat.a41,
			this->a41 * mat.a12 + this->a42 * mat.a22 + this->a43 * mat.a32 + this->a44 * mat.a42,
			this->a41 * mat.a13 + this->a42 * mat.a23 + this->a43 * mat.a33 + this->a44 * mat.a43,
			this->a41 * mat.a14 + this->a42 * mat.a24 + this->a43 * mat.a34 + this->a44 * mat.a44

		);
	};

	mat4 multiply_number(float n) {

		return mat4(

			this->a11 * n, this->a12 * n, this->a13 * n, this->a14 * n,
			this->a21 * n, this->a22 * n, this->a23 * n, this->a24 * n,
			this->a31 * n, this->a32 * n, this->a33 * n, this->a34 * n,
			this->a41 * n, this->a42 * n, this->a43 * n, this->a44 * n

		);

	};

	vec4 multiply_vector(const vec4& vec) {

		return vec4(

			this->a11 * vec.x + this->a12 * vec.y + this->a13 * vec.z + this->a14 * vec.w,
			this->a21 * vec.x + this->a22 * vec.y + this->a23 * vec.z + this->a24 * vec.w,
			this->a31 * vec.x + this->a32 * vec.y + this->a33 * vec.z + this->a34 * vec.w,
			this->a41 * vec.x + this->a42 * vec.y + this->a43 * vec.z + this->a44 * vec.w

		);

	};

	mat4 inverse() {

		float det = a11 * (a22 * (a33 * a44 - a34 * a43) - a23 * (a32 * a44 - a34 * a42) + a24 * (a32 * a43 - a33 * a42))
			- a12 * (a21 * (a33 * a44 - a34 * a43) - a23 * (a31 * a44 - a34 * a41) + a24 * (a31 * a43 - a33 * a41))
			+ a13 * (a21 * (a32 * a44 - a34 * a42) - a22 * (a31 * a44 - a34 * a41) + a24 * (a31 * a42 - a32 * a41))
			- a14 * (a21 * (a32 * a43 - a33 * a42) - a22 * (a31 * a43 - a33 * a41) + a23 * (a31 * a42 - a32 * a41));

		if (det == 0) {

			std::cerr << "Matrix is singular and cannot be inverted." << std::endl;
			return mat4(); // Return identity or some error indicator

		}

		float inv_det = 1.0f / det;

		return mat4(

			inv_det * (a22 * (a33 * a44 - a34 * a43) - a23 * (a32 * a44 - a34 * a42) + a24 * (a32 * a43 - a33 * a42)),
			inv_det * -(a12 * (a33 * a44 - a34 * a43) - a13 * (a32 * a44 - a34 * a42) + a14 * (a32 * a43 - a33 * a42)),
			inv_det * (a12 * (a23 * a44 - a24 * a43) - a13 * (a22 * a44 - a24 * a42) + a14 * (a22 * a43 - a23 * a42)),
			inv_det * -(a12 * (a23 * a34 - a24 * a33) - a13 * (a22 * a34 - a24 * a32) + a14 * (a22 * a33 - a23 * a32)),

			inv_det * -(a21 * (a33 * a44 - a34 * a43) - a23 * (a31 * a44 - a34 * a41) + a24 * (a31 * a43 - a33 * a41)),
			inv_det * (a11 * (a33 * a44 - a34 * a43) - a13 * (a31 * a44 - a34 * a41) + a14 * (a31 * a43 - a33 * a41)),
			inv_det * -(a11 * (a23 * a44 - a24 * a43) - a13 * (a21 * a44 - a24 * a41) + a14 * (a21 * a43 - a23 * a41)),
			inv_det * (a11 * (a23 * a34 - a24 * a33) - a13 * (a21 * a34 - a24 * a31) + a14 * (a21 * a33 - a23 * a31)),

			inv_det * (a21 * (a32 * a44 - a34 * a42) - a22 * (a31 * a44 - a34 * a41) + a24 * (a31 * a42 - a32 * a41)),
			inv_det * -(a11 * (a32 * a44 - a34 * a42) - a12 * (a31 * a44 - a34 * a41) + a14 * (a31 * a42 - a32 * a41)),
			inv_det * (a11 * (a22 * a44 - a24 * a42) - a12 * (a21 * a44 - a24 * a41) + a14 * (a21 * a42 - a22 * a41)),
			inv_det * -(a11 * (a22 * a34 - a24 * a32) - a12 * (a21 * a34 - a24 * a31) + a14 * (a21 * a32 - a22 * a31)),

			inv_det * -(a21 * (a32 * a43 - a33 * a42) - a22 * (a31 * a43 - a33 * a41) + a23 * (a31 * a42 - a32 * a41)),
			inv_det * (a11 * (a32 * a43 - a33 * a42) - a12 * (a31 * a43 - a33 * a41) + a13 * (a31 * a42 - a32 * a41)),
			inv_det * -(a11 * (a22 * a43 - a23 * a42) - a12 * (a21 * a43 - a23 * a41) + a13 * (a21 * a42 - a22 * a41)),
			inv_det * (a11 * (a22 * a33 - a23 * a32) - a12 * (a21 * a33 - a23 * a31) + a13 * (a21 * a32 - a22 * a31))

		);
	}

	std::vector<float> to_GL() const {

		return {

			a11, a21, a31, a41,
			a12, a22, a32, a42,
			a13, a23, a33, a43,
			a14, a24, a34, a44

		};

	}

};

static float to_radians(float degree) {

	return degree * 3.14159265359 / 180;

};

//2D
static mat3 create_translation_matrix(float x, float y) {

	return mat3(

		1, 0, x,
		0, 1, y,
		0, 0, 1

	);

};

static mat3 create_scale_matrix(float x, float y) {

	return mat3(

		x, 0, 0,
		0, y, 0,
		0, 0, 1

	);

};

static mat3 create_rotation_matrix(float alpha) {

	alpha = to_radians(alpha);

	return mat3(

		std::cos(alpha), -std::sin(alpha), 0,
		std::sin(alpha), std::cos(alpha), 0,
		0, 0, 1

	);

};

static mat3 create_model_transformation_matrix(float Tx, float Ty, float Sx, float Sy, float alpha) {

	mat3 R = create_rotation_matrix(alpha);
	mat3 S = create_scale_matrix(Sx, Sy);
	mat3 T = create_translation_matrix(Tx, Ty);

	return R.multiply_matrix(S).multiply_matrix(T);

};

static mat3 create_model_transformation_matrix(mat3& T, mat3& S, mat3& R) {

	return R.multiply_matrix(S).multiply_matrix(T);

};

//3D
static mat4 create_translation_matrix(float x, float y, float z) {

	return mat4(

		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1

	);

};

static mat4 create_scale_matrix(float x, float y, float z) {

	return mat4(

		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1

	);

};

static mat4 create_X_rotation_matrix(float alpha) {

	alpha = to_radians(alpha);

	return mat4(

		1, 0, 0, 0,
		0, std::cos(alpha), std::sin(alpha), 0,
		0, -std::sin(alpha), std::cos(alpha), 0,
		0, 0, 0, 1

	);

};

static mat4 create_Y_rotation_matrix(float alpha) {

	alpha = to_radians(alpha);

	return mat4(

		std::cos(alpha), 0, -std::sin(alpha), 0,
		0, 1, 0, 0,
		std::sin(alpha), 0, std::cos(alpha), 0,
		0, 0, 0, 1

	);

};

static mat4 create_Z_rotation_matrix(float alpha) {

	alpha = to_radians(alpha);

	return mat4(

		std::cos(alpha), std::sin(alpha), 0, 0,
		-std::sin(alpha), std::cos(alpha), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1

	);

};

static mat4 create_rotation_matrix(float alpha, float theta, float delta) {

	return create_X_rotation_matrix(alpha).multiply_matrix(create_Y_rotation_matrix(theta)).multiply_matrix(create_Z_rotation_matrix(delta));

};

static mat4 create_model_transformation_matrix(const vec3& T, const vec3& S, const vec3& R) {

	mat4 translation = create_translation_matrix(T.x, T.y, T.z);
	mat4 scaling = create_scale_matrix(S.x, S.y, S.z);
	mat4 rotation = create_X_rotation_matrix(R.x).multiply_matrix(create_Y_rotation_matrix(R.y)).multiply_matrix(create_Z_rotation_matrix(R.z));

	return rotation.multiply_matrix(scaling).multiply_matrix(translation);

};

static mat4 create_model_transformation_matrix(mat4& T, mat4& S, mat4& R) {

	return R.multiply_matrix(S).multiply_matrix(T);

};

//3D SCREEN TRANSFORMATION
static mat4 create_view_matrix(vec3& r, vec3& u, vec3& d, vec3& camera) {//also known as lookAtMatrix

    //here am basically taking r as X-axis, u as Y-axis, and d as Z-axis and using them to find the actual right, up, direction vectors
	vec3 direction = vec3(0, 0, 0) - camera;
	direction.normalize();

	vec3 right = u.cross(direction);
	right.normalize();

	vec3 up = direction.cross(right);

	return mat4(

		right.x, right.y, right.z, -right.dot(camera),
		up.x, up.y, up.z, -up.dot(camera),
		-direction.x, -direction.y, -direction.z, direction.dot(camera),
		0, 0, 0, 1

	);

};

static mat4 create_parallel_projection_matrix(float fov, float screen_width, float screen_height, float near, float far) {//also known as orthographic projection matrix

	fov = to_radians(fov);

	float aspect_ratio = screen_width / screen_height;
	float alpha = tan(fov / 2.0f);
	float top = near * alpha;
	float bottom = -top;
	float right = near * aspect_ratio * alpha;
	float left = -right;

	return mat4(

		2 / (right - left), 0, 0, -(right + left) / (right - left),
		0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
		0, 0, -2 / (far - near), -(far + near) / (far - near),
		0, 0, 0, 1

	);

};

static mat4 create_frustum_projection_matrix(float fov, float screen_width, float screen_height, float near, float far) {

	fov = to_radians(fov);

	float aspect_ratio = screen_width / screen_height;
	float alpha = tan(fov / 2.0f);
	float top = near * alpha;
	float bottom = -top;
	float right = near * aspect_ratio * alpha;
	float left = -right;

	mat4 frustum = mat4(

		(2 * near) / (right - left), 0, (right + left) / (right - left), 0,
		0, (2 * near) / (top - bottom), (top + bottom) / (top - bottom), 0,
		0, 0, (-far - near) / (far - near), (-2 * far * near) / (far - near),
		0, 0, -1, 0

	);

	return frustum;

};

//in openGL we dont require a view port matrix since the API transforms our coordinates automatically to NDC, we just need to use the function glViewport() at start of program initialization
static mat4 create_view_port_matrix(float fov, float screen_width, float screen_height, float near, float far) {

	fov = to_radians(fov);

	float aspect_ratio = screen_width / screen_height;
	float alpha = tan(fov / 2.0f);
	float top = near * alpha;
	float bottom = -top;
	float right = near * aspect_ratio * alpha;
	float left = -right;

	return mat4(

		screen_width / 2, 0, 0, left + (screen_width / 2),
		0, screen_height / 2, 0, bottom + (screen_height / 2),
		0, 0, (far - near) / 2, (far + near) / 2,
		0, 0, 0, 1

	);

};

static mat4 create_model_view_matrix(mat4& model_transformation_matrix, mat4& look_at_matrix) {

	return look_at_matrix.multiply_matrix(model_transformation_matrix);

};

static mat4 create_model_view_projection_matrix(mat4& model_transformation_matrix, mat4& look_at_matrix, mat4& frustum_matrix) {

	return frustum_matrix.multiply_matrix(create_model_view_matrix(model_transformation_matrix, look_at_matrix));

};

static float get_determinant(const vec2& A, const vec2& B) {

	float a = A.x * B.y;
	float b = A.y * B.x;

	return a - b;

}

static float get_area_of_triangle(vec2& a, vec2& b, vec2& c) {

	vec2 ab = b - a;
	vec2 ac = c - a;

	return 0.5 * get_determinant(ab, ac);

}

//openGL calculates these automatically
static std::vector<float> get_barycentric_coordinates(vec3& point, vec3& A, vec3& B, vec3& C) {

	vec2 p = vec2(point.x, point.y);
	vec2 a = vec2(A.x, A.y);
	vec2 b = vec2(B.x, B.y);
	vec2 c = vec2(C.x, C.y);

	std::vector<vec2> main_triangle = { a, b, c };
	std::vector<vec2> beta_triangle = { p, c, a };
	std::vector<vec2> alpha_triangle = { p, b, c };
	std::vector<vec2> gamma_triangle = { p, a, b };

	float area_main = get_area_of_triangle(main_triangle[0], main_triangle[1], main_triangle[2]);
	float area_alpha = get_area_of_triangle(alpha_triangle[0], alpha_triangle[1], alpha_triangle[2]);
	float area_beta = get_area_of_triangle(beta_triangle[0], beta_triangle[1], beta_triangle[2]);
	float area_gamma = get_area_of_triangle(gamma_triangle[0], gamma_triangle[1], gamma_triangle[2]);

	float alpha = area_alpha / area_main;
	float beta = area_beta / area_main;
	float gamma = area_gamma / area_main;

	return { alpha, beta, gamma };

}