//���ļ�������������
#include "pch.h"
#include "generator.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <ctime>

using namespace std;


const static int ColTransNum = 40320;	//�б任����Ϊ40320��
const static int Row = 9, Col = 9;		//�����������ֱ�Ϊ9
const static int RowTransNum = 30;		//�б任����Ϊ30��
const static int GenerateSudoFromFirstRow[10][10] = {	//���ݵ�һ�е����������֮�����е���
	{1,2,3,4,5,6,7,8,9},
	{7,8,9,1,2,3,4,5,6},
	{4,5,6,7,8,9,1,2,3},
	{2,3,4,5,6,7,8,9,1},
	{8,9,1,2,3,4,5,6,7},
	{5,6,7,8,9,1,2,3,4},
	{3,4,5,6,7,8,9,1,2},
	{9,1,2,3,4,5,6,7,8},
	{6,7,8,9,1,2,3,4,5}
};

const static char RowTrans[40][10] = {//�����е��б任���
	{1,2,3,4,5,6,7,9,8},
	{1,2,3,4,5,6,8,7,9},
	{1,2,3,4,5,6,8,9,7},
	{1,2,3,4,5,6,9,7,8},
	{1,2,3,4,5,6,9,8,7},
	{1,2,3,4,5,6,7,8,9},
	{1,2,3,4,6,5,7,9,8},
	{1,2,3,4,6,5,8,7,9},
	{1,2,3,4,6,5,8,9,7},
	{1,2,3,4,6,5,9,7,8},
	{1,2,3,4,6,5,9,8,7},
	{1,2,3,4,6,5,7,8,9},
	{1,2,3,5,4,6,7,9,8},
	{1,2,3,5,4,6,8,7,9},
	{1,2,3,5,4,6,8,9,7},
	{1,2,3,5,4,6,9,7,8},
	{1,2,3,5,4,6,9,8,7},
	{1,2,3,5,4,6,7,8,9},
	{1,2,3,5,6,4,7,9,8},
	{1,2,3,5,6,4,8,7,9},
	{1,2,3,5,6,4,8,9,7},
	{1,2,3,5,6,4,9,7,8},
	{1,2,3,5,6,4,9,8,7},
	{1,2,3,5,6,4,7,8,9},
	{1,2,3,6,4,5,7,9,8},
	{1,2,3,6,4,5,8,7,9},
	{1,2,3,6,4,5,8,9,7},
	{1,2,3,6,4,5,9,7,8},
	{1,2,3,6,4,5,9,8,7},
	{1,2,3,6,4,5,7,8,9},
	{1,2,3,6,5,4,7,9,8},
	{1,2,3,6,5,4,8,7,9},
	{1,2,3,6,5,4,8,9,7},
	{1,2,3,6,5,4,9,7,8},
	{1,2,3,6,5,4,9,8,7},
	{1,2,3,6,5,4,7,8,9}

};

static char tmpSudoMatrix[9][18];	//��¼ÿ���б任֮��������վ�
static char tmpSudoMatrix2[9][18];	//��¼ÿ���б任֮��������վ�
static char Rrow[11] = { '5', '1', '2', '3', '4', '6', '7', '8', '9', '\0', '\0' }; //�����ϻ��з��Լ��ַ���������



int Generator(char * FinalSudo, int Num) {//Num����������������, FinalSudo��¼�������ɵ������վ�

	int cntGenerate = 0;	 //��¼��ǰ���ɵ�������


	for (int j = 0; j < ColTransNum; j++) {	//�������е��б任
		for (int r = 0; r < Row; r++) {
			for (int c = 0; c < Col; c++) {
				//���ݵ�һ�е�������������µ������վ�
				int idx = GenerateSudoFromFirstRow[r][c];
				tmpSudoMatrix[r][c * 2] = Rrow[idx - 1];	//ż��λ�÷����֣�����λ�÷ſո�
				if (c < Col - 1) {
					tmpSudoMatrix[r][c * 2 + 1] = ' ';
				}
			}
			tmpSudoMatrix[r][17] = '\n';
		}
		for (int i = 0; i < RowTransNum; i++) {		//�������е��б任

			if (cntGenerate >= Num) {

				//fwrite(FinalSudo, sizeof(char), cntGenerate * (sizeof(tmpSudoMatrix2) + 1) - 2, WriteP);//��ӡ���վ�
				//CheckSudo(FinalSudo, cntGenerate * (sizeof(tmpSudoMatrix2) + 1) - 2);//���Լ�������վֵĺϷ���
				return cntGenerate; //���ﵽ������������ʱ����ֹͣ���ɣ������������������������������������������������
			}
			for (int j = 0; j < Row; j++) {//��+1����Ϊ������س�����
				memcpy(tmpSudoMatrix2[j], tmpSudoMatrix[RowTrans[i][j] - 1], sizeof(tmpSudoMatrix[j]));


			}
			memcpy(FinalSudo + cntGenerate * (sizeof(tmpSudoMatrix2) + 1), tmpSudoMatrix2, sizeof(tmpSudoMatrix2));
			memcpy(FinalSudo + cntGenerate * (sizeof(tmpSudoMatrix2) + 1) + sizeof(tmpSudoMatrix2), "\n", 1);
			cntGenerate++;


		}
		next_permutation(Rrow + 1, Rrow + 9);//���˵�һ��Ԫ���⣬ȫ��������������Ԫ��

	}

	return cntGenerate;//�������棬Ӧ���޷�ִ�е���仰
}