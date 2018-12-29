#include "pch.h"
#include "CheckSudo.h"
#include <string.h>
#include <math.h>
#include <stdio.h>


using namespace std;
static bool Isdigit(char c) {
	return c >= '0' && c <= '9';
}
static int SudoMat[10][10];
//static int Bitmap[10];
static bool Check(int SudoMat[][10]) {//检查某一特定的数独合法性

	int BitMap[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (int i = 1; i <= 9; i++) {//检查行方向合法性

		for (int j = 1; j <= 9; j++) {
			BitMap[SudoMat[i][j]] = true;
		}
		for (int j = 1; j <= 9; j++) {
			if (BitMap[j] == false) {
			
				printf("行非法：%d, %d\n", i, j);
				return false;//出现不合法数独

			}
		}
		memset(BitMap, 0, sizeof(BitMap));
	}

	for (int j = 1; j <= 9; j++) {//检查列方向合法性

		for (int i = 1; i <= 9; i++) {
			BitMap[SudoMat[i][j]] = true;
		}
		for (int i = 1; i <= 9; i++) {
			if (BitMap[i] == false) {
			
				printf("列非法：%d, %d\n", i, j);
				return false;//出现不合法数独
			}
		}
		memset(BitMap, 0, sizeof(BitMap));
	}
	const int dr[] = { 0, 1, 2 };
	const int dc[] = { 0, 1, 2 };
	for (int i = 1; i <= 9; i += 3) {
		
		for (int j = 1; j <= 9; j += 3) {

			for (int k = 0; k <= 2; k++) {
				for (int m = 0; m <= 2; m++) {
					BitMap[SudoMat[i + dr[k]][j + dc[m]]] = true;
				}
			}
			for (int m = 1; m <= 9; m++) {
				if (BitMap[m] == false) {
			
					printf("3*3非法：%d, %d\n", i, j);
					return false;//出现不合法数独
				}
			}
			memset(BitMap, 0, sizeof(BitMap));
		}
	}
	return true;
}

bool CheckSudo(char * SudoAllMat, int Size) {//检查输入的所有数独的正确性

	int idx = 0;
	int CntNum = 0;//计算输入的数字量
	int CntSudo = 0;//计算数独数量
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
			if (Correct == false) {
				printf("非法数独：%d\n", CntSudo);
			}
			CntNum = 0;
			memset(SudoMat, 0, sizeof(SudoMat));
		}

		idx++;
	}
	printf("所有数独均合法\n");
	return true;
}