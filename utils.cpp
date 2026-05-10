#include "utils.h"

std::string to_utf8(std::u32string_view input) {
    std::string result;
    for (char32_t ch : input) {
        if (ch < 0x80) {
            result += static_cast<char>(ch);
        } else if (ch < 0x800) {
            result += static_cast<char>(0xC0 | (ch >> 6));
            result += static_cast<char>(0x80 | (ch & 0x3F));
        } else if (ch < 0x10000) {
            result += static_cast<char>(0xE0 | (ch >> 12));
            result += static_cast<char>(0x80 | ((ch >> 6) & 0x3F));
            result += static_cast<char>(0x80 | (ch & 0x3F));
        } else {
            result += static_cast<char>(0xF0 | (ch >> 18));
            result += static_cast<char>(0x80 | ((ch >> 12) & 0x3F));
            result += static_cast<char>(0x80 | ((ch >> 6) & 0x3F));
            result += static_cast<char>(0x80 | (ch & 0x3F));
        }
    }
    return result;
}

std::u32string to_utf32(std::string_view input) {
    std::u32string result;
    const char* ptr = input.data();
    const char* end = ptr + input.size();

    while (ptr < end) {
        unsigned char c = static_cast<unsigned char>(*ptr);
        char32_t ch;

        if (c < 0x80) {
            ch = c;
            ptr += 1;
        } else if ((c & 0xE0) == 0xC0) {
            if (ptr + 2 > end) break;
            ch = (static_cast<char32_t>(ptr[0] & 0x1F) << 6) |
                 (static_cast<char32_t>(ptr[1] & 0x3F));
            ptr += 2;
        } else if ((c & 0xF0) == 0xE0) {
            if (ptr + 3 > end) break;
            ch = (static_cast<char32_t>(ptr[0] & 0x0F) << 12) |
                 (static_cast<char32_t>(ptr[1] & 0x3F) << 6) |
                 (static_cast<char32_t>(ptr[2] & 0x3F));
            ptr += 3;
        } else if ((c & 0xF8) == 0xF0) {
            if (ptr + 4 > end) break;
            ch = (static_cast<char32_t>(ptr[0] & 0x07) << 18) |
                 (static_cast<char32_t>(ptr[1] & 0x3F) << 12) |
                 (static_cast<char32_t>(ptr[2] & 0x3F) << 6) |
                 (static_cast<char32_t>(ptr[3] & 0x3F));
            ptr += 4;
        } else {
            break;  // 非法字节序
        }
        result.push_back(ch);
    }
    return result;
}
