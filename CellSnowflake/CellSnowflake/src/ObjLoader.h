#pragma once

#include <string>
#include <GL/glew.h>
#include "Vector.h"

struct ObjVec3{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	ObjVec3(float nx = 0.0f, float ny = 0.0f, float nz = 0.0f);
	operator float* ();
	operator const float* () const;
};

struct ObjVertex {
	Vector2 texcoord;
	Vector3 normal;
	Vector3 position;
	ObjVertex() {}
};

struct ObjMaterial {
	std::string name;
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	GLfloat shininess;
	GLfloat alpha;

	std::string ambientMapName;
	std::string diffuseMapName;
	std::string specularMapName;
	std::string bumpMapName;
	ObjMaterial() {}
};

struct ObjSubset {
	unsigned int materialIndex;
	unsigned int faceStart;
	unsigned int faceCount;
	ObjSubset() {}
};

struct ObjBoundingBox {
	Vector3 max;
	Vector3 min;
	Vector3 size;
	void merge(Vector3 v);
	ObjBoundingBox() {}
	ObjBoundingBox(Vector3 v);
};

class ObjMesh {
public:
	ObjMesh() {}
	~ObjMesh();

	bool loadFile(const char* fileName);
	void release();
	void draw();

	int verticesNum();
	int subsetsNum();
	int materialsNum();
	int indicesNum();
	int indexData(int index);
	int* indices();

	ObjVertex GetVertex(int index);
	ObjVertex* GetVertices();
	ObjSubset GetSubset(int index);
	ObjSubset* GetSubsets();
	ObjMaterial GetMaterial(int index);
	ObjMaterial* GetMaterials();
	ObjBoundingBox GetBoundingBox();

private:
	ObjVertex* mVertices = nullptr;
	ObjSubset* mSubsets = nullptr;
	ObjMaterial* mMaterials = nullptr;
	int* mIndices = nullptr;

	int mVerticesNum = 0;
	int mSubsetsNum = 0;
	int mMateerialsNum = 0;
	int mIndicesNum= 0;

	ObjBoundingBox mBoundingBox;
	std::string mDirectryPath;

	bool loadMTLFile(const char* fileName);
	bool loadOBJFile(const char* fileName);
};