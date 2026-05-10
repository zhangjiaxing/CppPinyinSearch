#ifndef SEARCHER_H
#define SEARCHER_H

#include "types.h"
#include <string_view>
#include <vector>

class Dictionary;

class Searcher {
public:
    explicit Searcher(const Dictionary& dict);
    ~Searcher();

    // 不允许拷贝
    Searcher(const Searcher&) = delete;
    Searcher& operator=(const Searcher&) = delete;

    // 将文本加入搜索索引
    // id 是不透明的关联指针，按原样存储，调用方需确保其生存期
    void add(std::u32string_view text, const char32_t* id);

    // 查找拼音匹配的文本，空字符串返回空结果
    std::vector<const char32_t*> find(std::string_view pinyin,
                                      int max_results = 10) const;

    // 检查拼音序列是否能完整匹配一段文本
    // 返回未匹配的字符数（0 表示完全匹配）
    static int match_pinyin(const Dictionary& dict,
                            std::string_view pinyin,
                            std::string_view text_utf8);

private:
    struct Node {
        Node* children[26] = {};  // a–z，按 letter - 'a' 索引
        int ref_count = 0;        // 经过此节点的文本数
        const char32_t* text = nullptr;  // 非空表示有文本在此结束
    };

    // 递归将文本拼音插入 Trie
    // index 是当前正在处理的字符位置
    void insert(Node* node, std::u32string_view text, size_t index,
                const char32_t* id);

    // 查找匹配查询的 Trie 节点（含模糊展开）
    void find_nodes(Node* node, const char* query,
                    std::vector<Node*>& matches, int& remaining) const;

    // 收集节点下的所有文本指针
    void collect_texts(Node* node,
                       std::vector<const char32_t*>& results,
                       int& remaining) const;

    Node* create_node();

    const Dictionary& dict_;
    Node* root_;
};

#endif // SEARCHER_H
