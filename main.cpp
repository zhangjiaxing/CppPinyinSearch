#include <iostream>
#include <fstream>
#include "pinyindict.h"
#include "pinyintools.h"
#include "pinyinmatcher.h"


using namespace std;


void testPrintPinyinsList(PinyinDict *dict){
    //auto pinyinsList = getPinyinsList(&pyDic, U"朝辞白帝彩云间，\n千里江陵一日还。");
    //std::cout << "故人西辞黄鹤楼，烟花三月下扬州。" << std::endl;
    //printPinyinsList(pinyinsList);

    printPinyinsList(dict, U"欲穷千里目，\n更上一层楼。", ToneType::ToneNone);
    std::cout << std::endl;
    printPinyinsList(dict, U"欲穷千里目，\n更上一层楼。", ToneType::ToneMark);
    std::cout << std::endl;
    printPinyinsList(dict, U"欲穷千里目，\n更上一层楼。", ToneType::ToneNumber);
    std::cout << std::endl;
    printPinyinsList(dict, U"欲穷千里目，\n更上一层楼。", ToneType::ToneNumber2);
    std::cout << std::endl;
}


void testMatchTextList(PinyinDict *dict){
    PinyinMatcher matcher(dict);

    int matchRet = matcher.matchText(dict, "yqongqlmlouabc", "欲穷千里目，\n更上一层楼。");
    std::cout << "match: " << matchRet;

    std::list<const char*> nameList;;
    nameList.push_back("琅琊");
    nameList.push_back("利用");
    nameList.push_back("阿姨");
    nameList.push_back("你好哇");
    matcher.matchTextList(dict, "lay", nameList);
    std::cout << std::endl;
}


void testMatcher(PinyinDict *dict){
    PinyinMatcher matcher(dict);

    const char32_t *s32 = U"朝辞白帝彩云间，\n千里江陵一日还。";
    matcher.addText(s32, s32);

    char32_t chengyuList[] = U"爱屋及乌 爱惜羽毛 爱憎分明 碍手碍脚 碍足碍手 安邦定国 安不忘危 安家乐业 安家立业 安家落户 安居乐业 安老怀少 ";
    std::u32string chengyu;
    char32_t *chengyuPos = chengyuList;
    while(*chengyuPos != U'\0'){
        //std::cout << chengyuPos;
        if(*chengyuPos == U' '){
            *chengyuPos = U'\0';
            matcher.addText(chengyu, chengyuPos - chengyu.length());
            chengyu.clear();
        }else{
            chengyu.push_back(*chengyuPos);
        }

        chengyuPos++;
    }

    matcher.printTree();

    std::list<const char32_t *> resultList = matcher.searchText("alaohs", 10);
    for(const char32_t *text: resultList){
        std::cout << u32stringTostring(text) << std::endl;
    }
    std::cout << std::endl;
}


void testChengyusousuo(PinyinDict *dict){

    std::list<std::u32string> chengyuList{};

    std::ifstream chengyuFile;
    chengyuFile.open("/home/zhiliao/workspace/CppPinyin/test/chengyu.txt", std::ios::in);
    if (!chengyuFile.is_open()) {
      std::cout << "文件打开失败" << std::endl;
      return;
    }

    std::string line;
    while(std::getline(chengyuFile, line)){
        const std::u32string &chengyu = stringTou32string(line);
        chengyuList.push_back(chengyu);
    }

    PinyinMatcher matcher(dict);
    for(const std::u32string &chengyu : chengyuList){
        matcher.addText(chengyu, chengyu.data());
    }

    std::list<const char32_t *> resultList = matcher.searchText("zhutou", 10);
    for(const char32_t *text: resultList){
        std::cout << u32stringTostring(text) << std::endl;
    }
    std::cout << std::endl;
}


void testChengyujielong(PinyinDict *dict){
    std::list<std::u32string> chengyuList{};

    std::ifstream chengyuFile;
    chengyuFile.open("/home/zhiliao/workspace/CppPinyin/test/chengyu.txt", std::ios::in);
    if (!chengyuFile.is_open()) {
      std::cout << "文件打开失败" << std::endl;
      return;
    }

    std::string line;
    while(std::getline(chengyuFile, line)){
        const std::u32string &chengyu = stringTou32string(line);
        chengyuList.push_back(chengyu);
    }

    PinyinMatcher matcher(dict);
    for(const std::u32string &chengyu : chengyuList){
        matcher.addText(chengyu, chengyu.data());
    }

    while(true){
        string hanzi;
        cout << "请输入需要接龙的成语或汉字：";
        if(not (cin >> hanzi)){
            break;
        }

        u32string u32hanzi = stringTou32string(hanzi);
        Pinyins pinyins = getPinyinsTone(dict, u32hanzi.back());
        if(pinyins.size() > 1){
            cout << u32stringTostring(&u32hanzi[u32hanzi.length()-1]) << "是多音字！" << endl;
        }
        for(Pinyin pinyin : pinyins){
            cout << pinyin << "开头的成语：" << endl;
            std::list<const char32_t *> resultList = matcher.searchText(pinyin.c_str(), 10);
            for(const char32_t *text: resultList){
                std::cout << u32stringTostring(text) << std::endl;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}


int main()
{
    std::setlocale(LC_ALL, "en_US.utf8");

    PinyinDict dict;
    dict.loadFile("/home/zhiliao/workspace/CppPinyin/pinyin.txt");

    //testPrintPinyinsList(&dict);
    //testMatchTextList(&dict);
    //testMatcher(&dict);
    //testChengyusousuo(&dict);
    testChengyujielong(&dict);

    return 0;
}
