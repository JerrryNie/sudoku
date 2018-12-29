// sudoku.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "solver.h"
#include "CheckSudo.h"
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
static char CheckMat[1000];
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
		for (int i = 0; i < RowTransNum; i++) {		//遍历所有的行变换
			
			if (cntGenerate >= Num) {

				fwrite(FinalSudo, sizeof(char), cntGenerate * (sizeof(tmpSudoMatrix2) + 1) - 2, WriteP);//打印本终局
				//CheckSudo(FinalSudo, cntGenerate * (sizeof(tmpSudoMatrix2) + 1) - 2);//用以检测数独终局的合法性
				return; //当达到最大的生成数量时，即停止生成
			}
			for (int j = 0; j < Row; j++) {//“+1”是为了输入回车换行
				memcpy(tmpSudoMatrix2[j], tmpSudoMatrix[RowTrans[i][j] - 1], sizeof(tmpSudoMatrix[j]));

				
			}
			memcpy(FinalSudo + cntGenerate * (sizeof(tmpSudoMatrix2) + 1), tmpSudoMatrix2, sizeof(tmpSudoMatrix2));
			memcpy(FinalSudo + cntGenerate * (sizeof(tmpSudoMatrix2) + 1) + sizeof(tmpSudoMatrix2), "\n", 1);
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


void CloseAllFile() {//一次性将打开的所有文件关闭


}

int main(int argc, char * argv[])
{
	FILE *absolute_path_of_puzzlefile = NULL;
	

	//处理命令行输入部分
	int way = -1;//0表示处理生成，1表示处理求解
	
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
		way = 0;
	}
	else if (argc == 3 && !strcmp("-s", argv[1]) && strlen(argv[1]) == 2) {//-s

		printf("这是一个关于求解数独的测试分支!！\n");
		if (fopen_s(&absolute_path_of_puzzlefile, argv[2], "r")) {
			printf("sudoku.txt打开失败\n");
			exit(1);
		}
		//return 0;
		way = 1;
	}
	else {
		printf("Invaid input parameters!\n");
		exit(1);
	}
	if(way == 0) {
		if (fopen_s(&WriteP, "sudoku.txt", "w")) {//第一步，打开要写的文件
			printf("sudoku.txt打开失败\n");
			exit(1);
		}
		clock_t StartGenerate = clock();
	

		GenerateSudo(GenerateNum);	//第二步，生成相应的数独

		clock_t EndGenerate = clock();

		printf("Generating Sudoku time cost = %.2fms\n", ((double)(EndGenerate - StartGenerate)));//第三步，显示总耗时

		fclose(WriteP);//第四步，关闭文件指针
	}
	else if (way == 1) {
		clock_t StartSolve = clock();

		int idx = 0;
		char c = 0;
		clock_t SolveStart = clock();//求解数独计时开始
		while (((c = fgetc(absolute_path_of_puzzlefile)) != EOF) && c) {
			
			SudoProblem[idx] = c;
			idx++;
		}

		int flag = Solver(SudoProblem, idx);	//求解数独

		FILE * WriteTheSolution = NULL;
		//for (int i = 0; i < idx; i++)putchar(SudoProblem[i]);//打印出求解数独的结果
		if (fopen_s(&WriteTheSolution, ".\\sudoku.txt", "w")) {
			printf("向sudoku.txt写入最终结果前，打开文件失败\n");
			//fwrite(SudoProblem, 1, idx, WriteTheSolution);
			exit(1);
		}
		//printf("---\n");
		//将文件结果写回文件
		fwrite(SudoProblem, 1, idx, WriteTheSolution);

		clock_t SolveEnd = clock();//求解数独计时结束

		printf("Generating Sudoku time cost = %.2fms\n", ((double)(SolveEnd - SolveStart)));
		printf("打印求解数独的结果：\n");
		for (int i = 0; i < idx; i++)putchar(SudoProblem[i]);//打印出求解数独的结果
	}
	//
	/*FILE * CheckPoint = NULL;
	if (fopen_s(&CheckPoint, "F:\\CheckSudo.txt", "r")) {
		printf("提取合法性检验测试文件，打开文件失败\n");
		exit(1);
	}
	char c = 0;
	int idx = 0;
	while (((c = fgetc(CheckPoint)) != EOF) && c) {
		CheckMat[idx] = c;
		idx++;
	}
	CheckSudo(CheckMat, idx);*/
	//
	CloseAllFile();
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
