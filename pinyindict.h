#ifndef PINYINDICT_H
#define PINYINDICT_H

#include <list>
#include <map>


class PinyinDict
{
public:
    PinyinDict();

    int loadFile(const char* filename);
    void printDict();
private:
    std::map<char32_t, std::list<std::string>> data;
};

#endif // PINYINDICT_H
