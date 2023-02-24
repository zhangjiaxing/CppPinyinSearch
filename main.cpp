#include <iostream>
#include "pinyindict.h"
#include "phoneticconvert.h"



using namespace std;

int main()
{
    PinyinDict py;
    py.loadFile("/home/zhiliao/workspace/CppPinyin/pinyin.txt");

    //py.printDict();
    PinyinDict::Pinyins pinyins = py.getPinyins(U'张');
    //py.printPinyins(pinyins);
    std::cout << "pinyin:\t" << pinyins.back() << std::endl;

    PhoneticConvert pconv;
    auto tone2 = pconv.to_tone2(pinyins.back().c_str());
    std::cout << "tone2:\t" << tone2 << std::endl;

    auto tone3 = pconv.to_tone3(pinyins.back().c_str());
    std::cout << "tone3:\t" << tone3 << std::endl;


    auto pinyinsList = py.getPinyinsList(U"故人西辞黄鹤楼，烟花三月下扬州。");

    std::cout << "故人西辞黄鹤楼，烟花三月下扬州。" << std::endl;
    for(PinyinDict::Pinyins pinyins : pinyinsList){
        py.printPinyins(pinyins);
    }
    std::cout << std::endl;

    return 0;
}
