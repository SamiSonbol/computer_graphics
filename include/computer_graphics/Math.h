#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <bit>
#include <float.h>
#include <limits>
#include <cstdint>

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

	vec2(const float& x = 0.0f, const float& y = 0.0f) : x(x), y(y) {};

	float magnitude() const {

		return std::sqrt(this->x * this->x + this->y * this->y);

	};

	float dot(const vec2& vec) {

		return this->x * vec.x + this->y * vec.y;

	};

	vec2 operator-(const vec2& vec) const {

		return vec2(this->x - vec.x, this->y - vec.y);

	};
	void operator-=(const vec2& vec) {

		this->x -= vec.x;
		this->y -= vec.y;

	};

	vec2 operator+(const vec2& vec) const {

		return vec2(this->x + vec.x, this->y + vec.y);

	};
	void operator+=(const vec2& vec) {

		this->x += vec.x;
		this->y += vec.y;

	};

	vec2 operator*(const vec2& vec) const {

		return vec2(this->x * vec.x, this->y * vec.y);

	};
	vec2 operator*(const float& n) const {

		return vec2(this->x * n, this->y * n);

	};
	void operator*=(const vec2& vec) {

		this->x *= vec.x;
		this->y *= vec.y;

	};
	void operator*=(const float& n) {

		this->x *= n;
		this->y *= n;

	};

	vec2 operator/(const vec2& vec) const {

		float x = this->x, y = this->y;

		if (this->x != 0 && vec.x != 0) { x = this->x / vec.x; };

		if (this->y != 0 && vec.y != 0) { y = this->y / vec.y; };

		return vec2(x, y);

	};
	vec2 operator/(const float& n) const {

		float x = this->x, y = this->y;

		if (this->x != 0 && n != 0) { x = this->x / n; };

		if (this->y != 0 && n != 0) { y = this->y / n; };

		return vec2(x, y);

	};
	void operator/=(const vec2& vec) {

		if (this->x != 0 && vec.x != 0) { this->x /= vec.x; };

		if (this->y != 0 && vec.y != 0) { this->y /= vec.y; };

	};
	void operator/=(const float& n) {

		if (this->x != 0 && n != 0) { this->x /= n; };

		if (this->y != 0 && n != 0) { this->y /= n; };

	};

	bool operator==(const vec2& vec) const {

		return this->x == vec.x && this->y == vec.y;

	};

	bool operator!=(const vec2& vec) const {

		return !(*this == vec);

	};

	void operator=(const float array[2]) {

		if (array) {

			this->x = array[0];
			this->y = array[1];

		}
		else {

			std::cerr << "WARNING: dynamically allocated array is NULL, no new value was given to this vec2\n";

		};

	};

	vec2 normalize() const {

	    return vec2(this->x, this->y) / this->magnitude();


	};

	//VIMP/WARNING: this function is only used WHEN YOU ABSOLUTELY HAVE A REASON TO USE IT, and it the memory allocated from it has to be deallocated afterwards
	float* to_dynamically_allocated_array() const {

		float* array = new float[2];
		array[0] = this->x;
		array[1] = this->y;
		return array;

	};

	std::vector<float> to_GL() {

		return std::vector<float> { this->x, this->y };

	};


};

class vec3 {

public:

	float x, y, z;

	vec3(const float& x = 0.0f, const float& y = 0.0f, const float& z = 0.0f) : x(x), y(y), z(z) {};

	vec3(const vec2& vec, const float& z = 0.0f) : x(vec.x), y(vec.y), z(z) {};//override

