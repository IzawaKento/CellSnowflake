#version 430 core

in vec4 vColor[];
in uint vFlags[];
in mat4 vModelview[];
in mat4 vProjection[];
out vec4 gColor;
layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

const float pi = 3.1415926;

//フラグの代わり.....
const uint ISCRYSTAL = 1;
const uint ISEDGECRYSTAL = 2;
const uint ISBOUNDARY = 4;
const uint ISEDGEBOUNDARY = 8;
const uint MZISODD = 16;	//セル番号のZ値が奇数である
const uint ISENDOFCELLS = 32;

//flagIDは2進数の値
//flagsのビットのfragID番目（0から）が1ならtrue
bool isFlag(uint flagID) {
	return ((vFlags[0] & flagID) != 0);
}

void emitAdditiveVert(int i, vec3 additivePos){
	gl_Position = vProjection[i] * vModelview[i] * (gl_in[i].gl_Position + vec4(additivePos, 0.0));
	EmitVertex();
}

void main()
{
	//そもそもvFlagsの値で判断したいやけど、とりあえず、
	for(int i = 0; i < gl_in.length(); ++i){
		gl_Position = gl_in[i].gl_Position;
		gColor = vColor[i];
		
		//gColor[3] = 0.9999;		//0.0~0.999.. これを入れると動く、訳が分からない
		gColor[1] = 0.5555;		//0.01~1.0    これを入れても動く、訳が分からない
		
		//不透明に設定したセルだけ作る
		if(vColor[i][3] > 0.0){	//gColor[3] > 0.0でも同じ
		
			emitAdditiveVert(i,vec3(0.0, 0.0, 0.015/cos(radians(30.0)) ));
			emitAdditiveVert(i,vec3(0.015, 0.0, 0.015*tan(radians(30.0))));
			emitAdditiveVert(i,vec3(-0.015, 0.0, 0.015*tan(radians(30.0))));
			emitAdditiveVert(i,vec3(0.015, 0.0, -0.015*tan(radians(30.0))));
			emitAdditiveVert(i,vec3(-0.015, 0.0, -0.015*tan(radians(30.0))));
			emitAdditiveVert(i,vec3(0.0, 0.0, -0.015/cos(radians(30.0)) ));

		}
	}
	EndPrimitive();
}
