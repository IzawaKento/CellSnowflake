#pragma once
#include <iostream>
#include <array>
#include <GL/glew.h>

//エイリアス宣言
//関数の戻り値に配列を使えないのでarrayで代用?
using Vector3 = std::array<GLfloat, 3>;
using Vector4 = std::array<GLfloat, 4>;
//externなくても動くんやけど、、、
//Vec3
extern Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
extern Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
extern Vector3 operator*(const Vector3 &v1, const float f);
extern Vector3 operator/(const Vector3 &v1, const float f);
extern GLfloat LengthVec3(const Vector3 &v);
extern GLfloat SquareLenVec3(const Vector3 &v);
extern Vector3 NormalizeVec3(const Vector3 &v);
extern Vector3 CrossVec3(const Vector3& v1, const Vector3& v2);
//変換関数
extern void GLfloatToVec3(Vector3& v, const GLfloat* ptr);
//Vec4
extern Vector4 operator-(const Vector4 v1, const Vector4 v2);
extern Vector4 operator+(const Vector4 v1, const Vector4 v2);
extern Vector4 operator*(const Vector4 v1, const float f);
extern Vector4 operator/(const Vector4 v1, const float f);
extern float LengthVec4(const Vector4 v);
extern Vector4 NormalizeVec4(const Vector4 v);
extern Vector3 Vec4To3(const Vector4 v);