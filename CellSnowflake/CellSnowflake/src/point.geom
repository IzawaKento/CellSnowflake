#version 430 core
layout (points) in;
layout (points, max_vertices = 1) out;
in vec4 vColor[];
in uint vFlags[];
out vec4 gColor;

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
	gl_Position = gl_in[0].gl_Position;
	gColor = vColor[0].rgba;
	gColor.a = vColor[0].a;
	if(vColor[0].a > 0.01){
		EmitVertex();
	}
	//EmitVertex();
	EndPrimitive();
}