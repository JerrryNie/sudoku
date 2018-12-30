//���ļ�������ģ�飬�����������������ȷ���������еķ�ʽ(���ɻ����),�Բ���way����
#include "pch.h"
#include "InputProcess.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <ctime>


const static int MaxSudo = 1000000;



static bool CheckValid(char * str) {//�����������Ƿ�Ϊint

	int len = (int)strlen(str);
	for (int i = 0; i < len; i++) {
		if (!(str[i] >= '0' && str[i] <= '9'))
			return false;
	}
	return true;
}

int InputProcess(int argc, char** argv, int &way, int &GenerateNum, FILE* &WriteP, FILE* &absolute_path_of_puzzlefile) {

	if (argc == 3 && !strcmp("-c", argv[1]) && strlen(argv[1]) == 2) {//-c

		if (!CheckValid(argv[2])) {	//���ڶ��������Ƿ�Ϊ�Ϸ�����
			printf("���������վֲ������Ϸ���\n");
			return -1;
			//exit(1);
		}
		else {
			GenerateNum = atoi(argv[2]);
			if (GenerateNum > MaxSudo) {
				printf("������������վ���������\n");
				return -1;
				//exit(1);
			}
		}
		if (fopen_s(&WriteP, "sudoku.txt", "w")) {//��Ҫд���ļ�
			printf("sudoku.txt��ʧ��\n");
			return -1;
			//exit(1);
		}
		way = 0;
	}
	else if (argc == 3 && !strcmp("-s", argv[1]) && strlen(argv[1]) == 2) {//-s

		//printf("����һ��������������Ĳ��Է�֧!��\n");
		if (fopen_s(&absolute_path_of_puzzlefile, argv[2], "r")) {
			printf("sudoku.txt��ʧ��\n");
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