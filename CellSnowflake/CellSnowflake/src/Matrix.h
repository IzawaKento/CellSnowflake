#pragma once
#include <cmath>
#include <algorithm>
#include <array>
#include <stack>
#include <GL/glew.h>
#include "Vector.h"

class Matrix {
private:
	GLfloat matrix[16];

public:
	//�f�o�b�O�p�֐�
	void printMat() {
		std::cout << matrix[0] << "," << matrix[4] << "," << matrix[8] << "," << matrix[12] << "," << std::endl;
		std::cout << matrix[1] << "," << matrix[5] << "," << matrix[9] << "," << matrix[13] << "," << std::endl;
		std::cout << matrix[2] << "," << matrix[6] << "," << matrix[10] << "," << matrix[14] << "," << std::endl;
		std::cout << matrix[3] << "," << matrix[7] << "," << matrix[11] << "," << matrix[15] << "," << std::endl;
	}

	// �R���X�g���N�^
	Matrix() {}
	// �z��̓��e�ŏ���������R���X�g���N�^
	// a: GLfloat �^�� 16 �v�f�̔z��
	Matrix(const GLfloat *a)
	{
		std::copy(a, a + 16, matrix);
	}

	// �ϊ��s��̔z���Ԃ�
	const GLfloat* data() const
	{
		return matrix;
	}

	void load(const Matrix& m) {
		//�Ƃ肠�����l��for�œ��ꂽ
		for (int i = 0; i < 16; ++i) {
			matrix[i] = m.data()[i];
		}
	}

	Vector4 multiplyVec4(Vector4 v) const{
		Vector4 t;
		for (int i = 0; i < 4; ++i)
		{
			t[i] =
				matrix[0 + i] * v[0] +
				matrix[4 + i] * v[1] +
				matrix[8 + i] * v[2] +
				matrix[12 + i] * v[3];
		}
		return t;
	}

	//// �@���x�N�g���̕ϊ��s������߂�
	//void getNormalMatrix(GLfloat *m) const
	//{
	//	m[0] = matrix[5] * matrix[10] - matrix[6] * matrix[9];
	//	m[1] = matrix[6] * matrix[8] - matrix[4] * matrix[10];
	//	m[2] = matrix[4] * matrix[9] - matrix[5] * matrix[8];
	//	m[3] = matrix[9] * matrix[2] - matrix[10] * matrix[1];
	//	m[4] = matrix[10] * matrix[0] - matrix[8] * matrix[2];
	//	m[5] = matrix[8] * matrix[1] - matrix[9] * matrix[0];
	//	m[6] = matrix[1] * matrix[6] - matrix[2] * matrix[5];
	//	m[7] = matrix[2] * matrix[4] - matrix[0] * matrix[6];
	//	m[8] = matrix[0] * matrix[5] - matrix[1] * matrix[4];
	//}

	// �P�ʍs���ݒ肷��
	void loadIdentity()
	{
		std::fill(matrix, matrix + 16, 0.0f);
		matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1.0f;
	}

	// �P�ʍs����쐬����
	static Matrix identity()
	{
		Matrix t;
		t.loadIdentity();
		return t;
	}

	// (x, y, z) �������s�ړ�����ϊ��s����쐬����
	static Matrix translate(Vector3 dir)
	{
		Matrix t;
		t.loadIdentity();
		t.matrix[12] = dir[0];
		t.matrix[13] = dir[1];
		t.matrix[14] = dir[2];
		return t;
	}

	// (x, y, z) �{�Ɋg��k������ϊ��s����쐬����
	static Matrix scale(GLfloat x, GLfloat y, GLfloat z)
	{
		Matrix t;
		t.loadIdentity();
		t.matrix[0] = x;
		t.matrix[5] = y;
		t.matrix[10] = z;
		return t;
	}

