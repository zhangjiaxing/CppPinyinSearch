#ifndef PINYINMATCHER_H
#define PINYINMATCHER_H

#include "pinyindict.h"


struct PinyinSearchTree {
    struct PinyinSearchTree *code[26];
    uint64_t ref;
    const char32_t *text;
};


class PinyinMatcher
{
public:
    PinyinMatcher(const PinyinDict *dict);
    ~PinyinMatcher();

    int addText(std::u32string_view u32text, const char32_t *referer);

    void printTree();
    void _printTree(PinyinSearchTree *pos);

    static int matchText(const PinyinDict *dict, const char*pinyinSequence, const char *text);
    static int matchTextList(const PinyinDict *dict, const char *pinyinSequence, std::list<const char *> &textList);

private:
    int _addText(PinyinSearchTree *head, std::u32string_view u32text, const char32_t *referer);

    const PinyinDict *dict{};
    PinyinSearchTree *searchTree{};
};

#endif // PINYINMATCHER_H