	float magnitude() const {

		return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);

	};

	float dot(const vec3& vec) const {

		return this->x * vec.x + this->y * vec.y + this->z * vec.z;

	};

	vec3 cross(const vec3& vec) const {

		return vec3(this->y * vec.z - this->z * vec.y,
			this->z * vec.x - this->x * vec.z,
			this->x * vec.y - this->y * vec.x);

	};

	vec3 operator-(const vec3& vec) const {

		return vec3(this->x - vec.x, this->y - vec.y, this->z - vec.z);

	};
	void operator-=(const vec3& vec) {

		this->x -= vec.x;
		this->y -= vec.y;
		this->z -= vec.z;

	};

	vec3 operator+(const vec3& vec) const {

		return vec3(this->x + vec.x, this->y + vec.y, this->z + vec.z);

	};
	void operator+=(const vec3& vec) {

		this->x += vec.x;
		this->y += vec.y;
		this->z += vec.z;

	};

	vec3 operator*(const vec3& vec) const {

		return vec3(this->x * vec.x, this->y * vec.y, this->z * vec.z);

	};
	vec3 operator*(const float& n) const {

		return vec3(this->x * n, this->y * n, this->z * n);

	};
	void operator*=(const vec3& vec) {

		this->x *= vec.x;
		this->y *= vec.y;
		this->z *= vec.z;

	};
	void operator*=(const float& n) {

		this->x *= n;
		this->y *= n;
		this->z *= n;

	};

	vec3 operator/(const vec3& vec) const {

		float x = this->x, y = this->y, z = this->z;

		if (this->x != 0 && vec.x != 0) { x = this->x / vec.x; };

		if (this->y != 0 && vec.y != 0) { y = this->y / vec.y; };

		if (this->z != 0 && vec.z != 0) { z = this->z / vec.z; };

		return vec3(x, y, z);

	};
	vec3 operator/(const float& n) const {

		float x = this->x, y = this->y, z = this->z;

		if (this->x != 0 && n != 0) { x = this->x / n; };

		if (this->y != 0 && n != 0) { y = this->y / n; };

		if (this->z != 0 && n != 0) { z = this->z / n; };

		return vec3(x, y, z);

	};
	void operator/=(const vec3& vec) {

		if (this->x != 0 && vec.x != 0) { this->x /= vec.x; };

		if (this->y != 0 && vec.y != 0) { this->y /= vec.y; };

		if (this->z != 0 && vec.z != 0) { this->z /= vec.z; };

	};
	void operator/=(const float& n) {

		if (this->x != 0 && n != 0) { this->x /= n; };

		if (this->y != 0 && n != 0) { this->y /= n; };

		if (this->z != 0 && n != 0) { this->z /= n; };

	};

	bool operator==(const vec3& vec) const {

		return this->x == vec.x && this->y == vec.y && this->z == vec.z;

	};

	bool operator!=(const vec3& vec) const {

		return !(*this == vec);

	};

	void operator=(const float array[3]) {

		if (array) {

			this->x = array[0];
			this->y = array[1];
			this->z = array[2];

		}
		else {

			std::cerr << "WARNING: dynamically allocated array is NULL, no new value was given to this vec2\n";

		};

	};

	vec3 normalize() const {

		return vec3(this->x, this->y, this->z) / this->magnitude();

	};

	vec2 xy() const {

		return vec2(this->x, this->y);

	};

	//VIMP/WARNING: this function is only used WHEN YOU ABSOLUTELY HAVE A REASON TO USE IT, and it the memory allocated from it has to be deallocated afterwards
	float* to_dynamically_allocated_array() const {

		float* array = new float[3];
		array[0] = this->x;
		array[1] = this->y;
		array[2] = this->z;
		return array;

	};

	std::vector<float> to_GL() const {

		return { this->x, this->y, this->z };

	};

};

class vec4 {

public:

	float x, y, z, w;
	vec4(const float& x = 0.0f, const float& y = 0.0f, const float& z = 0.0f, const float& w = 1.0f) : x(x), y(y), z(z), w(w) {};

	vec4(const vec3& vec, const float& w = 1.0f) : x(vec.x), y(vec.y), z(vec.z), w(w) {};//override

	vec4(const vec2& vec, const float z = 0.0f, const float& w = 1.0f) : x(vec.x), y(vec.y), z(z), w(w) {};//override

	float magnitude() const {

		return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);

	};

	float dot(const vec4& vec) const {

		return this->x * vec.x + this->y * vec.y + this->z * vec.z + this->w * vec.w;

	};

	vec4 operator-(const vec4& vec) const {

		return vec4(this->x - vec.x, this->y - vec.y, this->z - vec.z, this->w - vec.w);

	};
	void operator-=(const vec4& vec) {

		this->x -= vec.x;
		this->y -= vec.y;
		this->z -= vec.z;
		this->w -= vec.w;

	};

	vec4 operator+(const vec4& vec) const {

		return vec4(this->x + vec.x, this->y + vec.y, this->z + vec.z, this->w + vec.w);

	};
	void operator+=(const vec4& vec) {

		this->x += vec.x;
		this->y += vec.y;
		this->z += vec.z;
		this->w += vec.w;

	};

	vec4 operator*(const vec4& vec) const {

		return vec4(this->x * vec.x, this->y * vec.y, this->z * vec.z, this->w * vec.w);

	};
	vec4 operator*(const float& n) const {

		return vec4(this->x * n, this->y * n, this->z * n, this->w * n);

	};
	void operator*=(const vec4& vec) {

		this->x *= vec.x;
		this->y *= vec.y;
		this->z *= vec.z;
		this->w *= vec.w;

	};
	void operator*=(const float& n) {

		this->x *= n;
		this->y *= n;
		this->z *= n;
		this->w *= n;

	};

	vec4 operator/(const vec4& vec) const {

		float x = this->x, y = this->y, z = this->z, w = this->w;

		if (this->x != 0 && vec.x != 0) { x = this->x / vec.x; };

		if (this->y != 0 && vec.y != 0) { y = this->y / vec.y; };

		if (this->z != 0 && vec.z != 0) { z = this->z / vec.z; };

		if (this->w != 0 && vec.w != 0) { w = this->w / vec.w; };

		return vec4(x, y, z, w);

	};
	vec4 operator/(const float& n) const {

		float x = this->x, y = this->y, z = this->z, w = this->w;

		if (this->x != 0 && n != 0) { x = this->x / n; };

		if (this->y != 0 && n != 0) { y = this->y / n; };

		if (this->z != 0 && n != 0) { z = this->z / n; };

		if (this->w != 0 && n != 0) { w = this->w / n; };

		return vec4(x, y, z, w);

	};
	void operator/=(const vec4& vec) {

		if (this->x != 0 && vec.x != 0) { this->x /= vec.x; };

		if (this->y != 0 && vec.y != 0) { this->y /= vec.y; };

		if (this->z != 0 && vec.z != 0) { this->z /= vec.z; };

		if (this->w != 0 && vec.w != 0) { this->w /= vec.w; };

	};
	void operator/=(const float& n) {

		if (this->x != 0 && n != 0) { this->x /= n; };

		if (this->y != 0 && n != 0) { this->y /= n; };

		if (this->z != 0 && n != 0) { this->z /= n; };

		if (this->w != 0 && n != 0) { this->w /= n; };

	};

	bool operator==(const vec4& vec) const {

		return this->x == vec.x && this->y == vec.y && this->z == vec.z && this->w == w;

	};

	bool operator!=(const vec4& vec) const {

		return !(*this == vec);

	};

	void operator=(const float array[4]) {

		if (array) {

			this->x = array[0];
			this->y = array[1];
			this->z = array[2];
			this->w = array[3];

		}
		else {

			std::cerr << "WARNING: dynamically allocated array is NULL, no new value was given to this vec2\n";

		};

	};

	vec4 normalize() const {

		return vec4(this->x, this->y, this->z, this->w) / this->magnitude();

	};

	vec2 xy() const {

		return vec2(this->x, this->y);

	};

	vec3 xyz() const {

		return vec3(this->x, this->y, this->z);

	};

	//VIMP/WARNING: this function is only used WHEN YOU ABSOLUTELY HAVE A REASON TO USE IT, and it the memory allocated from it has to be deallocated afterwards
	float* to_dynamically_allocated_array() const {

		float* array = new float[4];
		array[0] = this->x;
		array[1] = this->y;
		array[2] = this->z;
		array[3] = this->w;
		return array;

	};

	std::vector<float> to_GL() {

		return std::vector<float> { this->x, this->y, this->z, this->w };

	};

};