	// (x, y, z) ������ a ��]����ϊ��s����쐬����
	static Matrix rotate(GLfloat a, Vector3 axis)
	{
		Matrix t;
		const GLfloat d(LengthVec3(axis));
		if (d > 0.0f)
		{
			const GLfloat l(axis[0] / d), m(axis[1] / d), n(axis[2] / d);
			const GLfloat l2(l * l), m2(m * m), n2(n * n);
			const GLfloat lm(l * m), mn(m * n), nl(n * l);
			const GLfloat c(cos(a)), c1(1.0f - c), s(sin(a));
			t.loadIdentity();
			t.matrix[0] = (1.0f - l2) * c + l2;
			t.matrix[1] = lm * c1 + n * s;
			t.matrix[2] = nl * c1 - m * s;
			t.matrix[4] = lm * c1 - n * s;
			t.matrix[5] = (1.0f - m2) * c + m2;
			t.matrix[6] = mn * c1 + l * s;
			t.matrix[8] = nl * c1 + m * s;
			t.matrix[9] = mn * c1 - l * s;
			t.matrix[10] = (1.0f - n2) * c + n2;
		}
		return t;
	}

	// �r���[�ϊ��s����쐬����
	static Matrix lookat(
		const Vector3 eye, // ���_�̈ʒu
		const Vector3 dest, // �ڕW�_�̈ʒu
		const Vector3 up) // ������̃x�N�g��
	{
		// ���s�ړ��̕ϊ��s��
		const Matrix tv(translate(eye * -1));

		// t �� = e - g
		const Vector3 t(eye - dest);
		// r �� = u x t ��
		const Vector3 r(CrossVec3(up, t));
		// s �� = t �� x r ��
		const Vector3 s(CrossVec3(t, r));

		// s ���̒����̃`�F�b�N
		const GLfloat s2(SquareLenVec3(s));
		if (s2 == 0.0f) return tv;
		// ��]�̕ϊ��s��
		Matrix rv;
		rv.loadIdentity();
		// r ���𐳋K�����Ĕz��ϐ��Ɋi�[
		const GLfloat rDis(LengthVec3(r));
		rv.matrix[0] = r[0] / rDis;
		rv.matrix[4] = r[1] / rDis;
		rv.matrix[8] = r[2] / rDis;
		// s ���𐳋K�����Ĕz��ϐ��Ɋi�[
		const GLfloat sDis(sqrt(s2));
		rv.matrix[1] = s[0] / sDis;
		rv.matrix[5] = s[1] / sDis;
		rv.matrix[9] = s[2] / sDis;
		// t ���𐳋K�����Ĕz��ϐ��Ɋi�[
		const GLfloat tDis(LengthVec3(t));
		rv.matrix[2] = t[0] / tDis;
		rv.matrix[6] = t[1] / tDis;
		rv.matrix[10] = t[2] / tDis;

		// ���_�̕��s�ړ��̕ϊ��s��Ɏ����̉�]�̕ϊ��s����悶��
		return  rv * tv;
	}

	// �t�r���[�ϊ��s����쐬����
	static Matrix window2world(
		const Vector3 eye, // ���_�̈ʒu
		const Vector3 dest, // �ڕW�_�̈ʒu
		const Vector3 up) // ������̃x�N�g��
	{
		
		// ���s�ړ��̕ϊ��s��
		const Matrix tv(translate(eye * -1));

		// t �� = e - g
		const Vector3 t(eye - dest);
		// r �� = u x t ��
		const Vector3 r(CrossVec3(up, t));
		// s �� = t �� x r ��
		const Vector3 s(CrossVec3(t, r));

		// s ���̒����̃`�F�b�N
		const GLfloat s2(SquareLenVec3(s));
		if (s2 == 0.0f) return tv;
		// ��]�̕ϊ��s��
		Matrix rv;
		rv.loadIdentity();
		// r ���𐳋K�����Ĕz��ϐ��Ɋi�[
		const GLfloat rDis(LengthVec3(r));
		rv.matrix[0] = r[0] / rDis;
		rv.matrix[4] = r[1] / rDis;
		rv.matrix[8] = r[2] / rDis;
		// s ���𐳋K�����Ĕz��ϐ��Ɋi�[
		const GLfloat sDis(sqrt(s2));
		rv.matrix[1] = s[0] / sDis;
		rv.matrix[5] = s[1] / sDis;
		rv.matrix[9] = s[2] / sDis;
		// t ���𐳋K�����Ĕz��ϐ��Ɋi�[
		const GLfloat tDis(LengthVec3(t));
		rv.matrix[2] = t[0] / tDis;
		rv.matrix[6] = t[1] / tDis;
		rv.matrix[10] = t[2] / tDis;

		// ���_�̕��s�ړ��̕ϊ��s��Ɏ����̉�]�̕ϊ��s����悶��
		return  rv * tv;
	}

