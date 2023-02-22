#ifndef PHONETICCONVERT_H
#define PHONETICCONVERT_H

#include <string>


class PhoneticConvert
{
public:
    PhoneticConvert();


private:
    struct _PhoneticConvertData{
        std::string from;
        std::string to;
    };
    static const struct _PhoneticConvertData *phoneticConvertList;

};

#endif // PHONETICCONVERT_H
