#pragma once
#ifndef INPUTPRCESS_H
#define INPUTPRCESS_H
#include<cstdlib>//为了使FILE*通过编译
#include<cstdio>
int InputProcess(int argc, char** argv, int &way, int &GenerateNum, FILE* &WriteP, FILE* &absolute_path_of_puzzlefile);
#endif // !INPUTPRCESS_H
