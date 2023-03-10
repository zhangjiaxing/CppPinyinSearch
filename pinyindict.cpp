#include "pinyindict.h"

#include <iostream>
#include <cctype>
#include <fstream>
#include <cstdlib>



PinyinDict::PinyinDict()
{

}

PinyinDict::~PinyinDict(){
    //TODO
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

            Pinyins pinyinList;
            const char* PinyinStart = pos;
            while(*pos != '#' && *pos != '\0'){
                pos++;

                while(*pos == ',' || std::isblank(*pos)){
                    if(pos != PinyinStart){
                        Pinyin pinyin(PinyinStart, pos-PinyinStart);
                        pinyinList.push_back(pinyin);
                    }

                    pos++;
                    PinyinStart = pos;
                }
            }

            if(pos != PinyinStart){
                Pinyin pinyin(PinyinStart, pos-PinyinStart);
                pinyinList.push_back(pinyin);
            }
            if(not pinyinList.empty()){
                this->data[uniChar] = pinyinList;
            }
        }
    }

    return this->data.size();
}


Pinyins PinyinDict::getPinyins(char32_t unichar) const{
    auto iter = this->data.find(unichar);
    if(iter == this->data.cend()){
        return {};
    }
    return iter->second;
}



void PinyinDict::printDict(){
    for(auto iter = this->data.cbegin(); iter != this->data.cend(); iter++){
        std::cout << iter->first << ": ";
        auto &pinyinList = iter->second;

        for(auto pinyinIter = pinyinList.cbegin(); pinyinIter != pinyinList.cend(); pinyinIter++){
            std::cout << *pinyinIter << ",";
        }

        std::cout << std::endl;

    }
}

