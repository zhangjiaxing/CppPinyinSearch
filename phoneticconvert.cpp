#include "phoneticconvert.h"
#include <iostream>

//const PhoneticConvert::_PhoneticConvertData *PhoneticConvert::phoneticConvertList = nullptr;
std::map<const std::string_view, const char*> *PhoneticConvert::phoneticConvertMap = nullptr;


PhoneticConvert::PhoneticConvert()
{
    // 带声调字符
    static const struct _PhoneticConvertData _phoneticConvertList[] =
    {
        {"ā", "a1"},
        {"á", "a2"},
        {"ǎ", "a3"},
        {"à", "a4"},
        {"ē", "e1"},
        {"é", "e2"},
        {"ě", "e3"},
        {"è", "e4"},
        {"ō", "o1"},
        {"ó", "o2"},
        {"ǒ", "o3"},
        {"ò", "o4"},
        {"ī", "i1"},
        {"í", "i2"},
        {"ǐ", "i3"},
        {"ì", "i4"},
        {"ū", "u1"},
        {"ú", "u2"},
        {"ǔ", "u3"},
        {"ù", "u4"},
        {"ü", "v"},   // üe
        {"ǖ", "v1"},
        {"ǘ", "v2"},
        {"ǚ", "v3"},
        {"ǜ", "v4"},
        {"ń", "n2"},
        {"ň", "n3"},
        {"ǹ", "n4"},
        {"m̄", "m1"},  // len('m̄') == 2
        {"ḿ", "m2"},
        {"m̀", "m4"},  // len("m̀") == 2
        {"ê̄", "ê1"},  // len('ê̄') == 2
        {"ế", "ê2"},
        {"ê̌", "ê3"},  // len('ê̌') == 2
        {"ề", "ê4"},
    };

    //PhoneticConvert::phoneticConvertList = _phoneticConvertList;
    PhoneticConvert::phoneticConvertMap = new std::map<const std::string_view, const char*>;
    for(struct _PhoneticConvertData d : _phoneticConvertList){
        (*PhoneticConvert::phoneticConvertMap)[d.from] = d.to;
    }
}


std::string PhoneticConvert::to_tone2(const char *pinyin){
    std::string tone2;
    const char *p = pinyin;
    while(*p != '\0'){
        std::string_view yin(p, 2);
        std::string_view yin2(p, 3);

        auto iter = PhoneticConvert::phoneticConvertMap->find(yin);
        if(iter != PhoneticConvert::phoneticConvertMap->cend()){
            tone2.append(iter->second);
            p+= iter->first.length();
            continue;
        }

        auto iter2 = PhoneticConvert::phoneticConvertMap->find(yin2);
        if(iter2 != PhoneticConvert::phoneticConvertMap->cend()){
            tone2.append(iter2->second);
            p+= iter2->first.length();
            continue;
        }

        tone2.push_back(*p);
        p++;
    }
    return tone2;
}


std::string PhoneticConvert::to_tone3(const char *pinyin){

}

