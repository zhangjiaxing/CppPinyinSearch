#include "pinyindict.h"

#include <iostream>
#include <cctype>
#include <fstream>
#include <cstdlib>



PinyinDict::PinyinDict()
{

}

int PinyinDict::loadFile(const char* filename){
    std::ifstream pinyinFile;
    pinyinFile.open(filename, std::ios::in);
    if (!pinyinFile.is_open()) {
      std::cout << "文件打开失败" << std::endl;
      return 0;
    }

    std::string line;
    while(std::getline(pinyinFile, line)){
        const char* pos = line.c_str();
        while(std::isblank(*pos))
            pos++;
        if(*pos == '#'){
            continue;
        }
        if(*pos == 'U'){
            pos++;

            char *unicodeEnd = nullptr;
            char32_t uniChar = std::strtoul(pos, &unicodeEnd, 16);

            pos = unicodeEnd;
            while(std::isblank(*pos) || *pos == ':'){
                pos++;
            }

            //std::cout << uniChar << ":" << pos << std::endl;

            std::list<std::string> pinyinList;
            const char* PinyinStart = pos;
            while(*pos != '#' && *pos != '\0'){
                pos++;

                if(*pos == ','){
                    std::string pinyin(PinyinStart, pos-PinyinStart);
                    pinyinList.push_back(pinyin);
                    PinyinStart = pos+1;
                }
                while(std::isblank(*pos))  {
                    pos++;
                }
            }

            std::string pinyin(PinyinStart, pos-PinyinStart);
            if(not pinyin.empty()){
                pinyinList.push_back(pinyin);
            }
            if(not pinyinList.empty()){
                this->data[uniChar] = pinyinList;
            }
        }

    }

    return 0;
}

void PinyinDict::printDict(){
    for(auto iter = this->data.cbegin(); iter != this->data.cend(); iter++){
        std::cout << iter->first << ": ";
        auto &pinyinList = iter->second;

        for(auto pinyinIter = pinyinList.cbegin(); pinyinIter != pinyinList.cend(); pinyinIter++){
            std::cout << *pinyinIter << ";";
        }

        std::cout << std::endl;

    }
}