class mat3 {

public:

	float a11, a12, a13;
	float a21, a22, a23;
	float a31, a32, a33;

	mat3(

		float a11 = 1.0f, float a12 = 0.0f, float a13 = 0.0f,
		float a21 = 0.0f, float a22 = 1.0f, float a23 = 0.0f,
		float a31 = 0.0f, float a32 = 0.0f, float a33 = 1.0f) :

		a11(a11), a12(a12), a13(a13),
		a21(a21), a22(a22), a23(a23),
		a31(a31), a32(a32), a33(a33) {

	};

	mat3(const vec3& X, const vec3& Y, const vec3& Z)://override
	
		a11(X.x), a12(Y.x), a13(Z.x),
		a21(X.y), a22(Y.y), a23(Z.y),
		a31(X.z), a32(Y.z), a33(Z.z) {

	};

	mat3 operator*(const mat3& mat) const {

		return mat3(

			this->a11 * mat.a11 + this->a12 * mat.a21 + this->a13 * mat.a31, this->a11 * mat.a12 + this->a12 * mat.a22 + this->a13 * mat.a32, this->a11 * mat.a13 + this->a12 * mat.a23 + this->a13 * mat.a33,
			this->a21 * mat.a11 + this->a22 * mat.a21 + this->a23 * mat.a31, this->a21 * mat.a12 + this->a22 * mat.a22 + this->a23 * mat.a32, this->a21 * mat.a13 + this->a22 * mat.a23 + this->a23 * mat.a33,
			this->a31 * mat.a11 + this->a32 * mat.a21 + this->a33 * mat.a31, this->a31 * mat.a12 + this->a32 * mat.a22 + this->a33 * mat.a32, this->a31 * mat.a13 + this->a32 * mat.a23 + this->a33 * mat.a33

		);

	};

	mat3 operator*(const float& n) const {

		return mat3(

			this->a11 * n, this->a12 * n, this->a13 * n,
			this->a21 * n, this->a22 * n, this->a23 * n,
			this->a31 * n, this->a32 * n, this->a33 * n

		);

	};

	vec3 operator*(const vec3& vec) const {

		return vec3(

			this->a11 * vec.x + this->a12 * vec.y + this->a13 * vec.z,
			this->a21 * vec.x + this->a22 * vec.y + this->a23 * vec.z,
			this->a31 * vec.x + this->a32 * vec.y + this->a33 * vec.z

		);

	};

	mat3 transpose() const {

		return mat3(

			this->a11, this->a21, this->a31,
			this->a12, this->a22, this->a32,
			this->a13, this->a23, this->a33

		);

	};

	mat3 cofactor() const {

		return mat3(

			(this->a22 * this->a33 - this->a23 * this->a32), -(this->a12 * this->a33 - this->a13 * this->a32), (this->a12 * this->a23 - this->a13 * this->a22),
			-(this->a21 * this->a33 - this->a23 * this->a31), (this->a11 * this->a33 - this->a13 * this->a31), -(this->a11 * this->a23 - this->a13 * this->a21),
			(this->a21 * this->a32 - this->a22 * this->a31), -(this->a11 * this->a32 - this->a12 * this->a31), (this->a11 * this->a22 - this->a12 * this->a21)
			
		);


	};

