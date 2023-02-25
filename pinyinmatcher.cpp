#include <cstring>
#include <iostream>
#include "pinyinmatcher.h"
#include "pinyintools.h"
#include "phoneticconvert.h"


PinyinMatcher::PinyinMatcher(const PinyinDict *dict)
{
    this->dict = dict;
}


PinyinMatcher::~PinyinMatcher(){

}


int PinyinMatcher::addText(const char *text){
    std::u32string u32Text = stringToU32string(text);
    printPinyinsList(this->dict, u32Text);
    return 0;
}



int PinyinMatcher::matchText(const PinyinDict *dict, const char *pinyinSequence, const char *text){
    const char *pinyinSequenceEnd = strchr(pinyinSequence, '\0');
    std::list<Pinyins> pinyinsList = getPinyinsList(dict, text, ToneType::ToneNone);

    const char *matchPos = pinyinSequence;
    for (Pinyins pinyins : pinyinsList){
        const char *matchPtr = matchPos;
        const char *matchMax = matchPos;

        for(Pinyin pinyin : pinyins){
            const char *pos = pinyin.c_str();
            //std::cout << "> pos = " << pos << std::endl;

            while(true){
                pos = strchr(pos, *matchPtr);

                if(pos != nullptr && *pos != '\0'){
                    //std::cout << "|" << pinyin << " "<< *matchPtr << " pos: " << *pos << std::endl;
                    matchPtr++;
                    pos++;
                    //std::cout << "|" << pinyin << " "<< *matchPtr << " pos: " << *pos << std::endl;

                }else{
                    //std::cout << "break" << std::endl;
                    break;
                }
            }
            //std::cout << " matchPtr matchMax : " << *matchPtr << *matchMax << std::endl;
            if(matchPtr > matchMax){
                //std::cout << "> matchPtr > matchMax : " << *matchPtr << " > " << *matchMax << std::endl;
                matchMax = matchPtr;
            }
        }
        matchPos = matchMax;
    }

    return pinyinSequenceEnd - matchPos;
}


int PinyinMatcher::matchTextList(const PinyinDict *dict, const char *pinyinSequence, std::list<const char *> &textList){
    int matchCount = 0;
    for(const char *text: textList){
        int ret = PinyinMatcher::matchText(dict, pinyinSequence, text);
        if(ret == 0){
            std::cout << "match: " << text << std::endl;
            matchCount++;
        }
    }
    std::cout << std::endl;
    return matchCount;
}

