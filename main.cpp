#include <iostream>
#include <fstream>
#include <locale>
#include <cstring>
#include "dictionary.h"
#include "tone.h"
#include "utils.h"
#include "searcher.h"

// ============================== 演示功能 ==============================

// 演示 1：四种声调样式输出
static void demo_print_pinyins(const Dictionary& dict) {
    const char* text = "欲穷千里目，\n更上一层楼。";

    std::cout << "\n◆ 拼音输出演示" << std::endl;
    std::cout << "  text: " << text << std::endl;
    std::u32string u32text = to_utf32(text);

    for (ToneStyle style : {ToneStyle::None, ToneStyle::Mark,
                            ToneStyle::Number, ToneStyle::NumberEnd}) {
        const char* name = "";
        switch (style) {
            case ToneStyle::None:      name = "无声调"; break;
            case ToneStyle::Mark:      name = "声调符号"; break;
            case ToneStyle::Number:    name = "数字插入"; break;
            case ToneStyle::NumberEnd: name = "数字尾置"; break;
        }
        std::cout << "  [" << name << "] ";
        for (char32_t ch : u32text) {
            std::string utf8_char = to_utf8(std::u32string_view(&ch, 1));
            std::cout << utf8_char;

            SyllableList syllables = dict.lookup(ch);
            for (const auto& s : syllables) {
                std::cout << " " << tone::convert(s, style);
            }
        }
        std::cout << std::endl;
    }
}

// 演示 2：拼音匹配
static void demo_match_list(const Dictionary& dict) {
    std::cout << "\n◆ 拼音匹配演示" << std::endl;

    int ret = Searcher::match_pinyin(dict, "yqongqlmlouabc",
                                     "欲穷千里目，\n更上一层楼。");
    std::cout << "  拼音 \"yqongqlmlouabc\" 匹配诗句 => "
              << (ret == 0 ? "✓ 完全匹配" : "✗ 未完全匹配") << std::endl;

    struct NameEntry { const char* name; const char* pinyin; };
    NameEntry names[] = {
        {"琅琊", "lang2 ya2"},
        {"利用", "li4 yong4"},
        {"阿姨", "a1 yi2"},
        {"你好哇", "ni3 hao3 wa1"},
    };

    std::cout << "  拼音 \"lay\" 匹配名称：" << std::endl;
    int match_count = 0;
    for (const auto& entry : names) {
        int r = Searcher::match_pinyin(dict, "lay", entry.name);
        if (r == 0) {
            std::cout << "    ✓ " << entry.name
                      << " (" << entry.pinyin << ")" << std::endl;
            match_count++;
        }
    }
    if (match_count == 0) {
        std::cout << "    （无匹配）" << std::endl;
    } else {
        std::cout << "  共匹配 " << match_count << " 个" << std::endl;
    }
}

// 演示 3：Trie 搜索
static void demo_trie_search(const Dictionary& dict) {
    std::cout << "\n◆ Trie 搜索演示" << std::endl;

    std::vector<std::u32string> idiom_list = {
        U"爱屋及乌", U"爱惜羽毛", U"爱憎分明",
        U"碍手碍脚", U"安邦定国", U"安不忘危",
        U"安居乐业", U"按兵不动", U"暗度陈仓",
    };

    Searcher searcher(dict);
    for (const auto& idiom : idiom_list) {
        searcher.add(idiom, idiom.data());
    }

    auto results = searcher.find("alaohs", 5);
    std::cout << "  搜索 \"alaohs\" 找到 " << results.size() << " 条：" << std::endl;
    for (const char32_t* text : results) {
        std::cout << "    " << to_utf8(text) << std::endl;
    }
    if (results.empty()) {
        std::cout << "    （无结果）" << std::endl;
    }
}

