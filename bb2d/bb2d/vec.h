#pragma once

#ifndef BB2D_MATH_H
#define BB2D_MATH_H

namespace bb2d {

	namespace math {

		/*
		Low-cost 2D vector with default constructors
		*/
		struct vec2
		{
			//X-position of vector
			float x;
			//Y-position of vector
			float y;

			//Default constructor mixed with x,y component customiser
			vec2(float a = 0, float b = 0) {
				x = a;
				y = b;
			}

			//VEC2 OPERATOR DEFINITION BEGIN -------------------------------
			vec2 operator+(const vec2& b) {
				this->x += b.x;
				this->y += b.y;
				return *this;
			}

			vec2 operator-(const vec2& b) {
				this->x -= b.x;
				this->y -= b.y;
				return *this;
			}

			vec2 operator/(const vec2& b) {
				this->x /= b.x;
				this->y /= b.y;
				return *this;
			}

			vec2 operator*(const vec2& b) {
				this->x *= b.x;
				this->y *= b.y;
				return *this;
			}

			vec2 operator/(const float& b) {
				this->x /= b;
				this->y /= b;
				return *this;
			}

			vec2 operator*(const float& b) {
				this->x *= b;
				this->y *= b;
				return *this;
			}

			vec2 operator%(const int& b) {
				this->x = (int)this->x % b;
				this->y = (int)this->y % b;
				return *this;
			}

			vec2 operator+=(const vec2& b) {
				this->x = x + b.x;
				this->y = y + b.y;
				return *this;
			}

			vec2 operator-=(const vec2& b) {
				this->x = x - b.x;
				this->y = y - b.y;
				return *this;
			}

			vec2 operator*=(const vec2& b) {
				this->x = x * b.x;
				this->y = y * b.y;
				return *this;
			}

			vec2 operator/=(const vec2& b) {
				this->x = x / b.x;
				this->y = y / b.y;
				return *this;
			}

			vec2 operator*=(const float& b) {
				this->x = x * b;
				this->y = y * b;
				return *this;
			}

			vec2 operator/=(const float& b) {
				this->x = x / b;
				this->y = y / b;
				return *this;
			}
			//VEC2 OPERATOR DEFINITION END -----------------------------------
		};

		/*
		Low-cost 3D vector with default constructors
		*/
		struct vec3
		{
			//X-position of vector
			float x;
			//Y-position of vector
			float y;
			//Z-position of vector
			float z;

			//Default constructor mixed with x,y component customiser
			vec3(float a = 0, float b = 0, float c = 0) {
				x = a;
				y = b;
				z = c;
			}

			//Convert vec2 to vec3 constructor
			vec3(vec2 a, float b = 0) {
				x = a.x;
				y = a.y;
				z = b;
			}

			//VEC3 OPERATOR DEFINITION BEGIN -------------------------------
			vec3 operator+(const vec3& b) {
				this->x += b.x;
				this->y += b.y;
				this->z += b.z;
			}

			vec3 operator-(const vec3& b) {
				this->x -= b.x;
				this->y -= b.y;
				this->z -= b.z;
			}

			vec3 operator+(const vec2& b) {
				this->x += b.x;
				this->y += b.y;
			}

			vec3 operator-(const vec2& b) {
				this->x -= b.x;
				this->y -= b.y;
			}

			vec3 operator/(const vec3& b) {
				this->x /= b.x;
				this->y /= b.y;
				this->z /= b.z;
			}

			vec3 operator*(const vec3& b) {
				this->x *= b.x;
				this->y *= b.y;
				this->z *= b.z;
			}

			vec3 operator/(const vec2& b) {
				this->x /= b.x;
				this->y /= b.y;
			}

			vec3 operator*(const vec2& b) {
				this->x *= b.x;
				this->y *= b.y;
			}

			vec3 operator/(const float& b) {
				this->x /= b;
				this->y /= b;
				this->z /= b;
			}

			vec3 operator*(const float& b) {
				this->x *= b;
				this->y *= b;
				this->z *= b;
			}

			vec3 operator%(const int& b) {
				this->x = (int)this->x % b;
				this->y = (int)this->y % b;
				this->z = (int)this->z % b;
			}

			vec3 operator+=(const vec3& b) {
				this->x = x + b.x;
				this->y = y + b.y;
				this->z = z + b.z;
			}

			vec3 operator-=(const vec3& b) {
				this->x = x - b.x;
				this->y = y - b.y;
				this->z = z - b.z;
			}

			vec3 operator*=(const vec3& b) {
				this->x = x * b.x;
				this->y = y * b.y;
				this->z = z + b.z;
			}

			vec3 operator/=(const vec3& b) {
				this->x = x / b.x;
				this->y = y / b.y;
				this->z = z / b.z;
			}

			vec3 operator+=(const vec2& b) {
				this->x = x + b.x;
				this->y = y + b.y;
			}

			vec3 operator-=(const vec2& b) {
				this->x = x - b.x;
				this->y = y - b.y;
			}

			vec3 operator*=(const vec2& b) {
				this->x = x * b.x;
				this->y = y * b.y;
			}

			vec3 operator/=(const vec2& b) {
				this->x = x / b.x;
				this->y = y / b.y;
			}

			vec3 operator*=(const float& b) {
				this->x = x * b;
				this->y = y * b;
				this->z = z * b;
			}

			vec3 operator/=(const float& b) {
				this->x = x / b;
				this->y = y / b;
				this->z = z / b;
			}
			//VEC3 OPERATOR DEFINITION END -----------------------------------
		};

	}

}

#endif