	float determinate() const {

		return   this->a11 * (this->a22 * this->a33 - this->a23 * this->a32)
		       - this->a12 * (this->a21 * this->a33 - this->a23 * this->a31)
		       + this->a13 * (this->a21 * this->a32 - this->a22 * this->a31);

	};

	mat3 inverse() const {

		float det = this->determinate();
		if (det == 0) {

			std::cerr << "Matrix is singular and cannot be inverted\n";
			return mat3();

		};

		mat3 adjugate = this->cofactor().transpose();
		return adjugate * (1.0f / det);

	};

	std::vector<float> to_std_vector() {

		return {

			this->a11, this->a12, this->a13,
			this->a21, this->a22, this->a23,
			this->a31, this->a32, this->a33,

		};

	};

	std::vector<float> to_GL() const {

		return this->transpose().to_std_vector();

	};

};

class mat4 {

public:

	float a11, a12, a13, a14;
	float a21, a22, a23, a24;
	float a31, a32, a33, a34;
	float a41, a42, a43, a44;

	mat4(

		float a11 = 1.0f, float a12 = 0.0f, float a13 = 0.0f, float a14 = 0.0f,
		float a21 = 0.0f, float a22 = 1.0f, float a23 = 0.0f, float a24 = 0.0f,
		float a31 = 0.0f, float a32 = 0.0f, float a33 = 1.0f, float a34 = 0.0f,
		float a41 = 0.0f, float a42 = 0.0f, float a43 = 0.0f, float a44 = 1.0f) :

		a11(a11), a12(a12), a13(a13), a14(a14),
		a21(a21), a22(a22), a23(a23), a24(a24),
		a31(a31), a32(a32), a33(a33), a34(a34),
		a41(a41), a42(a42), a43(a43), a44(a44) {

	};
	
	mat4(const vec4& X, const vec4& Y, const vec4& Z, const vec4& W) ://override

		a11(X.x), a12(Y.x), a13(Z.x), a14(W.x),
		a21(X.y), a22(Y.y), a23(Z.y), a24(W.y),
		a31(X.z), a32(Y.z), a33(Z.z), a34(W.z),
	    a41(X.w), a42(Y.w), a43(Z.w), a44(W.w) {

	};

