#ifndef TONE_H
#define TONE_H

#include "types.h"
#include <string_view>

// 声调转换工具（无状态，自由函数）
namespace tone {

// 去掉声调： "zhāng" -> "zhang"
Syllable strip(std::string_view pinyin);

// 数字插在元音后： "zhāng" -> "zha1ng"
Syllable inline_number(std::string_view pinyin);

// 数字放在末尾： "zhāng" -> "zhang1"
Syllable end_number(std::string_view pinyin);

// 按指定样式转换
Syllable convert(std::string_view pinyin, ToneStyle style);

} // namespace tone

#endif // TONE_H
