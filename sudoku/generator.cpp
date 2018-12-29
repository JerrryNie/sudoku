//此文件用于生成数独
#include "pch.h"
#include "generator.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <ctime>

using namespace std;


const static int ColTransNum = 40320;	//列变换总数为40320个
const static int Row = 9, Col = 9;		//行数和列数分别都为9
const static int RowTransNum = 30;		//行变换总数为30个
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

static char tmpSudoMatrix[9][18];	//记录每次列变换之后的数独终局
static char tmpSudoMatrix2[9][18];	//记录每次行变换之后的数独终局
static char Rrow[11] = { '5', '1', '2', '3', '4', '6', '7', '8', '9', '\0', '\0' }; //最后加上换行符以及字符串结束符



int Generator(char * FinalSudo, int Num) {//Num是生成数独的数量, FinalSudo记录最终生成的数独终局

	int cntGenerate = 0;	 //记录当前生成的数独数


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
		}
		for (int i = 0; i < RowTransNum; i++) {		//遍历所有的行变换

			if (cntGenerate >= Num) {

				//fwrite(FinalSudo, sizeof(char), cntGenerate * (sizeof(tmpSudoMatrix2) + 1) - 2, WriteP);//打印本终局
				//CheckSudo(FinalSudo, cntGenerate * (sizeof(tmpSudoMatrix2) + 1) - 2);//用以检测数独终局的合法性
				return cntGenerate; //当达到最大的生成数量时，即停止生成，并返回主调函数，利用主调函数来进行最终数独的输出
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

	return cntGenerate;//消除警告，应该无法执行到这句话
}