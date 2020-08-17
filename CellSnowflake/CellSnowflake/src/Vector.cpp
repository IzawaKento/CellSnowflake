#include "Vector.h"

//グローバルの関数

//const参照渡しに変更して高速化してる
Vector2 operator-(const Vector2 &v1, const Vector2 &v2)
{
	Vector2 v{ v1[0] - v2[0], v1[1] - v2[1] };
	return v;
}

Vector2 operator+(const Vector2 &v1, const Vector2 &v2)
{
	Vector2 v{ v1[0] + v2[0], v1[1] + v2[1] };
	return v;
}

//スカラー
Vector2 operator*(const Vector2 &v1, const GLfloat f)
{
	Vector2 v{ v1[0] * f, v1[1] * f };
	return v;
}

Vector2 operator/(const Vector2 &v1, const GLfloat f)
{
	//ソフトウェアじゃないから0割り回避はいいかな
	Vector2 v{ v1[0] / f, v1[1] / f };
	return v;
}
//長さ
GLfloat LengthVec2(const Vector2 &v) {

	return sqrt(v[0] * v[0] + v[1] * v[1]);
}
//長さの2乗の値
GLfloat SquareLenVec2(const Vector2 &v) {
	return v[0] * v[0] + v[1] * v[1];
}
//正規化
Vector2 NormalizeVec2(const Vector2 &v) {
	return v / LengthVec2(v);
}

//const参照渡しに変更して高速化してる
Vector3 operator-(const Vector3 &v1, const Vector3 &v2)
{
	Vector3 v{ v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2] };
	return v;
}

Vector3 operator+(const Vector3 &v1, const Vector3 &v2)
{
	Vector3 v{ v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2] };
	return v;
}

//スカラー
Vector3 operator*(const Vector3 &v1, const GLfloat f)
{
	Vector3 v{ v1[0] * f, v1[1] * f, v1[2] * f };
	return v;
}

Vector3 operator/(const Vector3 &v1, const GLfloat f)
{
	//ソフトウェアじゃないから0割り回避はいいかな
	Vector3 v{ v1[0] / f, v1[1] / f, v1[2] / f };
	return v;
}
//長さ
GLfloat LengthVec3(const Vector3 &v) {

	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}
//長さの2乗の値
GLfloat SquareLenVec3(const Vector3 &v) {
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}
//正規化
Vector3 NormalizeVec3(const Vector3 &v) {
	return v / LengthVec3(v);
}

//外積
Vector3 CrossVec3(const Vector3& v1, const Vector3& v2) {
	Vector3 v{
		v1[1] * v2[2] - v1[2] * v2[1],
		v1[2] * v2[0] - v1[0] * v2[2],
		v1[0] * v2[1] - v1[1] * v2[0]
	};
	return v;
}


//なぜか中で変数宣言したらまずい気がするのでvoid
void GLfloatToVec3(Vector3& v, const GLfloat* ptr) {
	v[0] = ptr[0];
	v[1] = ptr[1];
	v[2] = ptr[2];
}

//Vector4//
Vector4 operator-(const Vector4 v1, const Vector4 v2)
{
	Vector4 v;
	v[0] = v1[0] - v2[0];
	v[1] = v1[1] - v2[1];
	v[2] = v1[2] - v2[2];
	v[3] = v1[3] - v2[3];
	return v;
}
Vector4 operator+(const Vector4 v1, const Vector4 v2)
{
	Vector4 v;
	v[0] = v1[0] + v2[0];
	v[1] = v1[1] + v2[1];
	v[2] = v1[2] + v2[2];
	v[3] = v1[3] + v2[3];
	return v;
}

//スカラー
Vector4 operator*(const Vector4 v1, const GLfloat f)
{
	Vector4 v;
	v[0] = v1[0] * f;
	v[1] = v1[1] * f;
	v[2] = v1[2] * f;
	v[3] = v1[3] * f;
	return v;
}

Vector4 operator/(const Vector4 v1, const GLfloat f)
{
	if (f == 0) {
		std::cerr << "Vec4: 0割り" << std::endl;
		return v1;
	}
	Vector4 v;
	v[0] = v1[0] / f;
	v[1] = v1[1] / f;
	v[2] = v1[2] / f;
	v[3] = v1[3] / f;
	return v;
}
//長さ?
float LengthVec4(const Vector4 v) {

	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
}
//正規化?
Vector4 NormalizeVec4(const Vector4 v) {
	return v / LengthVec4(v);
}

Vector3 Vec4To3(const Vector4 v) {
	Vector3 v3;
	if (v[3] == 0) {
		std::cerr << "Vec4to3: 0割り" << std::endl;
		v3[0] = v[0];
		v3[1] = v[1];
		v3[2] = v[2];
	}
	else {
		v3[0] = v[0] / v[3];
		v3[1] = v[1] / v[3];
		v3[2] = v[2] / v[3];
	}

	return v3;
}