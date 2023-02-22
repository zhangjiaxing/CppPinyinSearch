#include "phoneticconvert.h"

const PhoneticConvert::_PhoneticConvertData *PhoneticConvert::phoneticConvertList = nullptr;

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
        {nullptr, nullptr},
    };

    PhoneticConvert::phoneticConvertList = _phoneticConvertList;
}
