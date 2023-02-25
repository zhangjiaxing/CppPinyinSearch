#ifndef PINYINMATCHER_H
#define PINYINMATCHER_H

#include "pinyindict.h"


struct PinyinSearchCache {
    struct PinyinSearchCache *code[26];
    uint64_t ref;
    void *data;
};


class PinyinMatcher
{
public:
    PinyinMatcher(const PinyinDict *dict);
    ~PinyinMatcher();

    int addText(const char *text);


    static int matchText(const PinyinDict *dict, const char*pinyinSequence, const char *text);
    static int matchTextList(const PinyinDict *dict, const char *pinyinSequence, std::list<const char *> &textList);

private:
    const PinyinDict *dict;
};

#endif // PINYINMATCHER_H
