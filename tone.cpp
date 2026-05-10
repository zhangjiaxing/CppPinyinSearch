#include "tone.h"
#include <map>

namespace {

// 声调映射表：带调字母 → (基础字母, 声调数字)
struct ToneEntry {
    const char* plain;
    int number;  // 1–4，无调则为 0（如 "ü"）
};

using ToneTable = std::map<std::string_view, ToneEntry>;

const ToneTable& table() {
    static const ToneTable t = {
        {"ā", {"a", 1}}, {"á", {"a", 2}}, {"ǎ", {"a", 3}}, {"à", {"a", 4}},
        {"ē", {"e", 1}}, {"é", {"e", 2}}, {"ě", {"e", 3}}, {"è", {"e", 4}},
        {"ō", {"o", 1}}, {"ó", {"o", 2}}, {"ǒ", {"o", 3}}, {"ò", {"o", 4}},
        {"ī", {"i", 1}}, {"í", {"i", 2}}, {"ǐ", {"i", 3}}, {"ì", {"i", 4}},
        {"ū", {"u", 1}}, {"ú", {"u", 2}}, {"ǔ", {"u", 3}}, {"ù", {"u", 4}},
        {"ǖ", {"v", 1}}, {"ǘ", {"v", 2}}, {"ǚ", {"v", 3}}, {"ǜ", {"v", 4}},
        {"ü", {"v", 0}},
        {"ń", {"n", 2}}, {"ň", {"n", 3}}, {"ǹ", {"n", 4}},
        {"m̄", {"m", 1}}, {"ḿ", {"m", 2}}, {"m̀", {"m", 4}},
        {"ê̄", {"ê", 1}}, {"ế", {"ê", 2}}, {"ê̌", {"ê", 3}}, {"ề", {"ê", 4}},
    };
    return t;
}

// 尝试匹配 pos 处的带调字母，返回匹配字节数（2 或 3），无匹配返回 0
int match_len(std::string_view s, size_t pos) {
    for (int len = 3; len >= 2; --len) {
        if (pos + len <= s.size()) {
            auto key = s.substr(pos, len);
            if (table().count(key)) {
                return len;
            }
        }
    }
    return 0;
}

} // 匿名命名空间

Syllable tone::strip(std::string_view pinyin) {
    Syllable result;
    size_t i = 0;
    while (i < pinyin.size()) {
        int len = match_len(pinyin, i);
        if (len > 0) {
            auto key = pinyin.substr(i, len);
            result += table().find(key)->second.plain;
            i += len;
        } else {
            result += pinyin[i];
            i++;
        }
    }
    return result;
}

Syllable tone::inline_number(std::string_view pinyin) {
    Syllable result;
    size_t i = 0;
    while (i < pinyin.size()) {
        int len = match_len(pinyin, i);
        if (len > 0) {
            auto key = pinyin.substr(i, len);
            const auto& entry = table().find(key)->second;
            result += entry.plain;
            if (entry.number > 0) {
                result += std::to_string(entry.number);
            }
            i += len;
        } else {
            result += pinyin[i];
            i++;
        }
    }
    return result;
}

Syllable tone::end_number(std::string_view pinyin) {
    Syllable numbered = inline_number(pinyin);

    // 把所有数字挪到末尾
    Syllable letters;
    Syllable digits;
    for (char ch : numbered) {
        if (ch >= '0' && ch <= '9') {
            digits += ch;
        } else {
            letters += ch;
        }
    }
    return letters + digits;
}

Syllable tone::convert(std::string_view pinyin, ToneStyle style) {
    switch (style) {
        case ToneStyle::None:      return strip(pinyin);
        case ToneStyle::Mark:      return Syllable(pinyin);
        case ToneStyle::Number:    return inline_number(pinyin);
        case ToneStyle::NumberEnd: return end_number(pinyin);
    }
    return Syllable(pinyin);
}
