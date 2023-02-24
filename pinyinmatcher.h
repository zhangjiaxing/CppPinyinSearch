#ifndef PINYINMATCHER_H
#define PINYINMATCHER_H

#include "pinyindict.h"


class PinyinMatcher
{
public:
    PinyinMatcher();
    ~PinyinMatcher();

    int loadDictFile(const char* filename);
    int matchText(const char*pinyinSequence, const std::u32string_view text);

private:
    PinyinDict dict;
};

#endif // PINYINMATCHER_H
