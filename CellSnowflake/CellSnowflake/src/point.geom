#version 430 core

in vec4 vColor[];
in uint vFlags[];
in mat4 vModelview[];
in mat4 vProjection[];
out vec4 gColor;
layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

const float pi = 3.1415926;

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

void emitAdditiveVert(int i, vec3 additivePos){
	gl_Position = vProjection[i] * vModelview[i] * (gl_in[i].gl_Position + vec4(additivePos, 0.0));
	EmitVertex();
}

void main()
{
	//��������vFlags�̒l�Ŕ��f�������₯�ǁA�Ƃ肠�����A
	for(int i = 0; i < gl_in.length(); ++i){
		gl_Position = gl_in[i].gl_Position;
		gColor = vColor[i];
		
		//gColor[3] = 0.9999;		//0.0~0.999.. ���������Ɠ����A�󂪕�����Ȃ�
		gColor[1] = 0.5555;		//0.01~1.0    ��������Ă������A�󂪕�����Ȃ�
		
		//�s�����ɐݒ肵���Z���������
		if(vColor[i][3] > 0.0){	//gColor[3] > 0.0�ł�����
		
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
