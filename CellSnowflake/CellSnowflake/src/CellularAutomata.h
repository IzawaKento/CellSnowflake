#pragma once
#include <array>
#include <GL/glew.h>
#include "Program.h"
#include "MyMath.h"

class Cell;

class CellularAutomata {
public:
	//CellularAutomata() {};
	CellularAutomata(GLuint vfProgObj, const int gridNumX, const int gridNumY, const int gridNumZ);
	~CellularAutomata();

	void setInitialCells(int centerNum);

	void copySSBO(GLuint readBuffer, GLuint writeBuffer);

	void DispatchCompute();
	
	//�Z���𒸓_�Ƃ��ĕ`��
	void drawCell();

	//�Z��������
	void initialize();

	void setUniforms(GLfloat rho);
	
	//�֐����Ń����o����������g�����������Ȃ�
	const float getInitRho() const {
		return mInitRho;
	}
	const int gridNumX() const {
		return mGridNumX;
	}
	const int gridNumY() const {
		return mGridNumY;
	}
	const int gridNumZ() const {
		return mGridNumZ;
	}
	//�g���ĂȂ���
	const int getCellNumX(int i) {
		return (i % (mGridNumX * mGridNumZ)) % mGridNumX;
	}
	const int getCellNumY(int i) {
		return i / (mGridNumX * mGridNumZ);
	}
	const int getCellNumZ(int i) {
		return (i % (mGridNumX * mGridNumZ)) / mGridNumX;
	}
	const GLuint getSsbo() const{
		return ssbo;
	}
	//glGetUniformLocation�̂���
	const GLuint getDiffusion1ComProgObj() {
		return diffusion1ComProgObj;
	}

	//�Ă���
	Cell* GetCells() { return cells; }

	//��Cell�N���X����̈ڍs	boundary���C�̂ƊԈႦ�Ă��܂���
	enum CellFlags {
		ISCRYSTAL = 1,				//����snowflake�ɂ�����
		ISEDGECRYSTAL = (1 << 1),	//edge_snowflake
		ISBOUNDARY = (1 << 2),		//non_boundary
		ISEDGEBOUNDARY = (1 << 3),	//boundary
		MZISODD = (1 << 4),			//6�p�`�Z�����̓A�N�Z�X�p
		ISENDOFCELLS = (1 << 5),
	};
private:

	//�p�����[�^
	//�����g�U����
	const float mInitRho = 0.2f;
	//�Z���I�[�g�}�g���O���b�h�T�C�Y
	const int mGridNumX = 30, mGridNumY = 40, mGridNumZ = 30;
	const float mCellSizeX = 0.03f;
	const float mCellSizeZ = mCellSizeX * sin(60.0f * MyMath::pi / 180.0f);		//��3 / 2
	const float mCellSizeY = mCellSizeX * 1.10013f;						//c : a = 1.10013 : 1
	
	//�Z���擪�|�C���^
	Cell* cells;

	//�Z�������E�����ɂ���
	void SetEdgeCry(int);
	//�Z�����������Ɏg�p
	void set2RadiusCells(int centerNum, int yOffset);
	void set3RadiusCells(int centerNum, int yOffset);

	//�Z���`��p
	GLuint vao;

	//�o�b�t�@�Q������
	//�V�F�[�_�[�X�g���[�W�o�b�t�@�I�u�W�F�N�g
	GLuint ssbo;
	//�X�V�f�[�^�������ݗpSSBO�A�X�V�な�Ƀo�b�t�@���R�s�[����
	GLuint tmpSsbo;		//���O����ł����񂩂Ȃ�
	//��Ėʐ����p�A�g�~�b�N�J�E���^�[�o�b�t�@�I�u�W�F�N�g
	GLuint acbo;

	//vgf
	const GLuint mvfProgObj;

	//�R���s���[�g�V�F�[�_�[�p�v���O�����I�u�W�F�N�g
	GLuint resetEdgeComProgObj = Program::loadCompProgramObj("src\\resetEdge.comp");
	GLuint computeProgramObj = Program::loadCompProgramObj("src\\compute.comp");
	GLuint neighbourCryNumComProgObj = Program::loadCompProgramObj("src\\neighbourCryNum.comp");
	GLuint diffusion1ComProgObj = Program::loadCompProgramObj("src\\diffusionFast.comp");
	GLuint diffusion2ComProgObj = Program::loadCompProgramObj("src\\diffusion2.comp");
	GLuint freezingComProgObj = Program::loadCompProgramObj("src\\freezing.comp");
	GLuint attachmentComProgObj = Program::loadCompProgramObj("src\\attachment.comp");
	GLuint meltingComProgObj = Program::loadCompProgramObj("src\\melt.comp");

	GLuint testComProgObj = Program::loadCompProgramObj("src\\test.comp");

	//���j�t�H�[���ϐ��ꏊ
	const GLint rhoLoc = glGetUniformLocation(diffusion1ComProgObj, "rho");
};