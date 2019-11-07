#version 430 core

in vec4 vColor[];
in uint vFlags[];
out vec4 gColor;
layout (points) in;
layout (points, max_vertices = 1) out;

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

void main()
{
	//そもそもflagsの値で判断すべきやけど
	for(int i = 0; i < gl_in.length(); ++i){
		gl_Position = gl_in[i].gl_Position;
		gColor = vColor[i];
		
		gColor[3] = 0.9999;		//0.0~0.999.. これを入れると動く、訳が分からない
		//gColor[1] = 0.5555;		//0.01~1.0    これを入れても動く、訳が分からない
		
		//不透明に設定したセルだけ作る
		if((vColor[i][3] > 0.0) && (vColor[i][3] <= 1.0)){
			EmitVertex();
		}
	}
	EndPrimitive();
}