	mat4 operator*(const mat4& mat) const {

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

	mat4 operator*(const float& n) const {

		return mat4(

			this->a11 * n, this->a12 * n, this->a13 * n, this->a14 * n,
			this->a21 * n, this->a22 * n, this->a23 * n, this->a24 * n,
			this->a31 * n, this->a32 * n, this->a33 * n, this->a34 * n,
			this->a41 * n, this->a42 * n, this->a43 * n, this->a44 * n

		);

	};

	vec4 operator*(const vec4& vec) const {

		return vec4(

			this->a11 * vec.x + this->a12 * vec.y + this->a13 * vec.z + this->a14 * vec.w,
			this->a21 * vec.x + this->a22 * vec.y + this->a23 * vec.z + this->a24 * vec.w,
			this->a31 * vec.x + this->a32 * vec.y + this->a33 * vec.z + this->a34 * vec.w,
			this->a41 * vec.x + this->a42 * vec.y + this->a43 * vec.z + this->a44 * vec.w

		);

	};

	mat4 transpose() const {

		return mat4(

			this->a11, this->a21, this->a31, this->a41,
			this->a12, this->a22, this->a32, this->a42,
			this->a13, this->a23, this->a33, this->a43,
			this->a14, this->a24, this->a34, this->a44

		);

	};

	mat4 cofactor() const {

		mat4 cofactor;

		cofactor.a11 = mat3(this->a22, this->a23, this->a24, this->a32, this->a33, this->a34, this->a42, this->a43, this->a44).determinate();
		cofactor.a12 = -mat3(this->a21, this->a23, this->a24, this->a31, this->a33, this->a34, this->a41, this->a43, this->a44).determinate();
		cofactor.a13 = mat3(this->a21, this->a22, this->a24, this->a31, this->a32, this->a34, this->a41, this->a42, this->a44).determinate();
		cofactor.a14 = -mat3(this->a21, this->a22, this->a23, this->a31, this->a32, this->a33, this->a41, this->a42, this->a43).determinate();

		cofactor.a21 = -mat3(this->a12, this->a13, this->a14, this->a32, this->a33, this->a34, this->a42, this->a43, this->a44).determinate();
		cofactor.a22 = mat3(this->a11, this->a13, this->a14, this->a31, this->a33, this->a34, this->a41, this->a43, this->a44).determinate();
		cofactor.a23 = -mat3(this->a11, this->a12, this->a14, this->a31, this->a32, this->a34, this->a41, this->a42, this->a44).determinate();
		cofactor.a24 = mat3(this->a11, this->a12, this->a13, this->a31, this->a32, this->a33, this->a41, this->a42, this->a43).determinate();

		cofactor.a31 = mat3(this->a12, this->a13, this->a14, this->a22, this->a23, this->a24, this->a42, this->a43, this->a44).determinate();
		cofactor.a32 = -mat3(this->a11, this->a13, this->a14, this->a21, this->a23, this->a24, this->a41, this->a43, this->a44).determinate();
		cofactor.a33 = mat3(this->a11, this->a12, this->a14, this->a21, this->a22, this->a24, this->a41, this->a42, this->a44).determinate();
		cofactor.a34 = -mat3(this->a11, this->a12, this->a13, this->a21, this->a22, this->a23, this->a41, this->a42, this->a43).determinate();

		cofactor.a41 = -mat3(this->a12, this->a13, this->a14, this->a22, this->a23, this->a24, this->a32, this->a33, this->a34).determinate();
		cofactor.a42 = mat3(this->a11, this->a13, this->a14, this->a21, this->a23, this->a24, this->a31, this->a33, this->a34).determinate();
		cofactor.a43 = -mat3(this->a11, this->a12, this->a14, this->a21, this->a22, this->a24, this->a31, this->a32, this->a34).determinate();
		cofactor.a44 = mat3(this->a11, this->a12, this->a13, this->a21, this->a22, this->a23, this->a31, this->a32, this->a33).determinate();

		return cofactor;

	};

	float determinate() const {

		return   this->a11 * mat3(this->a22, this->a23, this->a24, this->a32, this->a33, this->a34, this->a42, this->a43, this->a44).determinate()
			   - this->a12 * mat3(this->a21, this->a23, this->a24, this->a31, this->a33, this->a34, this->a41, this->a43, this->a44).determinate()
			   + this->a13 * mat3(this->a21, this->a22, this->a24, this->a31, this->a32, this->a34, this->a41, this->a42, this->a44).determinate()
			   - this->a14 * mat3(this->a21, this->a22, this->a23, this->a31, this->a32, this->a33, this->a41, this->a42, this->a43).determinate();

	};

	mat4 inverse() const {

		float det = this->determinate();
		if (det == 0) {

			std::cerr << "Matrix is singular and cannot be inverted\n";
			return mat4(); // Return a zero matrix

		};

		mat4 adjugate = this->cofactor().transpose();
		return adjugate * (1.0f / det);

	};

	std::vector<float> to_std_vector() {

		return {

			this->a11, this->a12, this->a13, this->a14,
			this->a21, this->a22, this->a23, this->a24,
			this->a31, this->a32, this->a33, this->a34,
			this->a41, this->a42, this->a43, this->a44,

		};

	};

	std::vector<float> to_GL() const {

		return this->transpose().to_std_vector();

	};

};

static float to_radians(const float& degree) {

	return degree * 3.14159265359f / 180.0f;

};

//2D
static mat3 create_translation_matrix(const float& x, const float& y) {

	return mat3(

		1.0f, 0.0f, x,
		0.0f, 1.0f, y,
		0.0f, 0.0f, 1.0f

	);

};

static mat3 create_translation_matrix(const vec2& translation_vector) {//@override

	return mat3(

		1.0f, 0.0f, translation_vector.x,
		0.0f, 1.0f, translation_vector.y,
		0.0f, 0.0f, 1.0f

	);

};

static mat3 create_scale_matrix(const float& x, const float& y) {

	return mat3(

		x, 0.0f, 0.0f,
		0.0f, y, 0.0f,
		0.0f, 0.0f, 1.0f

	);

};

static mat3 create_scale_matrix(const vec2& scale_vector) {//@override

	return mat3(

		scale_vector.x, 0.0f, 0.0f,
		0.0f, scale_vector.y, 0.0f,
		0.0f, 0.0f, 1.0f

	);

};

static mat3 create_rotation_matrix(const float& angle) {

	float alpha = to_radians(angle);

	return mat3(

		std::cos(alpha), -std::sin(alpha), 0.0f,
		std::sin(alpha), std::cos(alpha), 0.0f,
		0.0f, 0.0f, 1.0f

	);

};

static mat3 create_model_transformation_matrix(const float& Tx, const float& Ty, const float& Sx, const float& Sy, const float& alpha) {

	return create_rotation_matrix(alpha) * create_scale_matrix(Sx, Sy) * create_translation_matrix(Tx, Ty);

};

static mat3 create_model_transformation_matrix(const vec2& T, const vec2& S, const float& alpha) {//@override

	return create_rotation_matrix(alpha) * create_scale_matrix(S) * create_translation_matrix(T);

};

static mat3 create_model_transformation_matrix(const mat3& T, const mat3& S, const mat3& R) {//@override

	return R * S * T;

};

//3D
static mat4 create_translation_matrix(const float& x, const float& y, const float& z) {

	return mat4(

		1.0f, 0.0f, 0.0f, x,
		0.0f, 1.0f, 0.0f, y,
		0.0f, 0.0f, 1.0f, z,
		0.0f, 0.0f, 0.0f, 1.0f

	);

};

static mat4 create_translation_matrix(const vec3& translation_vector) {//@override

	return mat4(

		1.0f, 0.0f, 0.0f, translation_vector.x,
		0.0f, 1.0f, 0.0f, translation_vector.y,
		0.0f, 0.0f, 1.0f, translation_vector.z,
		0.0f, 0.0f, 0.0f, 1.0f

	);

};

static mat4 create_scale_matrix(const float& x, const float& y, const float& z) {

	return mat4(

		x, 0.0f, 0.0f, 0.0f,
		0.0f, y, 0.0f, 0.0f,
		0.0f, 0.0f, z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f

	);

};

static mat4 create_scale_matrix(const vec3& scale_vector) {//@override

	return mat4(

		scale_vector.x, 0.0f, 0.0f, 0.0f,
		0.0f, scale_vector.y, 0.0f, 0.0f,
		0.0f, 0.0f, scale_vector.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f

	);

};

static mat4 create_X_rotation_matrix(const float& angle) {

	float alpha = to_radians(angle);

	return mat4(

		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, std::cos(alpha), std::sin(alpha), 0.0f,
		0.0f, -std::sin(alpha), std::cos(alpha), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f

	);

};

static mat4 create_Y_rotation_matrix(const float& angle) {

	float alpha = to_radians(angle);

	return mat4(

		std::cos(alpha), 0.0f, -std::sin(alpha), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		std::sin(alpha), 0.0f, std::cos(alpha), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f

	);

};

static mat4 create_Z_rotation_matrix(const float& angle) {

	float alpha = to_radians(angle);

	return mat4(

		std::cos(alpha), std::sin(alpha), 0.0f, 0.0f,
		-std::sin(alpha), std::cos(alpha), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f

	);

};

static mat4 create_rotation_matrix(const float& alpha, const float& theta, const float& delta) {

	return create_X_rotation_matrix(alpha) * create_Y_rotation_matrix(theta) * create_Z_rotation_matrix(delta);

};

static mat4 create_rotation_matrix(const vec3& rotation_vector) {//@override

	return create_rotation_matrix(rotation_vector.x, rotation_vector.y, rotation_vector.z);

};

static mat4 create_model_transformation_matrix(const vec3& T, const vec3& S, const vec3& R) {

	//the rotation is done around origin
	return create_translation_matrix(T) * create_rotation_matrix(R) * create_scale_matrix(S) * create_translation_matrix(0.0f, 0.0f, 0.0f);

};

static mat4 create_model_transformation_matrix(const mat4& T, const mat4& S, const mat4& R) {//@override

	return T * R * S * create_translation_matrix(0.0f, 0.0f, 0.0f);

};

//3D SCREEN TRANSFORMATION
static mat4 create_view_matrix(const vec3& camera_position, const vec3& target_position, const vec3& up_vector) {//also known as lookAtMatrix

	vec3 forward = (camera_position - target_position).normalize();
	vec3 right = up_vector.cross(forward).normalize();
	vec3 up = forward.cross(right).normalize();

	return mat4(

		right.x, right.y, right.z, -right.dot(camera_position),
		up.x, up.y, up.z, -up.dot(camera_position),
		-forward.x, -forward.y, -forward.z, forward.dot(camera_position),
		0.0f, 0.0f, 0.0f, 1.0f

	);

};

static mat4 create_orthographic_projection_matrix(const vec2& screen_size, const float& near, const float& far, const float& orthogonal_size) {

	float aspect_ratio = screen_size.x / screen_size.y;
	float left = -orthogonal_size * aspect_ratio;
	float right = orthogonal_size * aspect_ratio;
	float bottom = -orthogonal_size;
	float top = orthogonal_size;

	return mat4(

		2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
		0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
		0.0f, 0.0f, -2.0f / (far - near), -(far + near) / (far - near),
		0.0f, 0.0f, 0.0f, 1.0f

	);

};

static mat4 create_frustum_projection_matrix(const float& FOV, const vec2& screen_size, const float& near, const float& far) {

	float view_angle = to_radians(FOV);
	float aspect_ratio = screen_size.x / screen_size.y;
	float alpha = std::tan(view_angle / 2.0f);
	float top = near * alpha;
	float bottom = -top;
	float right = top * aspect_ratio;
	float left = -right;

	return mat4(

		(2.0f * near) / (right - left), 0.0f, (right + left) / (right - left), 0.0f,
		0.0f, (2.0f * near) / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
		0.0f, 0.0f, -(far + near) / (far - near), (-2.0f * far * near) / (far - near), //(-far - near) / (far - near)?
		0.0f, 0.0f, -1.0f, 0.0f

	);

};

//in openGL we dont require a view port matrix since openGL transforms our coordinates automatically to NDC with its own static view port matrix, we just need to use the function glViewport() at start of program initialization. Moreover if our Projection Matrix handles FOV we can simply use the static view port matrix
static mat4 create_view_port_matrix(const float& fov, const float& screen_width, const float& screen_height, const float& near, const float& far) {

	float view_angle = to_radians(fov);
	float aspect_ratio = screen_width / screen_height;
	float alpha = tan(view_angle / 2.0f);
	float top = near * alpha;
	float bottom = -top;
	float right = near * aspect_ratio * alpha;
	float left = -right;

	return mat4(

		screen_width / 2.0f, 0.0f, 0.0f, left + (screen_width / 2.0f),
		0.0f, screen_height / 2.0f, 0.0f, bottom + (screen_height / 2.0f),
		0.0f, 0.0f, (far - near) / 2.0f, (far + near) / 2.0f,
		0.0f, 0.0f, 0.0f, 1.0f

	);

};

//openGL uses this matrix implementation as its view port. NOTE: the inverse of this matrix transforms screen coordinates into NDC; HOWEVER, since GLFW has its (0,0) coordiates as top left corner on screen and y-axis scale downwards as we actually go up the screen, we can use the inverse of this matrix to get back NDC coords whilst using glfw
static mat4 create_static_view_port_matrix(const float& screen_width, const float& screen_height) {

	return mat4(

		screen_width / 2.0f, 0.0f, 0.0f, screen_width / 2.0f,
		0.0f, screen_height / 2.0f, 0.0f, screen_height / 2.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f

	);

};

//this matrix solves the issue of the viewport.inverse combined with GLFW
static mat4 create_static_screen_to_NDC_matrix(const float& screen_width, const float& screen_height) {

	return mat4(

		-2.0f / screen_width, 0.0f, 0.0f, -1.0f,
		0.0f, -2.0f / screen_height, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f

	);

};

static mat4 create_model_view_matrix(const mat4& model_transformation_matrix, const mat4& look_at_matrix) {

	return look_at_matrix * model_transformation_matrix;

};

static mat4 create_model_view_projection_matrix(const mat4& model_transformation_matrix, const mat4& look_at_matrix, const mat4& frustum_matrix) {

	return frustum_matrix * create_model_view_matrix(model_transformation_matrix, look_at_matrix);

};

static float get_area_of_circle(const float& radius) {

	return radius * radius * 3.14159265359f;

};

static float get_determinant(const vec2& A, const vec2& B) {

	float a = A.x * B.y;
	float b = A.y * B.x;

	return a - b;

};

static float get_area_of_triangle(const vec2& a, const vec2& b, const vec2& c) {

	vec2 ab = b - a;
	vec2 ac = c - a;

	return 0.5f * get_determinant(ab, ac);

};

static float get_area_of_2D_surface(const std::vector<vec3>& positions, const vec3& center) {

	float total_area = 0.0f;
	for (int i = 0; i < positions.size(); ++i) {

		int next_index = (i + 1) % positions.size();

		vec2 A(positions[i].x, positions[i].y);
		vec2 B(center.x, center.y);
		vec2 C(positions[next_index].x, positions[next_index].y);

		total_area += get_area_of_triangle(A, B, C);

	};

	return total_area;

};

//openGL calculates these automatically
static std::vector<float> get_barycentric_coordinates(const vec3& point, const vec3& A, const vec3& B, const vec3& C) {

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

};

static float clamp(const float& value, const float& min, const float& max) {

	if (value > max) {

		return max;

	}
	else if (value < min) {

		return min;

	}
	else {

		return value;

	};

};

static float get_decimal(const float& number) {

	int n = number;
	return number - n;

};

static std::pair<vec2, vec2> get_min_max(const std::vector<vec2>& data_set) {

	vec2 min_bound(FLT_MAX, FLT_MAX);
	vec2 max_bound(-FLT_MAX, -FLT_MAX);

	for (const vec2& position : data_set) {

		min_bound.x = std::min(min_bound.x, position.x);
		min_bound.y = std::min(min_bound.y, position.y);

		max_bound.x = std::max(max_bound.x, position.x);
		max_bound.y = std::max(max_bound.y, position.y);

	};

	return std::make_pair(min_bound, max_bound);

};

static std::pair<vec3, vec3> get_min_max(const std::vector<vec3>& data_set) {

	vec3 min_bound(FLT_MAX, FLT_MAX, FLT_MAX);
	vec3 max_bound(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (const vec3& position : data_set) {

		min_bound.x = std::min(min_bound.x, position.x);
		min_bound.y = std::min(min_bound.y, position.y);
		min_bound.z = std::min(min_bound.z, position.z);

		max_bound.x = std::max(max_bound.x, position.x);
		max_bound.y = std::max(max_bound.y, position.y);
		max_bound.z = std::max(max_bound.z, position.z);

	};

	return std::make_pair(min_bound, max_bound);

};

static std::pair<vec4, vec4> get_min_max(const std::vector<vec4>& data_set) {

	vec4 min_bound(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
	vec4 max_bound(-FLT_MAX, -FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (const vec4& position : data_set) {

		min_bound.x = std::min(min_bound.x, position.x);
		min_bound.y = std::min(min_bound.y, position.y);
		min_bound.z = std::min(min_bound.z, position.z);
		min_bound.w = std::min(min_bound.w, position.w);

		max_bound.x = std::max(max_bound.x, position.x);
		max_bound.y = std::max(max_bound.y, position.y);
		max_bound.z = std::max(max_bound.z, position.z);
		max_bound.w = std::max(max_bound.w, position.w);

	};

	return std::make_pair(min_bound, max_bound);

};

//these are hashing structs so the vectors can be used easily in maps. Moreover, we cast our float to unint32_t to evade flaoting point precision issues that might arise form arithmatic operations
static std::size_t hash_combine(std::size_t seed, std::size_t value) { return seed ^ (value + 0x9e3779b97f4a7c15 + (seed << 6) + (seed >> 2)); };
struct vec2_hasher {

	std::size_t operator()(const vec2& vec) const {

		std::size_t h1 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(vec.x));
		std::size_t h2 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(vec.y));

		std::size_t seed = h1;
		seed = hash_combine(seed, h2);
		return seed;

	};

};

struct vec3_hasher {

	std::size_t operator()(const vec3& vec) const {

		std::size_t h1 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(vec.x));
		std::size_t h2 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(vec.y));
		std::size_t h3 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(vec.z));

