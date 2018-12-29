软件工程个人项目-1120161722
=========================
*   运行环境：
    *   64&ensp;bit Windows 10操作系统
*   运行方式：
    *   命令行
*   使用流程：
    *   1.命令行进入 \BIN 文件夹
    *   2.若使用生成数独功能，则输入：`sudoku.exe -c NUM` 
        *   其中NUM为具体的生成数独终局数量
        *   最终输出的终局文件`sudoku.txt`将存放在与sudoku.exe同目录下  
    *   3.若使用求解数独功能，则输入：`sudoku.exe -s absolute_path_of_puzzlefile`
        *   其中`absolute_path_of_puzzlefile`是读取待解数独文件的绝对路径名
        *   最终的数独求解结果将输出到文件`sudoku.txt`中，其同样也放置在于`sudoku.exe`文件的相同目录下
