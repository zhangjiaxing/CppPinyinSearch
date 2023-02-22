#ifndef PINYINDICT_H
#define PINYINDICT_H

#include <list>
#include <map>


class PinyinDict
{
public:
    typedef std::string Pinyin;
    typedef std::list<std::string> Pinyins;

    PinyinDict();

    int loadFile(const char* filename);
    void printDict();
    Pinyins getPinyins(char32_t unichar);
    std::list<Pinyins> getPinyinsList(std::u32string_view unistring);

private:
    std::map<char32_t, Pinyins> data;
};

#endif // PINYINDICT_H
