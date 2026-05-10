#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>

// 单个音节，如 "zhang"、"guo"
using Syllable = std::string;

// 某个汉字对应的多个读音
using SyllableList = std::vector<Syllable>;

// 声调表示方式
enum class ToneStyle {
    None,       // zhang — 无声调
    Mark,       // zhāng — 带声调符号（来自词典原始数据）
    Number,     // zha1ng — 数字插在元音后
    NumberEnd,  // zhang1 — 数字放在末尾
};

#endif // TYPES_H
