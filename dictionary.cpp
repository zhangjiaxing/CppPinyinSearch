#include "dictionary.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

bool Dictionary::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "错误：无法打开 " << filename << std::endl;
        return false;
    }

    std::string line;
    int line_num = 0;
    while (std::getline(file, line)) {
        line_num++;
        std::string_view sv = trim(line);

        // 跳过空行和注释行
        if (sv.empty() || sv.front() == '#') {
            continue;
        }

        // 期待格式: "U+XXXX: pinyin1,pinyin2  # 注释"
        if (sv.size() < 2 || sv[0] != 'U' || sv[1] != '+') {
            continue;
        }

        // 解析 Unicode 码点
        size_t colon_pos = sv.find(':');
        if (colon_pos == std::string_view::npos) {
            continue;
        }

        std::string_view hex_part = sv.substr(2, colon_pos - 2);
        char* end = nullptr;
        char32_t code_point = static_cast<char32_t>(
            std::strtoul(hex_part.data(), &end, 16));
        if (end == hex_part.data()) {
            continue;  // 没解析出数字
        }

        // 解析冒号后的拼音列表
        std::string_view pinyin_part = sv.substr(colon_pos + 1);

        SyllableList syllables;
        while (!pinyin_part.empty()) {
        // 跳过前导空白和逗号
            size_t start = 0;
            while (start < pinyin_part.size() &&
                   (pinyin_part[start] == ' ' || pinyin_part[start] == ',')) {
                start++;
            }
            if (start >= pinyin_part.size()) break;
            pinyin_part = pinyin_part.substr(start);

            // 遇到井号表示后面是注释
            if (pinyin_part[0] == '#') break;

            // 找拼音结束位置（空白、逗号或注释）
            size_t end_pos = pinyin_part.find_first_of(" ,#");
            if (end_pos == 0) {
                // 前导分隔符，跳过
                pinyin_part = pinyin_part.substr(1);
                continue;
            }
            if (end_pos == std::string_view::npos) {
                syllables.emplace_back(pinyin_part);
                break;
            }
            syllables.emplace_back(pinyin_part.substr(0, end_pos));
            pinyin_part = pinyin_part.substr(end_pos);
        }

        if (!syllables.empty()) {
            data_[code_point] = std::move(syllables);
        }
    }

    std::cout << "已加载 " << data_.size() << " 条词典条目" << std::endl;
    return true;
}

SyllableList Dictionary::lookup(char32_t ch) const {
    auto it = data_.find(ch);
    if (it == data_.end()) {
        return {};
    }
    return it->second;
}

std::string_view Dictionary::trim(std::string_view s) {
    while (!s.empty() && (s.front() == ' ' || s.front() == '\t')) {
        s.remove_prefix(1);
    }
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t' || s.back() == '\r')) {
        s.remove_suffix(1);
    }
    return s;
}
