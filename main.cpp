#include <iostream>
#include "pinyindict.h"
#include "pinyintools.h"


using namespace std;

int main()
{
    setlocale(LC_ALL, "en_US.utf8");

    PinyinDict pyDic;
    pyDic.loadFile("/home/zhiliao/workspace/CppPinyin/pinyin.txt");

    //auto pinyinsList = getPinyinsList(&pyDic, U"朝辞白帝彩云间，\n千里江陵一日还。");
    //std::cout << "故人西辞黄鹤楼，烟花三月下扬州。" << std::endl;
    //printPinyinsList(pinyinsList);

    printPinyinsList(&pyDic, U"欲穷千里目，\n更上一层楼。", ToneType::ToneNone);
    std::cout << std::endl;
    printPinyinsList(&pyDic, U"欲穷千里目，\n更上一层楼。", ToneType::ToneMark);
    std::cout << std::endl;
    printPinyinsList(&pyDic, U"欲穷千里目，\n更上一层楼。", ToneType::ToneNumber);
    std::cout << std::endl;
    printPinyinsList(&pyDic, U"欲穷千里目，\n更上一层楼。", ToneType::ToneNumber2);
    std::cout << std::endl;
    return 0;
}
