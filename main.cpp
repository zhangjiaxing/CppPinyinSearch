#include <iostream>
#include "pinyindict.h"
#include "pinyintools.h"
#include "pinyinmatcher.h"


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

    PinyinMatcher matcher;
    matcher.loadDictFile("/home/zhiliao/workspace/CppPinyin/pinyin.txt");

    //int matchRet = matcher.matchText("yqongqlmlouabc", std::u32string_view(U"欲穷千里目，\n更上一层楼。"));
    //std::cout << "match: " << matchRet;

    std::list<const std::u32string_view> nameList;
    nameList.push_back(U"蓝牙");
    nameList.push_back(U"贪玩蓝月");
    nameList.push_back(U"琅琊");
    nameList.push_back(U"利用");
    nameList.push_back(U"阿姨");
    nameList.push_back(U"你好哇");


    matcher.matchTextList(&pyDic, "lay", nameList);

    std::cout << std::endl;

    return 0;
}
