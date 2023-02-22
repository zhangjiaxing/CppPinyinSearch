#include <iostream>
#include "pinyindict.h"



using namespace std;

int main()
{
    cout << "Hello World!" << endl;

    PinyinDict py;
    py.loadFile("/home/zhiliao/source/python-pinyin/pinyin-data/pinyin.txt");
    py.printDict();

    std::list<std::string> pinyins = py.getPinyins(U'长');

    for(std::string pinyin : pinyins){
        std::cout << pinyin << ".";
    }
    std::cout << "\n";
    return 0;
}
