#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "types.h"
#include <string>
#include <map>

class Dictionary {
public:
    Dictionary() = default;

    // 从文件加载拼音数据，成功返回 true
    bool load(const std::string& filename);

    // 查询某个 Unicode 字符的拼音列表
    // 若未收录则返回空列表
    SyllableList lookup(char32_t ch) const;

    // 返回已加载的条目数
    size_t size() const { return data_.size(); }

private:
    // 去除字符串首尾空白
    static std::string_view trim(std::string_view s);

    std::map<char32_t, SyllableList> data_;
};

#endif // DICTIONARY_H
