// piny 单元测试 — 无外部依赖，仅 C++17 标准库
// 编译：g++ -std=c++17 -Wall -Wextra -I. -c test/test.cpp
// 链接：g++ -o test/test_runner test/test.o dictionary.o tone.o utils.o searcher.o

#include "dictionary.h"
#include "tone.h"
#include "utils.h"
#include "searcher.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <clocale>

// ======================== 简易测试框架 ========================

struct TestCase {
    const char* name;
    void (*fn)();
    TestCase* next;
    static TestCase* list;

    TestCase(const char* n, void (*f)()) : name(n), fn(f), next(list) { list = this; }

    static int run_all() {
        int passed = 0, failed = 0;
        for (auto t = list; t; t = t->next) {
            try {
                t->fn();
                std::cout << "  \033[32m✓\033[0m " << t->name << std::endl;
                passed++;
            } catch (const std::exception& e) {
                std::cout << "  \033[31m✗\033[0m " << t->name << ": " << e.what() << std::endl;
                failed++;
            } catch (...) {
                std::cout << "  \033[31m✗\033[0m " << t->name << ": 未知异常" << std::endl;
                failed++;
            }
        }
        std::cout << "\n结果: " << (passed + failed) << " 个测试, "
                  << passed << " 通过, " << failed << " 失败" << std::endl;
        return failed;
    }
};

TestCase* TestCase::list = nullptr;

#define ASSERT(cond) do {                                       \
    if (!(cond)) {                                              \
        throw std::runtime_error(                               \
            std::string(__FILE__) + ":" + std::to_string(__LINE__) + \
            " [" #cond "]");                                    \
    }                                                           \
} while(0)

#define TEST(name)                                              \
    void test_##name();                                         \
    TestCase reg_##name(#name, test_##name);                    \
    void test_##name()

// ======================== UTF 编解码测试 ========================

TEST(Utf8RoundTrip_ASCII) {
    std::string s = "hello, world! 123";
    ASSERT(to_utf8(to_utf32(s)) == s);
}

TEST(Utf8RoundTrip_Chinese) {
    std::string s = "你好世界，测试！";
    ASSERT(to_utf8(to_utf32(s)) == s);
}

TEST(Utf8RoundTrip_Mixed) {
    std::string s = "Hello 你好 123 ！";
    ASSERT(to_utf8(to_utf32(s)) == s);
}

TEST(Utf8RoundTrip_Empty) {
    std::string s;
    ASSERT(to_utf8(to_utf32(s)).empty());
}

TEST(Utf8RoundTrip_EdgeChars) {
    // 2 字节、3 字节、4 字节字符混合
    std::string s = "a©中\U0001F600";  // a + © + 中 + 😀
    ASSERT(to_utf8(to_utf32(s)) == s);
}

TEST(Utf8ToUtf32_Count) {
    std::u32string u32 = to_utf32("你好");
    ASSERT(u32.size() == 2);
    // 你 = U+4F60, 好 = U+597D
    ASSERT(u32[0] == 0x4F60);
    ASSERT(u32[1] == 0x597D);
}

TEST(Utf8ToUtf32_PartialInput) {
    // 单个字符的 UTF-8 编码只给一部分，应优雅截断
    std::u32string u32 = to_utf32("\xE4\xBD");  // "你" 的不完整编码
    ASSERT(u32.empty());  // 应当返回空或已解码部分
}

// ======================== 声调转换测试 ========================

TEST(Tone_Strip_Basic) {
    ASSERT(tone::strip("hǎo") == "hao");
    ASSERT(tone::strip("zhōng") == "zhong");
    ASSERT(tone::strip("nǐ") == "ni");
    ASSERT(tone::strip("shì") == "shi");
    ASSERT(tone::strip("jiè") == "jie");
}

TEST(Tone_Strip_NoTone) {
    ASSERT(tone::strip("zhong") == "zhong");
    ASSERT(tone::strip("ni") == "ni");
    ASSERT(tone::strip("") == "");
}

