// sudoku.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "solver.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <ctime>

using namespace std;

static FILE * WriteP = NULL;//用于写数独终局

const static int RowTransNum = 30;		//行变换总数为30个
const static int ColTransNum = 40320;	//列变换总数为40320个

const static int Row = 9, Col = 9;		//行数和列数分别都为9

const static int MaxSudo = 1000000;

const static char TOPLEFT = '5';//学号后两位是22，因而左上角是5
const static char RowTrans[40][10] = {//给所有的行变换打表
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

const static int GenerateSudoFromFirstRow[10][10] = {	//根据第一行的情况，生成之后所有的行
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

//static int ColTrans[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };//列变换映射向量

static char Rrow[11] = { '5', '1', '2', '3', '4', '6', '7', '8', '9', '\0', '\0'}; //最后加上换行符以及字符串结束符

static char tmpSudoMatrix[9][18];	//记录每次列变换之后的数独终局
static char tmpSudoMatrix2[9][18];	//记录每次行变换之后的数独终局

static char FinalSudo[1000000 * (9 * 18 + 1)];
static char SudoProblem[1000000 * (9 * 18 + 1)];

static void GenerateSudo(int Num) {

	int cntGenerate = 0;	 //记录当前生成的数独数

	//for(int i = 0; i < Col; i++)

	for (int j = 0; j < ColTransNum; j++) {	//遍历所有的列变换
		for (int r = 0; r < Row; r++) {
			for (int c = 0; c < Col; c++) {
				//根据第一行的情况，生成余下的数独终局
				int idx = GenerateSudoFromFirstRow[r][c];
				tmpSudoMatrix[r][c * 2] = Rrow[idx - 1];	//偶数位置放数字，奇数位置放空格
				if (c < Col - 1) {
					tmpSudoMatrix[r][c * 2 + 1] = ' ';
				}
			}
			tmpSudoMatrix[r][17] = '\n';
			tmpSudoMatrix[r][18] = 0;
		}
		//tmpSudoMatrix[9][0] = '\n';//完成一个终局，输入回车
		//tmpSudoMatrix[9][1] = '\0';
		for (int i = 0; i < RowTransNum; i++) {		//遍历所有的行变换
			
			if (cntGenerate >= Num) {

				fwrite(FinalSudo, sizeof(char), cntGenerate * (sizeof(tmpSudoMatrix2) + 1) - 2, WriteP);//打印本终局
				return; //当达到最大的生成数量时，即停止生成
			}
			for (int j = 0; j < Row; j++) {//“+1”是为了输入回车换行
				//printf("j = %d:", j);
					memcpy(tmpSudoMatrix2[j], tmpSudoMatrix[RowTrans[i][j] - 1], sizeof(tmpSudoMatrix[j]));
					//fwrite(tmpSudoMatrix[RowTrans[i][j] - 1], sizeof(char), sizeof(tmpSudoMatrix[RowTrans[i][j] - 1]) / sizeof(char), WriteP);
					//fwrite(tmpSudoMatrix[RowTrans[i][j] - 1], 1, 18, WriteP);
				
			}
			//tmpSudoMatrix2[9][0] = '\n';	
			
			//fwrite(tmpSudoMatrix2, sizeof(char), sizeof(tmpSudoMatrix2) / sizeof(char), WriteP);//打印本终局
			memcpy(FinalSudo + cntGenerate * (sizeof(tmpSudoMatrix2) + 1), tmpSudoMatrix2, sizeof(tmpSudoMatrix2));
			memcpy(FinalSudo + cntGenerate * (sizeof(tmpSudoMatrix2) + 1) + sizeof(tmpSudoMatrix2), "\n", 1);
			//fputc('\n', WriteP);
			cntGenerate++;
			
			
		}
		next_permutation(Rrow + 1, Rrow + 9);//除了第一个元素外，全排列其他的所有元素
		
	}
}

static bool CheckValid(char * str) {//检查输入参数是否为int

	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		if (!(str[i] >= '0' && str[i] <= '9'))
			return false;
	}
	return true;
}

static bool CheckSudoValid(char Matrix[9][18]) {

	char BitMap[10];
	memset(BitMap, sizeof(BitMap), 0);

	for (int r = 0; r < 9; r++) {//检查行方向有效性
		for (int c = 0; c < 18; c++) {
			BitMap[Matrix[r][c * 2] - '0'] = 1;
		}
		for(int i = 1; i <= 9; i++) 
			if (BitMap[i] == 0) {
				return false;
			}
	}

	for (int c = 0; c < 9; c++) {//检查列方向有效性
		for (int r = 0; r < 18; r++) {
			BitMap[Matrix[r][c * 2] - '0'] = 1;
		}
		for (int i = 1; i <= 9; i++)
			if (BitMap[i] == 0) {
				return false;
				memset(BitMap, sizeof(BitMap), 0);
			}
	}
	const int dr[] = {0, 1, 2};
	const int dc[] = {0, 1, 2};
	const int x[] = {0, 3, 6};
	const int y[] = { 0, 3, 6 };
	for (int cx = 0; cx < 3; cx++) {
		for (int cy = 0; cy < 3; cy++) {
			for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						int tmpx = x[cx] + dr[i];
						int tmpy = x[cy] + dc[j];
						BitMap[Matrix[tmpx][tmpy] - '0'] = 1;
				}
			}
			for (int i = 1; i <= 9; i++)
				if (BitMap[i] == 0) {
					return false;
				}
			memset(BitMap, sizeof(BitMap), 0);
		}
	}
	

}

int main(int argc, char * argv[])
{
	FILE *absolute_path_of_puzzlefile = NULL;
	if (fopen_s(&WriteP, "sudoku.txt", "w")) {
		printf("sudoku.txt打开失败\n");
		exit(1);
	}
	memset(tmpSudoMatrix, sizeof(tmpSudoMatrix), 0);
	int GenerateNum = 10;
	if (argc == 3 && !strcmp("-c", argv[1]) && strlen(argv[1]) == 2) {//-c
		
		if (!CheckValid(argv[2])) {	//检查第二个数字是否为合法整数
			printf("输入生成终局参数不合法！\n");
			exit(1);
		}
		else {
			GenerateNum = atoi(argv[2]);
			if (GenerateNum > MaxSudo) {
				printf("超出最大数独终局生成量！\n");
				exit(1);
			}
		}
	}
	else if (argc == 3 && !strcmp("-s", argv[1]) && strlen(argv[1]) == 2) {//-s

		printf("这是一个关于求解数独的测试分支!！\n");
		if (fopen_s(&absolute_path_of_puzzlefile, argv[2], "r")) {
			printf("sudoku.txt打开失败\n");
			exit(1);
		}
		return 0;
	}
	else {
		printf("输入操作参数不合法！\n");
		exit(1);
	}

	clock_t StartGenerate = clock();
	
	GenerateSudo(GenerateNum);	//生成相应的数独

	clock_t EndGenerate = clock();

	printf("生成数独用时：%.2fms\n", ((double)(EndGenerate - StartGenerate)));

	fclose(WriteP);
	clock_t StartSolve = EndGenerate;
	
	int flag = Solver(SudoProblem);	//求解数独

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
