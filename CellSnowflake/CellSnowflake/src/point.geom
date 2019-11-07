#version 430 core

in vec4 vColor[];
in uint vFlags[];
out vec4 gColor;
layout (points) in;
layout (points, max_vertices = 1) out;

//�t���O�̑���.....
const uint ISCRYSTAL = 1;
const uint ISEDGECRYSTAL = 2;
const uint ISBOUNDARY = 4;
const uint ISEDGEBOUNDARY = 8;
const uint MZISODD = 16;	//�Z���ԍ���Z�l����ł���
const uint ISENDOFCELLS = 32;

//flagID��2�i���̒l
//flags�̃r�b�g��fragID�Ԗځi0����j��1�Ȃ�true
bool isFlag(uint flagID) {
	return ((vFlags[0] & flagID) != 0);
}

void main()
{
	//��������flags�̒l�Ŕ��f���ׂ��₯��
	for(int i = 0; i < gl_in.length(); ++i){
		gl_Position = gl_in[i].gl_Position;
		gColor = vColor[i];
		
		gColor[3] = 0.9999;		//0.0~0.999.. ���������Ɠ����A�󂪕�����Ȃ�
		//gColor[1] = 0.5555;		//0.01~1.0    ��������Ă������A�󂪕�����Ȃ�
		
		//�s�����ɐݒ肵���Z���������
		if((vColor[i][3] > 0.0) && (vColor[i][3] <= 1.0)){
			EmitVertex();
		}
	}
	EndPrimitive();
}