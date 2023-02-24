#ifndef PHONETICCONVERT_H
#define PHONETICCONVERT_H

#include <string>
#include <map>
#include "pinyincommon.h"


class PhoneticConvert
{
public:
    PhoneticConvert();

    static Pinyin to_tone0(const char *pinyin);
    static Pinyin to_tone2(const char *pinyin);
    static Pinyin to_tone3(const char *pinyin);

private:
    struct _PhoneticConvertData{
        const char* from;
        const char* to;
    };

    //static const struct _PhoneticConvertData *phoneticConvertList;
    static std::map<const std::string_view, const char*> *phoneticConvertMap;

};

#endif // PHONETICCONVERT_H
