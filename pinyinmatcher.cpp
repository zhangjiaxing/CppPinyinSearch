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


int PinyinMatcher::addText(std::u32string_view u32text, const char32_t *referer){
    std::cout << u32stringTostring(u32text) << std::endl;
    return this->_addText(this->searchTree, u32text, referer);
}


std::list<char32_t *> searchText(const char*pinyinSequence, int limit){

}


std::list<PinyinSearchTree *> PinyinMatcher::_searchText(PinyinSearchTree *pos, const char*pinyinSequence){
    if(pos == nullptr || pinyinSequence == nullptr){
        return{};
    }
    if(*pinyinSequence == '\0'){
        return {pos};
    }

    std::list<PinyinSearchTree*> nodeList;

    for(int i=0; i<26; i++){
        if(pos->code[i] != nullptr){
            if( i-'a' == *pinyinSequence){
                std::list<PinyinSearchTree*> matchList = this->_searchText(pos->code[i], pinyinSequence+1);
                nodeList.merge(matchList);
            }else{
                std::list<PinyinSearchTree*> matchList = this->_searchText(pos->code[i], pinyinSequence);
                nodeList.merge(matchList);
            }
        }
    }

    return nodeList;
}


void PinyinMatcher::printTree(){
    std::cout << std::endl << "printTree" << std::endl;
    this->_printTree(this->searchTree);
}


void PinyinMatcher::_printTree(PinyinSearchTree *pos){
    if(pos == nullptr){
        return;
    }
    if(pos->text != nullptr){
        std::cout << "[" << u32stringTostring(pos->text) << "]" << std::endl;
    }

    for(int i=0; i<26; i++){
        if(pos->code[i] != nullptr){
            std::cout << char('a' + i) << pos->code[i]->ref << " ";
            _printTree(pos->code[i]);
        }
    }
}


int PinyinMatcher::_addText(PinyinSearchTree *head, std::u32string_view u32text, const char32_t *referer){
    if(u32text.empty()){
        return 0;
    }
    //std::cout << "_addText " << u32stringTostring(u32text) << std::endl;
    std::u32string_view u32forward = u32text.substr(1);

    char32_t c32 = u32text.front();
    Pinyins pinyins = getPinyinsTone(this->dict, c32);
    for(Pinyin pinyin : pinyins){
        PinyinSearchTree *pinyinPos = head;
        auto iter = pinyin.cbegin();
        while(iter != pinyin.cend()){
            char yin = *iter;
            if(pinyinPos->code[yin - 'a'] == nullptr){
                PinyinSearchTree *node = new PinyinSearchTree();
                memset(node, 0, sizeof *node);
                pinyinPos->code[yin - 'a'] = node;
            }
            pinyinPos->ref++;
            pinyinPos = pinyinPos->code[yin - 'a'];
            iter++;
        }
        if(u32forward.length() == 0){
            pinyinPos->text = referer;
            //std::cout << "text = referer " << referer << u32stringTostring(referer) << std::endl;
        }else{
            this->_addText(pinyinPos, u32forward, referer);
        }
    }

    if(pinyins.size() == 0){
        if(u32forward.length() != 0){
            this->_addText(head, u32forward, referer);
        }else{
            head->text = referer;
            //std::cout << "text = referer " << referer << u32stringTostring(referer) << std::endl;
        }
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