		std::size_t seed = h1;
		seed = hash_combine(seed, h2);
		seed = hash_combine(seed, h3);
		return seed;

	};

};

struct vec4_hasher {

	std::size_t operator()(const vec4& vec) const {

		std::size_t h1 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(vec.x));
		std::size_t h2 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(vec.y));
		std::size_t h3 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(vec.z));
		std::size_t h4 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(vec.w));

		std::size_t seed = h1;
		seed = hash_combine(seed, h2);
		seed = hash_combine(seed, h3);
		seed = hash_combine(seed, h4);
		return seed;

	};

};

struct mat3_hasher {

	std::size_t operator()(const mat3& matrix) const {

        std::size_t h1 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a11));
		std::size_t h2 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a12));
		std::size_t h3 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a13));

		std::size_t h4 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a21));
		std::size_t h5 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a22));
		std::size_t h6 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a23));

		std::size_t h7 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a31));
		std::size_t h8 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a32));
		std::size_t h9 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a33));
		
		std::size_t seed = h1;
		seed = hash_combine(seed, h2);
		seed = hash_combine(seed, h3);
		seed = hash_combine(seed, h4);
		seed = hash_combine(seed, h5);
		seed = hash_combine(seed, h6);
		seed = hash_combine(seed, h7);
		seed = hash_combine(seed, h8);
		seed = hash_combine(seed, h9);
		return seed;

	};

};

