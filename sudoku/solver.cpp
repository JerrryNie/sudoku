//#include "stdafx.h"
#include "solver.h"
#include "pch.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <csetjmp>
using namespace std;


static int asfds = 0;
static int all = (1 << 9) - 1;
static int num[1 << 10];
static jmp_buf buf;
//FILE *P; //读取数独文件

//extern char * SudoProb; //在sudoku中修改要求解的数独题目，并将此变量修改，在本求解函数中使用，因为传入数据可能会很大，所以
static int SudoMat[10][10];//存放当前待解的数独矩阵
static int id[10][10];
static bool JumpOut = false;
//clock_t Timeout_start, Timeout_end;
static int FinalIdx = 0;//用于向最终的求解矩阵中写结果(利用传入的矩阵)
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

static int SudoSize = 0;//存放传入的数独大小

void dfs(int dep)
{
	/*Timeout_end = clock();
	if (Timeout_end - Timeout_start > 1000)
		return;*/
	if (FinalIdx >= SudoSize) {
		longjmp(buf, 1);
	}
	if (!dep)
	{
		printf("dep = %d\n", dep);
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

		longjmp(buf, 1);	//跳出死循环 
		//return;
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
			//if (dep - 1 == 0) return;//////////
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

bool Isdigit(char c) {

	return c >= '0' && c <= '9';
}



int Solver(char * SudoProb, int Size) {//传入的参数为所有数独终局
	tmpSudoProb = SudoProb;
	SudoSize = Size;
	int idx = 0, cnt = 0, Complete = 0, NotZero = 0;//idx用来计算当前搜索的字符串位置，Complete用来确定当前搜索到的是一个完整的数独终局
	for (int i = 1; i <= 1023; i++)num[i] = __builtin_popcount(i);
	while (SudoProb[idx] != '\0') {

		if (Isdigit(SudoProb[idx])) {//
			cnt++;		//cnt用来计算当前的总输入数字个数
			int Num = SudoProb[idx] - '0';
			if (Num) {
				SudoMat[(cnt - 1) / 9 + 1][(cnt - 1) % 9 + 1] = Num;
				NotZero++;
			}//
			if (cnt == 81) {//接收一个完整的棋局
				//start_solve = clock();
				cnt = 0;
				
				/*if (cnt == 0)	//输入全0，任意输出 
				{
					createCompleteSudoku(0);
					continue;
				}*/
				
				InitSolve();
				printf("NotZero = %d\n", NotZero);
				for (int i = 1; i <= 9; i++) {
					for (int j = 1; j <= 9; j++) {
						printf("%d", SudoMat[i][j]);
					}
					printf("\n");
				}
				while(!setjmp(buf))
					dfs(81 - NotZero);
				
				NotZero = 0;
				memset(SudoMat, 0, sizeof(SudoMat));
				memset(num, 0, sizeof(num));
				memset(id, 0, sizeof(id));

			}
		}
		idx++;
	}

	return 0;
}
