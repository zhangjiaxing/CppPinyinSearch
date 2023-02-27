#ifndef __STDC_UTF_32__
#define __STDC_UTF_32__
#endif

#include <iostream>
#include <set>
#include <cuchar>
#include <cassert>
#include <climits>
#include "pinyintools.h"
#include "pinyindict.h"
#include "phoneticconvert.h"


Pinyins getPinyinsTone(const PinyinDict *dict, char32_t unichar, ToneType toneType){
    Pinyins pinyins = dict->getPinyins(unichar);
    Pinyins toneList;

    PhoneticConvert *PhoneConverter = PhoneticConvert::instance();
    if(toneType == ToneType::ToneNone){
        std::set<Pinyin> pinyinSet;
        for(Pinyin pinyin : pinyins){
            pinyinSet.insert(PhoneConverter->to_tone0(pinyin.c_str()));
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
            toneList.push_back(PhoneConverter->to_tone2(pinyin.c_str()));
        }
    }else{ //toneType == ToneType::ToneNumber2
        for(Pinyin pinyin : pinyins){
            toneList.push_back(PhoneConverter->to_tone3(pinyin.c_str()));
        }
    }

    return toneList;
}


std::list<Pinyins> getPinyinsList(const PinyinDict *dict, const char *str,  ToneType toneType){
    std::list<Pinyins> pinyinsList;
    std::u32string unistring = stringTou32string(str);
    for(char32_t unichar : unistring){
        pinyinsList.push_back(getPinyinsTone(dict, unichar, toneType));
    }
    return pinyinsList;
}


std::list<Pinyins> getPinyinsList(const PinyinDict *dict, std::u32string_view u32str,  ToneType toneType){
    std::list<Pinyins> pinyinsList;
    for(char32_t unichar : u32str){
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


std::string u32stringTostring(std::u32string_view u32str){
    std::mbstate_t state{};
    char buffer[MB_LEN_MAX]{};
    std::string outstr;
    for(char32_t c32 : u32str)
    {
        std::size_t rc = std::c32rtomb(buffer, c32, &state);
        if(rc != (std::size_t)-1){
            buffer[rc] = '\0';
            outstr.append(buffer);
        }else{
            std::cerr << "u32stringTostring !!! ERROR";
        }
    }
    return outstr;
}


std::u32string stringTou32string(std::string_view str){
    std::mbstate_t state {};
    const char *ptr = str.data();
    const char *end = str.data() + str.size();
    char32_t c32;

    std::u32string u32str;
    while(std::size_t rc = std::mbrtoc32(&c32, ptr, end - ptr, &state))
    {
      assert(rc != (std::size_t)-3); // no surrogates in UTF-32
      if(rc == (std::size_t)-1) break;
      if(rc == (std::size_t)-2) break;
      ptr += rc;
      u32str.push_back(c32);
    }

    return u32str;
}
