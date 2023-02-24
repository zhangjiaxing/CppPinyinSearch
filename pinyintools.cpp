#ifndef __STDC_UTF_32__
#define __STDC_UTF_32__
#endif

#include <iostream>
#include <uchar.h>
#include "pinyintools.h"
#include "pinyindict.h"
#include "phoneticconvert.h"



Pinyins getPinyinsTone2(const PinyinDict *dict, char32_t unichar){
    Pinyins pinyins = dict->getPinyins(unichar);
    Pinyins tone2List;
    PhoneticConvert phoneticConv;
    for(Pinyin pinyin : pinyins){
        tone2List.push_back(phoneticConv.to_tone2(pinyin.c_str()));
    }
    return tone2List;
}


Pinyins getPinyinsTone3(const PinyinDict *dict, char32_t unichar){
    Pinyins pinyins = dict->getPinyins(unichar);
    Pinyins tone3List;
    PhoneticConvert phoneticConv;
    for(Pinyin pinyin : pinyins){
        tone3List.push_back(phoneticConv.to_tone3(pinyin.c_str()));
    }
    return tone3List;
}


std::list<Pinyins> getPinyinsList(const PinyinDict *dict, std::u32string_view unistring){
    std::list<Pinyins> pinyinsList;
    for(char32_t unichar : unistring){
        pinyinsList.push_back(dict->getPinyins(unichar));
    }
    return pinyinsList;
}

std::list<Pinyins> getPinyinsTone2List(const PinyinDict *dict, std::u32string_view unistring){
    std::list<Pinyins> pinyinsList;
    for(char32_t unichar : unistring){
        pinyinsList.push_back(getPinyinsTone2(dict, unichar));
    }
    return pinyinsList;
}


std::list<Pinyins> getPinyinsTone3List(const PinyinDict *dict, std::u32string_view unistring){
    std::list<Pinyins> pinyinsList;
    for(char32_t unichar : unistring){
        pinyinsList.push_back(getPinyinsTone3(dict, unichar));
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


void printPinyinsList(const PinyinDict *dict, std::u32string_view unistring){
    mbstate_t state{};
    char outstr[MB_CUR_MAX+1];
    for(char32_t unichar : unistring){
        int rc = c32rtomb(outstr, unichar, &state);
        outstr[std::max(rc, 0)] = '\0';
        std::cout << outstr;
        Pinyins pinyins = getPinyinsTone3(dict, unichar);
        for(Pinyin pinyin : pinyins){
            std::cout << pinyin << " ";
        }
    }
    std::cout << std::endl;
}