TEST(Tone_Strip_UDiaeresis) {
    // ü → v, ǜ → v, ǖ → v
    ASSERT(tone::strip("lǜ") == "lv");
    ASSERT(tone::strip("nǚ") == "nv");
    ASSERT(tone::strip("lǖ") == "lv");
}

TEST(Tone_InlineNumber_Basic) {
    ASSERT(tone::inline_number("hǎo") == "ha3o");
    ASSERT(tone::inline_number("zhōng") == "zho1ng");
    ASSERT(tone::inline_number("nǐ") == "ni3");
    ASSERT(tone::inline_number("shì") == "shi4");
    ASSERT(tone::inline_number("jiè") == "jie4");
}

TEST(Tone_InlineNumber_NoTone) {
    ASSERT(tone::inline_number("zhong") == "zhong");
    ASSERT(tone::inline_number("") == "");
}

TEST(Tone_EndNumber_Basic) {
    ASSERT(tone::end_number("hǎo") == "hao3");
    ASSERT(tone::end_number("zhōng") == "zhong1");
    ASSERT(tone::end_number("nǐ") == "ni3");
    ASSERT(tone::end_number("jiè") == "jie4");
}

TEST(Tone_EndNumber_MultiVowel) {
    ASSERT(tone::end_number("huī") == "hui1");
    ASSERT(tone::end_number("kuài") == "kuai4");
}

TEST(Tone_Convert_AllStyles) {
    ASSERT(tone::convert("zhōng", ToneStyle::None) == "zhong");
    ASSERT(tone::convert("zhōng", ToneStyle::Mark) == "zhōng");
    ASSERT(tone::convert("zhōng", ToneStyle::Number) == "zho1ng");
    ASSERT(tone::convert("zhōng", ToneStyle::NumberEnd) == "zhong1");
}

TEST(Tone_Convert_Empty) {
    ASSERT(tone::convert("", ToneStyle::None).empty());
    ASSERT(tone::convert("", ToneStyle::Mark).empty());
}

// ======================== 词典测试 ========================

// 加载一次，所有词典测试共享
static const Dictionary& shared_dict() {
    static Dictionary dict = []() {
        Dictionary d;
        if (!d.load("pinyin.txt")) {
            std::cerr << "错误：无法加载 pinyin.txt，请从项目根目录运行测试" << std::endl;
            std::exit(1);
        }
        std::cout << "词典已加载 " << d.size() << " 条" << std::endl;
        return d;
    }();
    return dict;
}

TEST(Dict_LoadSize) {
    const auto& dict = shared_dict();
    ASSERT(dict.size() > 10000);  // 应该有上万条
}

TEST(Dict_Lookup_Known) {
    const auto& dict = shared_dict();
    SyllableList s = dict.lookup(0x4E2D);  // 中
    ASSERT(!s.empty());
    // 应该包含 zhōng 或 zhòng
    bool found_one = false;
    for (const auto& syl : s) {
        if (tone::strip(syl) == "zhong") found_one = true;
    }
    ASSERT(found_one);
}

TEST(Dict_Lookup_Unknown) {
    const auto& dict = shared_dict();
    SyllableList s = dict.lookup(0x0000);  // 空字符，不存在
    ASSERT(s.empty());
}

TEST(Dict_Lookup_NiHao) {
    const auto& dict = shared_dict();
    auto ni = dict.lookup(0x4F60);  // 你
    auto hao = dict.lookup(0x597D); // 好
    ASSERT(!ni.empty());
    ASSERT(!hao.empty());
    ASSERT(tone::strip(ni[0]) == "ni");
    ASSERT(tone::strip(hao[0]) == "hao");
}

TEST(Dict_Lookup_ShiJie) {
    const auto& dict = shared_dict();
    auto shi = dict.lookup(0x4E16);  // 世
    auto jie = dict.lookup(0x754C);  // 界
    ASSERT(!shi.empty());
    ASSERT(!jie.empty());
    ASSERT(tone::strip(shi[0]) == "shi");
    ASSERT(tone::strip(jie[0]) == "jie");
}

