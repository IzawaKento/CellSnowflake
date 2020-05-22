#version 460 core

in vec4 vColor[];
in uint vFlags[];
in vec3 vNeighSurDir[];
in mat4 vModelview[];
in mat4 vProjection[];
out vec4 gColor;
layout (points) in;
layout (triangle_strip, max_vertices = 9) out;
//layout (points, max_vertices = 1) out;
//フラグの代わり.....
const uint ISCRYSTAL = 1u;
const uint ISEDGECRYSTAL = 2u;
const uint ISBOUNDARY = 4u;
const uint ISEDGEBOUNDARY = 8u;
const uint MZISODD = 16u;	//セル番号のZ値が奇数である
const uint ISENDOFCELLS = 32u;

layout(std140) uniform TriangleConnectionTable{
	ivec4 triangleConnectionTable[1024];
	ivec4 cubeEdgeFlags[64];
	ivec4 edgeConnection[6];
};

//flagIDは2進数の値
//flagsのビットのfragID番目（0から）が1ならtrue

bool isFlag(int i, uint flagID) {
	return ((vFlags[i] & flagID) != 0);
}

void emitAdditiveVert(int i, vec3 additivePos){
	gl_Position = vProjection[i] * vModelview[i] * (gl_in[i].gl_Position + vec4(additivePos * 0.9, 0.0));
	EmitVertex();
}

int xy2i(int x, int y){
	return x * 16 + y;
}

void main()
{
	//そもそもvFlagsの値で判断したいけどうまくいかん（泣）
	for(int i = 0; i < gl_in.length(); ++i){

		gColor = vColor[i];
		
		//gColor[3] = 0.9999;		//0.0~0.999.. これを入れると動く、訳が分からない
		//gColor[1] = 0.5555;		//0.01~1.0    これを入れても動く、訳が分からない
		uint tmpFlags = vFlags[i];

		/*
		uint vFlagsの値　想定と違う値になってしまっている glvertexattribpointer	ぽいんたちゃんと取れてない説
		1099431936 crystal      0100 0001 1000 1000
		1100480512 edgeCrystal  0100 0001 1001 1000
		1101004800 boundary		0100 0001 1010 0000
		1112539136 endofcell    0100 0010 0101 0000
		011
		010
		100
		*/
		
		//不透明に設定したセルだけ作る
		if(vColor[i][3] > 0.0){	// isFlag(i,524288)
			
			//TEST
			/*
			vec3 v3 = vec3(float(triangleConnectionTable[8][0]),
				float(triangleConnectionTable[8][1]),
				float(triangleConnectionTable[8][2])
			);
			*/

			gColor = vColor[i];


			//emitAdditiveVert(i,vec3(0.0, 0.0, 0.0));
			//なぜか全三角形上向いてる
			emitAdditiveVert(i,vec3(0.0, 0.015, 0.015/cos(radians(30.0)) ));
			emitAdditiveVert(i,vec3(0.015, 0.015, 0.015*tan(radians(30.0))));
			emitAdditiveVert(i,vec3(-0.015, 0.015, 0.015*tan(radians(30.0))));
			emitAdditiveVert(i,vec3(0.015, 0.015, -0.015*tan(radians(30.0))));
			emitAdditiveVert(i,vec3(-0.015, 0.015, -0.015*tan(radians(30.0))));
			emitAdditiveVert(i,vec3(0.0, 0.015, -0.015/cos(radians(30.0)) ));
			EndPrimitive();
			
			/*
			emitAdditiveVert(i,vec3(0.0, 0.015, 0.015/cos(radians(30.0)) ));
			emitAdditiveVert(i,vec3(0.0, -0.015, 0.015/cos(radians(30.0)) ));
			emitAdditiveVert(i,vec3(0.015, 0.015, 0.015*tan(radians(30.0))));
			emitAdditiveVert(i,vec3(0.015, -0.015, 0.015*tan(radians(30.0))));
			emitAdditiveVert(i,vec3(0.015, 0.015, -0.015*tan(radians(30.0))));
			emitAdditiveVert(i,vec3(0.015, -0.015, -0.015*tan(radians(30.0))));
			emitAdditiveVert(i,vec3(0.0, 0.015, -0.015/cos(radians(30.0)) ));
			emitAdditiveVert(i,vec3(0.0, -0.015, -0.015/cos(radians(30.0)) ));
			emitAdditiveVert(i,vec3(-0.015, 0.015, -0.015*tan(radians(30.0))));
			emitAdditiveVert(i,vec3(-0.015, -0.015, -0.015*tan(radians(30.0))));
			emitAdditiveVert(i,vec3(-0.015, 0.015, 0.015*tan(radians(30.0))));
			emitAdditiveVert(i,vec3(-0.015, -0.015, 0.015*tan(radians(30.0))));
			emitAdditiveVert(i,vec3(0.0, 0.015, 0.015/cos(radians(30.0)) ));
			emitAdditiveVert(i,vec3(0.0, -0.015, 0.015/cos(radians(30.0)) ));
			*/
		}
		
	}
	EndPrimitive();
}
