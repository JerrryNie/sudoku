//�˴�Ϊ�������ģ�ͨ��������ͬ��·����һ�������������վ֣���һ�����������
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

static char FinalSudo[1000000 * (9 * 18 + 1)];//��¼���ɵ���������
static const int Sudolen = 9 * 18;//���ڼ��㵥�������ܳ���
static char SudoProblem[1000000 * (9 * 18 + 1)];//��¼����������


void TransferModule(int way, int GenerateNum, FILE *WriteP, FILE *absolute_path_of_puzzlefile, FILE *WriteTheSolution) {
	if (way == 0) {

		clock_t StartGenerate = clock();

		int cntGenerate = Generator(FinalSudo, GenerateNum);//������������ģ��
		fwrite(FinalSudo, sizeof(char), cntGenerate * (Sudolen + 1) - 2, WriteP);//��ӡ���վ�
		//CheckSudo(FinalSudo, cntGenerate * (sizeof(tmpSudoMatrix2) + 1) - 2);//���Լ�������վֵĺϷ���

		clock_t EndGenerate = clock();
		printf("Generating Sudoku time cost = %.2fms\n", ((double)(EndGenerate - StartGenerate)));//����������ʾ�ܺ�ʱ

		fclose(WriteP);//���Ĳ����ر��ļ�ָ��
	}
	else if (way == 1) {

		clock_t StartSolve = clock();

		int idx = 0;
		char c = 0;
		clock_t SolveStart = clock();//���������ʱ��ʼ
		while (((c = fgetc(absolute_path_of_puzzlefile)) != EOF) && c) {//��ȡ����

			SudoProblem[idx] = c;
			idx++;
		}

		int flag = Solver(SudoProblem, idx);	//�������

		if (fopen_s(&WriteTheSolution, ".\\sudoku.txt", "w")) {
			printf("��sudoku.txtд�����ս��ǰ�����ļ�ʧ��\n");
			exit(1);
		}
		//���ļ����д���ļ�
		fwrite(SudoProblem, 1, idx, WriteTheSolution);

		clock_t SolveEnd = clock();//���������ʱ����

		printf("Generating Sudoku time cost = %.2fms\n", ((double)(SolveEnd - SolveStart)));
		printf("��ӡ��������Ľ����\n");
		for (int i = 0; i < idx; i++)putchar(SudoProblem[i]);//��ӡ����������Ľ��
	}
}