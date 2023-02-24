#ifndef PINYINTOOLS_H
#define PINYINTOOLS_H

#include <pinyincommon.h>
#include <pinyindict.h>


enum ToneType {
    ToneNone, //张zhang
    ToneMark, //张zhāng
    ToneNumber, //张zha1ng
    ToneNumber2, //张zhang1
};


Pinyins getPinyinsTone(const PinyinDict *dict, char32_t unichar, ToneType toneType=ToneNone);

std::list<Pinyins> getPinyinsList(const PinyinDict *dict, std::u32string_view unistring, ToneType toneType=ToneNone);

void printPinyins(Pinyins pinyins);
void printPinyinsList(std::list<Pinyins> pinyinsList);
void printPinyinsList(const PinyinDict *dict, std::u32string_view unistring, ToneType toneType=ToneNone);


#endif // PINYINTOOLS_H
