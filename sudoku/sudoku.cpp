// sudoku.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "pch.h"
#include "solver.h"
#include "CheckSudo.h"
#include "generator.h"
#include "InputProcess.h"
#include "TransferModule.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <ctime>

using namespace std;

static FILE * WriteP = NULL;//用于写数独终局
static FILE *absolute_path_of_puzzlefile = NULL;//指向写入数独求解结果的文件
static FILE * WriteTheSolution = NULL;


//static const int Sudolen = 9 * 18;//用于计算单个数独总长度

const static char TOPLEFT = '5';//学号后两位是22，因而左上角是5

//static char FinalSudo[1000000 * (9 * 18 + 1)];
//static char SudoProblem[1000000 * (9 * 18 + 1)];
static char CheckMat[1000];

static bool CheckValid(char * str) {//检查输入参数是否为int

	int len = (int)strlen(str);
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
	int way = -1;//0表示处理生成，1表示处理求解
	
	int GenerateNum = 0;
	InputProcess(argc, argv, way, GenerateNum, WriteP, absolute_path_of_puzzlefile);//进行输入数据处理
	
	TransferModule(way, GenerateNum, WriteP, absolute_path_of_puzzlefile, WriteTheSolution);//转向事务中心，进行必要的活动路径调用
	CloseAllFile();//关闭所有的文件指针(若尚未关闭)
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
