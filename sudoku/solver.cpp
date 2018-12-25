//#include "stdafx.h"
#include "solver.h"
#include "pch.h"
#include <string.h>

using namespace std;


static int asfds = 0;
static int all = (1 << 9) - 1;
static int num[1 << 10];
//FILE *P; //��ȡ�����ļ�

//extern char * SudoProb; //��sudoku���޸�Ҫ����������Ŀ�������˱����޸ģ��ڱ���⺯����ʹ�ã���Ϊ�������ݿ��ܻ�ܴ�����
static int SudoMat[10][10];//��ŵ�ǰ�������������
static int id[10][10];
static bool JumpOut = false;
//clock_t Timeout_start, Timeout_end;
static int FinalIdx = 0;//���������յ���������д���(���ô���ľ���)
static char *tmpSudoProb;

template <typename Dtype>
unsigned int __builtin_popcount(Dtype u)
{
	u = (u & 0x55555555) + ((u >> 1) & 0x55555555);
	u = (u & 0x33333333) + ((u >> 2) & 0x33333333);
	u = (u & 0x0F0F0F0F) + ((u >> 4) & 0x0F0F0F0F);
	u = (u & 0x00FF00FF) + ((u >> 8) & 0x00FF00FF);
	u = (u & 0x0000FFFF) + ((u >> 16) & 0x0000FFFF);
	return u;
}

void dfs(int dep)
{
	/*Timeout_end = clock();
	if (Timeout_end - Timeout_start > 1000)
		return;*/
	if (!dep)
	{
		for (int i = 1; i <= 9; i++) {
			for (int j = 1; j <= 8; j++) {
				//solvesudoku += (SudoMat[i][j] + '0');
				tmpSudoProb[FinalIdx++] = SudoMat[i][j] + '0';
				tmpSudoProb[FinalIdx++] = ' ';
				//solvesudoku += ' ';
			}
			//solvesudoku += (SudoMat[i][9] + '0');
			tmpSudoProb[FinalIdx++] = SudoMat[i][9] + '0';

			//solvesudoku += '\n';
			tmpSudoProb[FinalIdx++] = '\n';

		}
		//solvesudoku += '\n';
		tmpSudoProb[FinalIdx++] = '\n';

		//longjmp(buf, 1);	//������ѭ�� 
		return;
	}
	int b[10][10], c[10][10], x = 0, y = 0, z = 9;
	for (int i = 1; i <= 9; i++)
	{
		for (int j = 1; j <= 9; j++)
		{
			if (!SudoMat[i][j] && !id[i][j])
				return;
			b[i][j] = SudoMat[i][j];
			c[i][j] = id[i][j];
			if (!SudoMat[i][j])
			{
				if (num[id[i][j]] < z)
					z = num[id[i][j]], x = i, y = j;
			}
		}
	}
	for (int i = 0; i < 9; i++)
		if (id[x][y] & (1 << i))
		{
			SudoMat[x][y] = i + 1;
			for (int k = 1; k <= 9; k++)
				id[k][y] &= ((1 << 9) - 1 - (1 << i));
			for (int k = 1; k <= 9; k++)
				id[x][k] &= ((1 << 9) - 1 - (1 << i));
			for (int k = (x - 1) / 3 * 3 + 1; k <= (x - 1) / 3 * 3 + 3; k++)
			{
				for (int l = (y - 1) / 3 * 3 + 1; l <= (y - 1) / 3 * 3 + 3; l++)
				{
					id[k][l] &= ((1 << 9) - 1 - (1 << i));
				}
			}
			dfs(dep - 1);
			if (dep - 1 == 0) return;//////////
			for (int i = 1; i <= 9; i++)
			{
				for (int j = 1; j <= 9; j++)
				{
					id[i][j] = c[i][j], SudoMat[i][j] = b[i][j];
				}
			}
		}
	return;
}

static void InitSolve()
{
	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
			id[i][j] = all;
	for (int i = 1; i <= 9; i++)
	{
		for (int j = 1; j <= 9; j++)
		{
			if (SudoMat[i][j])
			{
				for (int k = 1; k <= 9; k++)
					id[k][j] &= ((1 << 9) - 1 - (1 << (SudoMat[i][j] - 1)));

				for (int k = 1; k <= 9; k++)
					id[i][k] &= ((1 << 9) - 1 - (1 << (SudoMat[i][j] - 1)));

				for (int k = (i - 1) / 3 * 3 + 1; k <= (i - 1) / 3 * 3 + 3; k++)
					for (int l = (j - 1) / 3 * 3 + 1; l <= (j - 1) / 3 * 3 + 3; l++)
						id[k][l] &= ((1 << 9) - 1 - (1 << (SudoMat[i][j] - 1)));
			}
		}
	}
}

static bool Isdigit(char c) {

	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int Solver(char * SudoProb) {//����Ĳ���Ϊ���������վ�
	tmpSudoProb = SudoProb;
	int idx = 0, cnt = 0, Complete = 0, NotZero = 0;//idx�������㵱ǰ�������ַ���λ�ã�Complete����ȷ����ǰ����������һ�������������վ�
	for (int i = 1; i <= 1023; i++)num[i] = __builtin_popcount(i);
	while (SudoProb[idx++] != '\0') {

		if (Isdigit(SudoProb[idx])) {
			cnt++;		//cnt�������㵱ǰ�����������ָ���
			int Num = SudoProb[idx] - '0';
			if (Num) {
				SudoMat[cnt / 9 + 1][cnt % 9 + 1] = Num;
				NotZero++;
			}
			if (cnt == 81) {//����һ�����������
				//start_solve = clock();
				cnt = 0;

				/*if (cnt == 0)	//����ȫ0��������� 
				{
					createCompleteSudoku(0);
					continue;
				}*/

				InitSolve();

				dfs(81 - cnt);
				cnt = 0;
				memset(SudoMat, 0, sizeof(SudoMat));
				memset(num, 0, sizeof(num));
				memset(id, 0, sizeof(id));

			}
		}

	}

	return 0;
}