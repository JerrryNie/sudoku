#include "stdafx.h"
#include "CppUnitTest.h"
#include "../sudoku/sudoku.cpp"/*
#include "../sudoku/pch.cpp"
#include "../sudoku/solver.h"
#include "../sudoku/CheckSudo.h"
#include "../sudoku/generator.h"
#include "../sudoku/InputProcess.h"
#include "../sudoku/TransferModule.h"*/
//#include "../sudoku/Sudoku.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			int status = -1;
			char *argv[3];
			argv[0] = "sudoku.exe";
			argv[1] = "abc";
			argv[2] = "10";
			main(3, argv);
			Assert::AreEqual(status, -1);
		}
		TEST_METHOD(TestMethod2)
		{
			int status = -1;
			char *argv[3];
			argv[0] = "sudoku.exe";
			argv[1] = "-c";
			argv[2] = "10";
			status = main(3, argv);
			Assert::AreEqual(status, 0);
		}

		TEST_METHOD(TestMethod3)
		{
			int status = -1;
			char *argv[3];
			argv[0] = "sudoku.exe";
			argv[1] = "-s";
			argv[2] = "F:\\ProblemSudo.txt";
			status = main(3, argv);
			Assert::AreEqual(status, 0);
		}
		TEST_METHOD(TestMethod4)
		{
			int status = -1;
			char *argv[3];
			argv[0] = "sudoku.exe";
			argv[1] = "-s";
			argv[2] = "F:\\txt";
			status = main(3, argv);
			Assert::AreEqual(status, -1);
		}
		TEST_METHOD(TestMethod5)
		{
			int status = -1;
			char *argv[3];
			argv[0] = "sudoku.exe";
			argv[1] = "-c";
			argv[2] = "abc";
			status = main(3, argv);
			Assert::AreEqual(status, -1);
		}
		TEST_METHOD(TestMethod6)
		{
			int status = -1;
			char *argv[3];
			argv[0] = "sudoku.exe";
			argv[1] = "-s";
			argv[2] = "abc";
			status = main(3, argv);
			Assert::AreEqual(status, -1);
		}
		TEST_METHOD(TestMethod7)
		{
			int status = -1;
			char *argv[3];
			argv[0] = "sudoku.exe";
			argv[1] = "-s";
			argv[2] = "-1";
			status = main(3, argv);
			Assert::AreEqual(status, -1);
		}
		TEST_METHOD(TestMethod8)
		{
			int status = -1;
			char *argv[3];
			argv[0] = "sudoku.exe";
			argv[1] = "-s";
			argv[2] = "-1";
			argv[3] = "000";
			status = main(4, argv);
			Assert::AreEqual(status, -1);
		}
		TEST_METHOD(TestMethod9)
		{
			int status = -1;
			char *argv[3];
			argv[0] = "sudoku.exe";
			argv[1] = " ";
			status = main(2, argv);
			Assert::AreEqual(status, -1);
		}
		TEST_METHOD(TestMethod10)
		{
			int status = -1;
			char *argv[3];
			argv[0] = "sudoku.exe";
			argv[1] = " ";
			status = main(2, argv);
			Assert::AreEqual(status, -1);
		}
	};
}