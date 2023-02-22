#include <iostream>
#include "pinyindict.h"



using namespace std;

int main()
{
    cout << "Hello World!" << endl;

    PinyinDict py;
    py.loadFile("/home/zhiliao/source/python-pinyin/pinyin-data/pinyin.txt");
    //py.printDict();

    //PinyinDict::Pinyins pinyins = py.getPinyins(U'长');
    //py.printPinyins(pinyins);

    auto pinyinsList = py.getPinyinsList(U"故人西辞黄鹤楼，烟花三月下扬州。");

    std::cout << "故人西辞黄鹤楼，烟花三月下扬州。" << std::endl;
    for(PinyinDict::Pinyins pinyins : pinyinsList){
        py.printPinyins(pinyins);
    }
    std::cout << std::endl;

    return 0;
}
