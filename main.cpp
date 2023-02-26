#include <iostream>
#include "pinyindict.h"
#include "pinyintools.h"
#include "pinyinmatcher.h"


using namespace std;

int main()
{
    std::setlocale(LC_ALL, "en_US.utf8");

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

    PinyinMatcher matcher(&pyDic);

    //int matchRet = matcher.matchText("yqongqlmlouabc", std::u32string_view(U"欲穷千里目，\n更上一层楼。"));
    //std::cout << "match: " << matchRet;

    std::list<const char*> nameList;
    nameList.push_back("蓝牙");
    nameList.push_back("贪玩蓝月");
    nameList.push_back("琅琊");
    nameList.push_back("利用");
    nameList.push_back("阿姨");
    nameList.push_back("你好哇");


    matcher.matchTextList(&pyDic, "lay", nameList);

    const char32_t *s32 = U"朝辞白帝彩云间，\n千里江陵一日还。";
    matcher.addText(s32, s32);
    const char32_t *chengyuList = U"爱屋及乌 爱惜羽毛 爱憎分明 碍手碍脚 碍足碍手 安邦定国 安不忘危 安家乐业 安家立业 安家落户 安居乐业 安老怀少";
    std::u32string chengyu;
    const char32_t *chengyuPos = chengyuList;
    while(*chengyuPos != U'\0'){
        //std::cout << chengyuPos;
        if(*chengyuPos == U' '){
            matcher.addText(chengyu, chengyuPos);
            chengyu.clear();
        }else{
            chengyu.push_back(*chengyuPos);
        }

        chengyuPos++;
    }

    std::cout << std::endl;


    return 0;
}
