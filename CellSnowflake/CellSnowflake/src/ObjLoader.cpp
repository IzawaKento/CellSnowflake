#include "ObjLoader.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>

#include "Vector.h"

//文字列からディレクトリを取得
char* getDirectoryPath(const char* fileName)
{
	char *str = nullptr;
	char *dir;
	//ここで生成したメモリを使っていく
	dir = new char[strlen(fileName) + 1];
	strcpy_s(dir, strlen(dir), fileName);
	if (str = strrchr(dir, '/')) str[1] = '\0';
	else if (str = strrchr(dir, '\\')) str[1] = '\0';
	else dir[0] = '\0';

	return dir;
}

//文字列からディレクトリを削除
char* cutDirectoryPath(const char* fileName)
{
	char* base = new char[strlen(fileName) + 1];
	strcpy_s(base, strlen(base), fileName);

	char* s = NULL;
	if (s = strrchr(base, '\\')){
		s++;
		return s;
	}
	else if (s = strrchr(base, '/')){
		s++;
		return s;
	}
	else{
		return base;
	}
}

//ObjBoundingBox

ObjBoundingBox::ObjBoundingBox(Vector3 v) 
: max(v), min(v) {

}

//サイズ更新
void ObjBoundingBox::merge(Vector3 v)
{
	for (int i = 0; i < 3; ++i) {
		max[i] = v[i] > max[i] ? v[i] : max[i];
		min[i] = v[i] < min[i] ? v[i] : min[i];
	}
	size = max - min;
}

//ObjMesh

ObjMesh::~ObjMesh()
{
	release();
};

//メンバ解放
void ObjMesh::release() {
	delete[] mIndices;
	delete[] mMaterials;
	delete[] mSubsets;
	delete[] mVertices;
}

bool ObjMesh::loadOBJFile(const char* fileName) {
	std::ifstream in(fileName, std::ios::in);
	if (!in) {
		std::cerr << "Cannot open " << fileName << std::endl;
	}

	std::string mtlFileName = { 0 };
	std::string buf = { 0 };
	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoord;
	std::vector<ObjVertex> tmpVertices;
	std::vector<ObjSubset> tmpSubsets;
	std::vector<int> tIncices;
	bool initBox = false;
	int prevSize = 0;

	long total = 0;

	ObjMaterial material;
	int dwFaceIndex = 0;
	int dwFaceCount = 0;
	int dwCurrSubset = 0;

	//strcpy_s(mDirectryPath, )
	return false;
}