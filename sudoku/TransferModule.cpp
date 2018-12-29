//此处为事务中心，通向两条不同的路径：一个是生成数独终局，另一个是求解数独
#include "pch.h"
#include "solver.h"
#include "CheckSudo.h"
#include "generator.h"
#include "InputProcess.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <ctime>

using namespace std;

static char FinalSudo[1000000 * (9 * 18 + 1)];//记录生成的数独数据
static const int Sudolen = 9 * 18;//用于计算单个数独总长度
static char SudoProblem[1000000 * (9 * 18 + 1)];//记录待求解的数独


void TransferModule(int way, int GenerateNum, FILE *WriteP, FILE *absolute_path_of_puzzlefile, FILE *WriteTheSolution) {
	if (way == 0) {

		clock_t StartGenerate = clock();

		int cntGenerate = Generator(FinalSudo, GenerateNum);//进入生成数独模块
		fwrite(FinalSudo, sizeof(char), cntGenerate * (Sudolen + 1) - 2, WriteP);//打印本终局
		//CheckSudo(FinalSudo, cntGenerate * (sizeof(tmpSudoMatrix2) + 1) - 2);//用以检测数独终局的合法性

		clock_t EndGenerate = clock();
		printf("Generating Sudoku time cost = %.2fms\n", ((double)(EndGenerate - StartGenerate)));//第三步，显示总耗时

		fclose(WriteP);//第四步，关闭文件指针
	}
	else if (way == 1) {

		clock_t StartSolve = clock();

		int idx = 0;
		char c = 0;
		clock_t SolveStart = clock();//求解数独计时开始
		while (((c = fgetc(absolute_path_of_puzzlefile)) != EOF) && c) {//读取数独

			SudoProblem[idx] = c;
			idx++;
		}

		int flag = Solver(SudoProblem, idx);	//求解数独

		if (fopen_s(&WriteTheSolution, ".\\sudoku.txt", "w")) {
			printf("向sudoku.txt写入最终结果前，打开文件失败\n");
			exit(1);
		}
		//将文件结果写回文件
		fwrite(SudoProblem, 1, idx, WriteTheSolution);

		clock_t SolveEnd = clock();//求解数独计时结束

		printf("Generating Sudoku time cost = %.2fms\n", ((double)(SolveEnd - SolveStart)));
		printf("打印求解数独的结果：\n");
		for (int i = 0; i < idx; i++)putchar(SudoProblem[i]);//打印出求解数独的结果
	}
}