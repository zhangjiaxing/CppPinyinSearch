#include <iostream>
#include "pinyindict.h"
#include "phoneticconvert.h"
#include "pinyintools.h"



using namespace std;

int main()
{
    setlocale(LC_ALL, "en_US.utf8");

    PinyinDict pyDic;
    pyDic.loadFile("/home/zhiliao/workspace/CppPinyin/pinyin.txt");

    //py.printDict();
    Pinyins pinyins = pyDic.getPinyins(U'张');
    //py.printPinyins(pinyins);
    std::cout << "pinyin:\t" << pinyins.back() << std::endl;

    PhoneticConvert pconv;
    auto tone2 = pconv.to_tone2(pinyins.back().data());
    std::cout << "tone2:\t" << tone2 << std::endl;

    auto tone3 = pconv.to_tone3(pinyins.back().data());
    std::cout << "tone3:\t" << tone3 << std::endl;
    std::cout << std::endl;

    //auto pinyinsList = getPinyinsTone3List(&pyDic, U"故人西辞黄鹤楼，烟花三月下扬州。");
    //std::cout << "故人西辞黄鹤楼，烟花三月下扬州。" << std::endl;
    //printPinyinsList(pinyinsList);

    printPinyinsList(&pyDic, U"春江潮水连海平，\n海上明月共潮生。");
    std::cout << std::endl;
    return 0;
}
