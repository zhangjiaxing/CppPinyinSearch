#include <cstring>
#include <iostream>
#include "pinyinmatcher.h"
#include "pinyintools.h"
#include "phoneticconvert.h"


PinyinMatcher::PinyinMatcher(const PinyinDict *dict)
{
    this->dict = dict;
    this->searchTree = new PinyinSearchTree();
    memset(this->searchTree, 0, sizeof *this->searchTree);
}


PinyinMatcher::~PinyinMatcher(){

}


int PinyinMatcher::addText(std::u32string_view u32text){
    std::cout << u32stringTostring(u32text) << std::endl;
    return this->_addText(this->searchTree, u32text);
}


int PinyinMatcher::_addText(PinyinSearchTree *tree, std::u32string_view u32text){
    PinyinSearchTree *pos = tree;

    if(u32text.empty()){
        return 0;
    }

    std::u32string_view u32forward = u32text.substr(1);
    char32_t c32 = u32text.front();
    Pinyins pinyins = getPinyinsTone(this->dict, c32);
    for(Pinyin pinyin : pinyins){
        auto iter = pinyin.cbegin();
        while(iter != pinyin.cend()){
            char yin = *iter;
            if(pos->code[yin - 'a'] == nullptr){
                PinyinSearchTree *node = new PinyinSearchTree();
                memset(tree, 0, sizeof *node);
                pos->code[yin - 'a'] = node;
            }
            pos->ref++;
            pos = pos->code[yin - 'a'];
            iter++;
        }
        this->_addText(pos, u32forward);
    }
    if(pinyins.empty()){
        this->_addText(pos, u32forward);
    }

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

