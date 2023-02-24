#ifndef __STDC_UTF_32__
#define __STDC_UTF_32__
#endif

#include <iostream>
#include <set>
#include <uchar.h>
#include "pinyintools.h"
#include "pinyindict.h"
#include "phoneticconvert.h"


Pinyins getPinyinsTone(const PinyinDict *dict, char32_t unichar, ToneType toneType){
    Pinyins pinyins = dict->getPinyins(unichar);
    Pinyins toneList;
    PhoneticConvert phoneticConv;

    if(toneType == ToneType::ToneNone){
        std::set<Pinyin> pinyinSet;
        for(Pinyin pinyin : pinyins){
            pinyinSet.insert(phoneticConv.to_tone0(pinyin.c_str()));
        }
        for(Pinyin pinyin : pinyinSet){
            toneList.push_back(pinyin);
        }
    }else if(toneType == ToneType::ToneMark){
        for(Pinyin pinyin : pinyins){
            toneList.push_back(pinyin.c_str());
        }
    }else if(toneType == ToneType::ToneNumber){
        for(Pinyin pinyin : pinyins){
            toneList.push_back(phoneticConv.to_tone2(pinyin.c_str()));
        }
    }else{ //toneType == ToneType::ToneNumber2
        for(Pinyin pinyin : pinyins){
            toneList.push_back(phoneticConv.to_tone3(pinyin.c_str()));
        }
    }

    return toneList;
}


std::list<Pinyins> getPinyinsList(const PinyinDict *dict, std::u32string_view unistring,  ToneType toneType){
    std::list<Pinyins> pinyinsList;
    for(char32_t unichar : unistring){
        pinyinsList.push_back(getPinyinsTone(dict, unichar, toneType));
    }
    return pinyinsList;
}


void printPinyins(Pinyins pinyins){
    for(Pinyin pinyin : pinyins){
        std::cout << pinyin << " ";
    }
    std::cout << " ";
}


void printPinyinsList(std::list<Pinyins> pinyinsList){
    for(Pinyins pinyins : pinyinsList){
        printPinyins(pinyins);
    }
    std::cout << std::endl;
}


void printPinyinsList(const PinyinDict *dict, std::u32string_view unistring, ToneType toneType){
    mbstate_t state{};
    char outstr[MB_CUR_MAX+1];
    for(char32_t unichar : unistring){
        int rc = c32rtomb(outstr, unichar, &state);
        outstr[std::max(rc, 0)] = '\0';
        std::cout << outstr;
        Pinyins pinyins = getPinyinsTone(dict, unichar, toneType);
        for(Pinyin pinyin : pinyins){
            std::cout << pinyin << " ";
        }
    }
    std::cout << std::endl;
}