	// ���𓊉e�ϊ��s����쐬����
	static Matrix orthogonal(GLfloat left, GLfloat right,
		GLfloat bottom, GLfloat top,
		GLfloat zNear, GLfloat zFar)
	{
		Matrix t;
		const GLfloat dx(right - left);
		const GLfloat dy(top - bottom);
		const GLfloat dz(zFar - zNear);
		if (dx != 0.0f && dy != 0.0f && dz != 0.0f)
		{
			t.loadIdentity();
			t.matrix[0] = 2.0f / dx;
			t.matrix[5] = 2.0f / dy;
			t.matrix[10] = -2.0f / dz;
			t.matrix[12] = -(right + left) / dx;
			t.matrix[13] = -(top + bottom) / dy;
			t.matrix[14] = -(zFar + zNear) / dz;
		}
		return t;
	}

	// �������e�ϊ��s����쐬����
	static Matrix frustum(GLfloat left, GLfloat right,
		GLfloat bottom, GLfloat top,
		GLfloat zNear, GLfloat zFar)
	{
		Matrix t;
		const GLfloat dx(right - left);
		const GLfloat dy(top - bottom);
		const GLfloat dz(zFar - zNear);
		if (dx != 0.0f && dy != 0.0f && dz != 0.0f)
		{
			t.loadIdentity();
			t.matrix[0] = 2.0f * zNear / dx;
			t.matrix[5] = 2.0f * zNear / dy;
			t.matrix[8] = (right + left) / dx;
			t.matrix[9] = (top + bottom) / dy;
			t.matrix[10] = -(zFar + zNear) / dz;
			t.matrix[11] = -1.0f;
			t.matrix[14] = -2.0f * zFar * zNear / dz;
			t.matrix[15] = 0.0f;
		}
		return t;
	}

	// ��p���w�肵�ē������e�ϊ��s����쐬����
	static Matrix perspective(GLfloat fovy, GLfloat aspect,
		GLfloat zNear, GLfloat zFar)
	{
		Matrix t;
		const GLfloat dz(zFar - zNear);
		if (dz != 0.0f)
		{
			t.loadIdentity();
			t.matrix[5] = 1.0f / tan(fovy * 0.5f);
			t.matrix[0] = t.matrix[5] / aspect;
			t.matrix[10] = -(zFar + zNear) / dz;
			t.matrix[11] = -1.0f;
			t.matrix[14] = -2.0f * zFar * zNear / dz;
			t.matrix[15] = 0.0f;
		}
		return t;
	}

	void projection(const Vector4 v1, Vector3& v2) {
		Vector4 t;
		t = multiplyVec4(v1);
		/*std::cout << "t" << t[0] << " "
			<< t[1] << " "
			<< t[2] << " "
			<< t[3] << std::endl;*/
		v2 = Vec4To3(t);
	}

	// ��Z
	Matrix operator*(const Matrix &m) const
	{
		Matrix t;
		for (int i = 0; i < 16; ++i)
		{
			//0,1,2,3,0,1,2,3..  0,0,0,0,4,4,4,4,8,8,8,8,12,12,12,12
			const int j(i & 3), k(i & ~3);
			t.matrix[i] =
				matrix[0 + j] * m.matrix[k + 0] +
				matrix[4 + j] * m.matrix[k + 1] +
				matrix[8 + j] * m.matrix[k + 2] +
				matrix[12 + j] * m.matrix[k + 3];
		}
		return t;
	}

};