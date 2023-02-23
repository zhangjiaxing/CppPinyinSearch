#ifndef PHONETICCONVERT_H
#define PHONETICCONVERT_H

#include <string>
#include <map>


class PhoneticConvert
{
public:
    PhoneticConvert();

    static std::string to_tone2(const char *pinyin);
    static std::string to_tone3(const char *pinyin);

private:
    struct _PhoneticConvertData{
        const char* from;
        const char* to;
    };

    //static const struct _PhoneticConvertData *phoneticConvertList;
    static std::map<const std::string_view, const char*> *phoneticConvertMap;

};

#endif // PHONETICCONVERT_H
