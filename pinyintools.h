#ifndef PINYINTOOLS_H
#define PINYINTOOLS_H

#include <pinyin_common.h>
#include <pinyindict.h>


Pinyins getPinyinsTone2(const PinyinDict *dict, char32_t unichar);
Pinyins getPinyinsTone3(const PinyinDict *dict, char32_t unichar);

std::list<Pinyins> getPinyinsList(const PinyinDict *dict, std::u32string_view unistring);
std::list<Pinyins> getPinyinsTone2List(const PinyinDict *dict, std::u32string_view unistring);
std::list<Pinyins> getPinyinsTone3List(const PinyinDict *dict, std::u32string_view unistring);

void printPinyins(Pinyins pinyins);
void printPinyinsList(std::list<Pinyins> pinyinsList);
void printPinyinsList(const PinyinDict *dict, std::u32string_view unistring);



#endif // PINYINTOOLS_H
