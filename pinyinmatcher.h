#ifndef PINYINMATCHER_H
#define PINYINMATCHER_H

#include "pinyindict.h"


class PinyinMatcher
{
public:
    PinyinMatcher();
    ~PinyinMatcher();

    int loadDictFile(const char* filename);



    static int matchText(const PinyinDict *dict, const char*pinyinSequence, const std::u32string_view &text);
    static int matchTextList(const PinyinDict *dict, const char *pinyinSequence, std::list<const std::u32string_view> &textList);

private:
    PinyinDict dict;
};

#endif // PINYINMATCHER_H