// 演示 4：从成语文件搜索
static void demo_file_search(const Dictionary& dict) {
    std::cout << "\n◆ 文件搜索演示" << std::endl;

    std::ifstream file("test/chengyu.txt");
    if (!file.is_open()) {
        std::cerr << "  错误：无法打开 test/chengyu.txt" << std::endl;
        return;
    }

    std::vector<std::u32string> idiom_list;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            idiom_list.push_back(to_utf32(line));
        }
    }
    file.close();

    Searcher searcher(dict);
    for (const auto& idiom : idiom_list) {
        searcher.add(idiom, idiom.data());
    }

    const char* queries[] = {"zhutou", "yujie", "yiban", nullptr};
    for (int i = 0; queries[i]; i++) {
        auto results = searcher.find(queries[i], 5);
        std::cout << "  搜索 \"" << queries[i] << "\" 找到 "
                  << results.size() << " 条：" << std::endl;
        for (const char32_t* text : results) {
            std::cout << "    " << to_utf8(text) << std::endl;
        }
        if (results.empty()) {
            std::cout << "    （无结果）" << std::endl;
        }
    }
}

// 运行全部演示
static void run_all_demos(const Dictionary& dict) {
    std::cout << "\n═══════════════════════════════════════" << std::endl;
    std::cout << "  功能演示" << std::endl;
    std::cout << "═══════════════════════════════════════" << std::endl;
    demo_print_pinyins(dict);
    demo_match_list(dict);
    demo_trie_search(dict);
    demo_file_search(dict);
    std::cout << "\n演示结束，按 Enter 返回菜单..." << std::endl;
    std::cin.get();
    std::cin.get();
}

// ============================== 成语接龙 ==============================

static void game_idiom_chain(const Dictionary& dict) {
    std::ifstream file("test/chengyu.txt");
    if (!file.is_open()) {
        std::cerr << "错误：无法打开 test/chengyu.txt" << std::endl;
        return;
    }

    std::vector<std::u32string> idiom_list;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            idiom_list.push_back(to_utf32(line));
        }
    }
    file.close();

    Searcher searcher(dict);
    for (const auto& idiom : idiom_list) {
        searcher.add(idiom, idiom.data());
    }
    std::cout << "已加载 " << idiom_list.size() << " 条成语" << std::endl;

    std::cout << "\n◆ 成语接龙" << std::endl;
    std::cout << "  输入一个汉字或成语，显示以最后一个字拼音开头的成语。" << std::endl;
    std::cout << "  输入 exit 或 0 返回菜单。" << std::endl;

    std::string input;
    while (true) {
        std::cout << "\n请输入: " << std::flush;
        if (!(std::cin >> input)) {
            std::cin.clear();
            continue;
        }
        if (input == "exit" || input == "0") break;

        std::u32string u32input = to_utf32(input);
        if (u32input.empty()) continue;
        std::cout << std::endl;

        char32_t last_char = u32input.back();
        SyllableList syllables = dict.lookup(last_char);

        if (syllables.size() > 1) {
            std::cout << to_utf8(std::u32string_view(&last_char, 1))
                      << " 是多音字！" << std::endl;
        }

        bool found = false;
        for (const auto& s : syllables) {
            Syllable stripped = tone::strip(s);
            if (stripped.empty()) continue;

            auto results = searcher.find(stripped, 10);
            if (!results.empty()) {
                found = true;
                std::cout << stripped << " 开头的成语：" << std::endl;
                for (const char32_t* text : results) {
                    std::cout << "  " << to_utf8(text) << std::endl;
                }
            }
        }
        if (!found) {
            std::cout << "  （未找到匹配的成语）" << std::endl;
        }
    }
}

// ============================== 命令行搜索 ==============================