TEST(Dict_Lookup_Polyphonic) {
    const auto& dict = shared_dict();
    auto s = dict.lookup(0x4E2D);  // 中 是多音字
    ASSERT(s.size() >= 2);  // zhōng, zhòng
}

// ======================== 搜索器测试 ========================

TEST(Searcher_EmptyFind) {
    const auto& dict = shared_dict();
    Searcher s(dict);
    auto results = s.find("", 5);
    ASSERT(results.empty());
}

TEST(Searcher_AddAndFindFull) {
    const auto& dict = shared_dict();
    Searcher s(dict);

    // 使用 "你好世界"
    std::u32string text = U"你好世界";
    s.add(text, text.data());

    // 完整拼音查找
    auto results = s.find("nihao", 5);
    ASSERT(!results.empty());
}

TEST(Searcher_AddAndFindExact) {
    const auto& dict = shared_dict();
    Searcher s(dict);

    std::u32string text = U"你好世界";
    s.add(text, text.data());

    // 全拼查找
    auto results = s.find("nihaoshijie", 5);
    ASSERT(!results.empty());
    // 验证找到的文本内容
    ASSERT(std::u32string(results[0]) == text);
}

TEST(Searcher_FuzzyMatch) {
    const auto& dict = shared_dict();
    Searcher s(dict);

    std::u32string text = U"你好世界";
    s.add(text, text.data());

    // 模糊查找：n 打头，后面的字母模糊匹配
    auto results = s.find("nhao", 5);
    ASSERT(!results.empty());
}

TEST(Searcher_MultipleEntries) {
    const auto& dict = shared_dict();
    Searcher s(dict);

    std::vector<std::u32string> texts = {
        U"中国",
        U"你好世界",
        U"好好学习",
    };
    for (const auto& t : texts) {
        s.add(t, t.data());
    }

    // 查找 "zhongguo" — 应该匹配"中国"
    auto results = s.find("zhongguo", 5);
    ASSERT(!results.empty());
    ASSERT(std::u32string(results[0]) == U"中国");
}

TEST(Searcher_MaxResults) {
    const auto& dict = shared_dict();
    Searcher s(dict);

    // 添加一些以"中"开头的成语
    std::vector<std::u32string> texts = {
        U"中流砥柱", U"中饱私囊", U"中道而废",
        U"中规中矩", U"中西合璧",
    };
    for (const auto& t : texts) {
        s.add(t, t.data());
    }

    auto results = s.find("zhong", 3);
    ASSERT(results.size() <= 3);
}

TEST(Searcher_Dedup) {
    const auto& dict = shared_dict();
    Searcher s(dict);

    std::u32string text = U"中国";
    // 重复添加同一个文本
    s.add(text, text.data());
    s.add(text, text.data());

    auto results = s.find("zhongguo", 5);
    // 去重后只有一条
    ASSERT(results.size() == 1);
}

TEST(Searcher_NoMatch) {
    const auto& dict = shared_dict();
    Searcher s(dict);

    std::u32string text = U"你好世界";
    s.add(text, text.data());

    auto results = s.find("xxxxx", 5);
    ASSERT(results.empty());
}

TEST(Searcher_Polyphonic) {
    const auto& dict = shared_dict();
    Searcher s(dict);

    // 测试多音字："中"有 zhōng 和 zhòng 两个读音
    // 不管哪个读音，Trie 都应该能匹配
    std::u32string text = U"中国";
    s.add(text, text.data());

    auto results = s.find("zhong", 5);
    ASSERT(!results.empty());
}

// ======================== 入口 ========================

int main() {
    std::setlocale(LC_ALL, "en_US.utf8");

    std::cout << "\n╔══════════════════════════════╗" << std::endl;
    std::cout <<   "║       piny 单元测试           ║" << std::endl;
    std::cout <<   "╚══════════════════════════════╝" << std::endl;
    std::cout << std::endl;

    int failed = TestCase::run_all();
    return failed > 0 ? 1 : 0;
}
