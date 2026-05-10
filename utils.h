#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <string_view>

// UTF-32 转 UTF-8（纯手工编码，不依赖 locale）
std::string to_utf8(std::u32string_view input);

// UTF-8 转 UTF-32（纯手工解码，不依赖 locale）
std::u32string to_utf32(std::string_view input);

#endif // UTILS_H