static void cmd_search(const Dictionary& dict) {
    std::ifstream file("test/chengyu.txt");
    if (!file.is_open()) {
        std::cerr << "错误：无法打开 test/chengyu.txt" << std::endl;
        return;
    }

    std::vector<std::u32string> idiom_list;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            idiom_list.push_back(to_utf32(line));
        }
    }
    file.close();

    Searcher searcher(dict);
    for (const auto& idiom : idiom_list) {
        searcher.add(idiom, idiom.data());
    }
    std::cout << "已加载 " << idiom_list.size() << " 条成语" << std::endl;

    std::cout << "\n◆ 拼音搜索" << std::endl;
    std::cout << "  输入拼音查找成语，支持模糊匹配。输入 exit 或 0 返回菜单。" << std::endl;

    std::string input;
    while (true) {
        std::cout << "\n拼音: " << std::flush;
        if (!(std::cin >> input)) {
            std::cin.clear();
            continue;
        }
        if (input == "exit" || input == "0") break;

        // 只保留 a-z 字母
        Syllable clean;
        for (char ch : input) {
            if (ch >= 'A' && ch <= 'Z') clean += ch - 'A' + 'a';
            else if (ch >= 'a' && ch <= 'z') clean += ch;
        }
        if (clean.empty()) continue;

        auto results = searcher.find(clean, 20);
        std::cout << "结果：" << results.size() << " 条" << std::endl;
        for (const char32_t* text : results) {
            // 同时显示拼音
            std::u32string u32text(text);
            std::cout << "  " << to_utf8(text) << "  ";
            for (char32_t ch : u32text) {
                SyllableList syllables = dict.lookup(ch);
                for (size_t i = 0; i < syllables.size(); i++) {
                    if (i > 0) std::cout << "/";
                    std::cout << tone::strip(syllables[i]);
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }
        if (results.empty()) {
            std::cout << "  （无结果）" << std::endl;
        }
    }
}

// ============================== 主菜单 ==============================

static void show_menu() {
    std::cout << "\n"
              << "  ╔══════════════════════════╗\n"
              << "  ║     piny 拼音工具箱        ║\n"
              << "  ╚══════════════════════════╝\n"
              << std::endl;
    std::cout << "  1. 成语接龙（游戏）" << std::endl;
    std::cout << "  2. 功能演示" << std::endl;
    std::cout << "  3. 拼音搜索" << std::endl;
    std::cout << "  0. 退出" << std::endl;
    std::cout << std::endl;
    std::cout << "请选择: " << std::flush;
}

static void run_menu(const Dictionary& dict) {
    while (true) {
        show_menu();
        std::string choice;
        if (!(std::cin >> choice)) break;

        if (choice == "1") {
            game_idiom_chain(dict);
        } else if (choice == "2") {
            run_all_demos(dict);
        } else if (choice == "3") {
            cmd_search(dict);
        } else if (choice == "0" || choice == "exit") {
            break;
        } else {
            std::cout << "无效选项，请重新选择。" << std::endl;
        }
    }
}

// ============================== 入口 ==============================

static void print_usage(const char* prog) {
    std::cout << "用法：" << std::endl;
    std::cout << "  " << prog << "             互动菜单" << std::endl;
    std::cout << "  " << prog << " game        成语接龙" << std::endl;
    std::cout << "  " << prog << " demo        功能演示" << std::endl;
    std::cout << "  " << prog << " search      拼音搜索" << std::endl;
}

int main(int argc, char* argv[]) {
    std::setlocale(LC_ALL, "en_US.utf8");

    Dictionary dict;
    if (!dict.load("pinyin.txt")) {
        return 1;
    }

    if (argc > 1) {
        if (std::strcmp(argv[1], "game") == 0) {
            game_idiom_chain(dict);
        } else if (std::strcmp(argv[1], "demo") == 0) {
            run_all_demos(dict);
        } else if (std::strcmp(argv[1], "search") == 0) {
            cmd_search(dict);
        } else if (std::strcmp(argv[1], "-h") == 0 ||
                   std::strcmp(argv[1], "--help") == 0) {
            print_usage(argv[0]);
        } else {
            std::cout << "未知选项：" << argv[1] << std::endl;
            print_usage(argv[0]);
        }
    } else {
        run_menu(dict);
    }

    std::cout << "再见！" << std::endl;
    return 0;
}
