//本文件是输入模块，将接收输入参数，并确定本次运行的方式(生成或求解),以参数way传出
#include "pch.h"
#include "InputProcess.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <ctime>


const static int MaxSudo = 1000000;



static bool CheckValid(char * str) {//检查输入参数是否为int

	int len = (int)strlen(str);
	for (int i = 0; i < len; i++) {
		if (!(str[i] >= '0' && str[i] <= '9'))
			return false;
	}
	return true;
}

int InputProcess(int argc, char** argv, int &way, int &GenerateNum, FILE* &WriteP, FILE* &absolute_path_of_puzzlefile) {

	if (argc == 3 && !strcmp("-c", argv[1]) && strlen(argv[1]) == 2) {//-c

		if (!CheckValid(argv[2])) {	//检查第二个数字是否为合法整数
			printf("输入生成终局参数不合法！\n");
			return -1;
			//exit(1);
		}
		else {
			GenerateNum = atoi(argv[2]);
			if (GenerateNum > MaxSudo) {
				printf("超出最大数独终局生成量！\n");
				return -1;
				//exit(1);
			}
		}
		if (fopen_s(&WriteP, "sudoku.txt", "w")) {//打开要写的文件
			printf("sudoku.txt打开失败\n");
			return -1;
			//exit(1);
		}
		way = 0;
	}
	else if (argc == 3 && !strcmp("-s", argv[1]) && strlen(argv[1]) == 2) {//-s

		//printf("这是一个关于求解数独的测试分支!！\n");
		if (fopen_s(&absolute_path_of_puzzlefile, argv[2], "r")) {
			printf("sudoku.txt打开失败\n");
			return -1;
			//exit(1);
		}
		//return 0;
		way = 1;
	}
	else {
		printf("Invaid input parameters!\n");
		return -1;
		//exit(1);
	}
	return 0;
}