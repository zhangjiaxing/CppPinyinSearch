#include <iostream>
#include "pinyindict.h"



using namespace std;

int main()
{

    cout << "Hello World!" << endl;

    PinyinDict py;
    py.loadFile("/home/zhiliao/source/python-pinyin/pinyin-data/pinyin.txt");
    py.printDict();
    return 0;
}
