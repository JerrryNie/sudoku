#include "pch.h"
#include "CheckSudo.h"
#include <string.h>
#include <math.h>
#include <stdio.h>


using namespace std;
extern bool Isdigit(char);

static int SudoMat[10][10];
//static int Bitmap[10];
static bool Check(int SudoMat[][10]) {//���ĳһ�ض��������Ϸ���

	int BitMap[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 1; i <= 9; i++) {//����з���Ϸ���

		for (int j = 1; j <= 9; j++) {
			BitMap[SudoMat[i][j]] = true;
		}
		for (int j = 1; j <= 9; j++) {
			if (BitMap[j] == false) {
			
				printf("�зǷ���%d, %d\n", i, j);
				return false;//���ֲ��Ϸ�����

			}
		}
		memset(BitMap, 0, sizeof(BitMap));
	}

	for (int j = 1; j <= 9; j++) {//����з���Ϸ���

		for (int i = 1; i <= 9; i++) {
			BitMap[SudoMat[i][j]] = true;
		}
		for (int i = 1; i <= 9; i++) {
			if (BitMap[i] == false) {
			
				printf("�зǷ���%d, %d\n", i, j);
				return false;//���ֲ��Ϸ�����
			}
		}
		memset(BitMap, 0, sizeof(BitMap));
	}
	const int dr[] = { 0, 1, 2 };
	const int dc[] = { 0, 1, 2 };
	/*printf("---------------\n");
	for (int i = 1; i <= 9; i++) {
		for (int j = 1; j <= 9; j++) {
			printf("%d", SudoMat[i][j]);
		}
		puts("");
	}*/
	for (int i = 1; i <= 9; i += 3) {
		
		for (int j = 1; j <= 9; j += 3) {

			for (int k = 0; k <= 2; k++) {
				for (int m = 0; m <= 2; m++) {
					BitMap[SudoMat[i + dr[k]][j + dc[m]]] = true;
				}
			}
			for (int m = 1; m <= 9; m++) {
				if (BitMap[m] == false) {
			
					printf("3*3�Ƿ���%d, %d\n", i, j);
					return false;//���ֲ��Ϸ�����
				}
			}
			memset(BitMap, 0, sizeof(BitMap));
		}
	}
	return true;
}

bool CheckSudo(char * SudoAllMat, int Size) {//��������������������ȷ��

	int idx = 0;
	int CntNum = 0;//���������������
	int CntSudo = 0;//������������
	while (SudoAllMat[idx] != '\0' && idx < Size) {

		if (Isdigit(SudoAllMat[idx]) && SudoAllMat[idx] == '0') {
			
			return false;
			
		}
		else if (Isdigit(SudoAllMat[idx])) {
			
			CntNum++;
			SudoMat[(CntNum - 1) / 9 + 1][(CntNum - 1) % 9 + 1] = SudoAllMat[idx] - '0';
		}
		if (CntNum == 81) {

			int Correct = Check(SudoMat);
			CntSudo++;
			//printf("id : %d\tCorrect : %d", CntSudo, Correct);
			if (Correct == false) {
				printf("�Ƿ�������%d\n", CntSudo);
			}
			CntNum = 0;
			memset(SudoMat, 0, sizeof(SudoMat));
		}

		idx++;
	}
	printf("�����������Ϸ�\n");
	return true;
}