struct mat4_hasher {

	std::size_t operator()(const mat4& matrix) const {

		std::size_t h1 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a11));
		std::size_t h2 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a12));
		std::size_t h3 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a13));
		std::size_t h4 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a14));

		std::size_t h5 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a21));
		std::size_t h6 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a22));
		std::size_t h7 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a23));
		std::size_t h8 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a24));

		std::size_t h9 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a31));
		std::size_t h10 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a32));
		std::size_t h11 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a33));
		std::size_t h12 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a34));

		std::size_t h13 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a41));
		std::size_t h14 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a42));
		std::size_t h15 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a43));
		std::size_t h16 = std::hash<uint32_t>()(std::bit_cast<uint32_t>(matrix.a44));

		std::size_t seed = h1;
		seed = hash_combine(seed, h2);
		seed = hash_combine(seed, h3);
		seed = hash_combine(seed, h4);
		seed = hash_combine(seed, h5);
		seed = hash_combine(seed, h6);
		seed = hash_combine(seed, h7);
		seed = hash_combine(seed, h8);
		seed = hash_combine(seed, h9);
		seed = hash_combine(seed, h10);
		seed = hash_combine(seed, h11);
		seed = hash_combine(seed, h12);
		seed = hash_combine(seed, h13);
		seed = hash_combine(seed, h14);
		seed = hash_combine(seed, h15);
		seed = hash_combine(seed, h16);
		return seed;

	};

};

//THIS FUNCTION SHOULD ONLY BE USED AS A SHORTCUT TO EMPLACE ALL COMPONENTS OF A VEC2 AT ONCE DIRECTLY INTO AN STD::VECTOR. EXAMPLE AN STD::VECTOR<FLOAT>
template <typename T>
static void emplace_back(const vec2& vec, std::vector<T>& storage) {

	storage.emplace_back(vec.x);
	storage.emplace_back(vec.y);

};

//THIS FUNCTION SHOULD ONLY BE USED AS A SHORTCUT TO EMPLACE ALL COMPONENTS OF A VEC3 AT ONCE DIRECTLY INTO AN STD::VECTOR. EXAMPLE AN STD::VECTOR<FLOAT>
template <typename T>
static void emplace_back(const vec3& vec, std::vector<T>& storage) {

	storage.emplace_back(vec.x);
	storage.emplace_back(vec.y);
	storage.emplace_back(vec.z);

};

static void print_vec(const vec2& vec) {

	std::cout << vec.x << ", " << vec.y << "\n";

};
static void print_vec(const vec3& vec) {

	std::cout << vec.x << ", " << vec.y << ", " << vec.z << "\n";

};
static void print_vec(const vec4& vec) {

	std::cout << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << "\n";

};