#pragma once
#include <iostream>
#include <array>
#include <GL/glew.h>

//エイリアス宣言
//関数の戻り値に配列を使えないのでarrayで代用?
using Vector2 = std::array<GLfloat, 2>;
using Vector3 = std::array<GLfloat, 3>;
using Vector4 = std::array<GLfloat, 4>;
//externなくても動くんやけど、、、
//Vec2
extern Vector2 operator-(const Vector2 &v1, const Vector2 &v2);
extern Vector2 operator+(const Vector2 &v1, const Vector2 &v2);
extern Vector2 operator*(const Vector2 &v1, const GLfloat f);
extern Vector2 operator/(const Vector2 &v1, const GLfloat f);
extern GLfloat LengthVec2(const Vector2 &v);
extern GLfloat SquareLenVec2(const Vector2 &v);
extern Vector2 NormalizeVec2(const Vector2 &v);

//Vec3
extern Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
extern Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
extern Vector3 operator*(const Vector3 &v1, const GLfloat f);
extern Vector3 operator/(const Vector3 &v1, const GLfloat f);
extern GLfloat LengthVec3(const Vector3 &v);
extern GLfloat SquareLenVec3(const Vector3 &v);
extern Vector3 NormalizeVec3(const Vector3 &v);
extern Vector3 CrossVec3(const Vector3& v1, const Vector3& v2);
//変換関数
extern void GLfloatToVec3(Vector3& v, const GLfloat* ptr);
//Vec4
extern Vector4 operator-(const Vector4 v1, const Vector4 v2);
extern Vector4 operator+(const Vector4 v1, const Vector4 v2);
extern Vector4 operator*(const Vector4 v1, const GLfloat f);
extern Vector4 operator/(const Vector4 v1, const GLfloat f);
extern GLfloat LengthVec4(const Vector4 v);
extern Vector4 NormalizeVec4(const Vector4 v);
extern Vector3 Vec4To3(const Vector4 v);