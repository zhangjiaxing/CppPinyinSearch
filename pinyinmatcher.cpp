#include <cstring>
#include <iostream>
#include "pinyinmatcher.h"
#include "pinyintools.h"
#include "phoneticconvert.h"


PinyinMatcher::PinyinMatcher(){

}

PinyinMatcher::~PinyinMatcher(){

}

int PinyinMatcher::loadDictFile(const char* filename){
    return this->dict.loadFile(filename);
}


int PinyinMatcher::matchText(const char *pinyinSequence, const std::u32string_view &text){
    const char *pinyinSequenceEnd = strchr(pinyinSequence, '\0');
    std::list<Pinyins> pinyinsList = getPinyinsList(&this->dict, text, ToneType::ToneNone);

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


int PinyinMatcher::matchTextList(const char *pinyinSequence, std::list<const std::u32string_view> &textList){
    int matchCount = 0;
    for(const std::u32string_view text: textList){
        int ret = this->matchText(pinyinSequence, text);
        if(ret == 0){

            std::cout << "match: " << fromString32(text) << std::endl;

            matchCount++;
        }
    }
    std::cout << std::endl;
    return matchCount;
}

