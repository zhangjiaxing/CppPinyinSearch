#ifndef PINYINDICT_H
#define PINYINDICT_H

#include <list>
#include <map>
#include "pinyincommon.h"


class PinyinDict
{
public:

    PinyinDict();
    ~PinyinDict();

    int loadFile(const char* filename);
    Pinyins getPinyins(char32_t unichar) const;
    void printDict();
private:
    std::map<char32_t, Pinyins> data;

};

#endif // PINYINDICT_H
