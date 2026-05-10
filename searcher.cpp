#include "searcher.h"
#include "dictionary.h"
#include "tone.h"
#include <set>

Searcher::Searcher(const Dictionary& dict)
    : dict_(dict)
    , root_(create_node())
{
}

Searcher::~Searcher() {
    // 用显式栈释放所有节点，避免递归过深
    if (root_) {
        std::vector<Node*> stack = {root_};
        while (!stack.empty()) {
            Node* node = stack.back();
            stack.pop_back();
            for (auto* child : node->children) {
                if (child) {
                    stack.push_back(child);
                }
            }
            delete node;
        }
    }
}

Searcher::Node* Searcher::create_node() {
    return new Node{};  // 值初始化将所有成员置零
}

void Searcher::add(std::u32string_view text, const char32_t* id) {
    if (text.empty()) return;
    insert(root_, text, 0, id);
}

void Searcher::insert(Node* node, std::u32string_view text, size_t index,
                      const char32_t* id) {
    if (index >= text.size()) {
        // 文本结束，存储引用
        node->text = id;
        return;
    }

    char32_t ch = text[index];
    SyllableList syllables = dict_.lookup(ch);

    if (syllables.empty()) {
        // 该字符没有拼音（如标点），跳过
        insert(node, text, index + 1, id);
        return;
    }

    for (const auto& syllable : syllables) {
        Syllable stripped = tone::strip(syllable);
        if (stripped.empty()) continue;

        // 沿音节的字母逐个走 Trie
        Node* current = node;
        for (char letter : stripped) {
            int idx = letter - 'a';
            if (idx < 0 || idx >= 26) continue;
            if (!current->children[idx]) {
                current->children[idx] = create_node();
            }
            current = current->children[idx];
        }

        // 继续处理下一个汉字
        insert(current, text, index + 1, id);
    }
}

std::vector<const char32_t*> Searcher::find(std::string_view pinyin,
                                            int max_results) const {
    if (pinyin.empty() || max_results <= 0) {
        return {};
    }

    // 第一步：在 Trie 中查找匹配的节点
    std::vector<Node*> match_nodes;
    int remaining = max_results;
    find_nodes(root_, pinyin.data(), match_nodes, remaining);

    // 第二步：从匹配节点展开，收集所有文本指针
    std::vector<const char32_t*> all_texts;
    remaining = max_results;
    for (Node* node : match_nodes) {
        collect_texts(node, all_texts, remaining);
        if (remaining <= 0) break;
    }

    // 第三步：去重（保留首次出现的顺序）
    std::set<const char32_t*> seen;
    std::vector<const char32_t*> result;
    for (const char32_t* text : all_texts) {
        if (text && seen.find(text) == seen.end()) {
            seen.insert(text);
            result.push_back(text);
            if (static_cast<int>(result.size()) >= max_results) {
                break;
            }
        }
    }

    return result;
}

void Searcher::find_nodes(Node* node, const char* query,
                          std::vector<Node*>& matches,
                          int& remaining) const {
    if (!node || remaining <= 0) return;

    if (*query == '\0') {
        matches.push_back(node);
        return;
    }

    int exact_idx = *query - 'a';
    bool valid_idx = (exact_idx >= 0 && exact_idx < 26);

    // 优先级 1：精确匹配——吃掉一个查询字符
    if (valid_idx && node->children[exact_idx]) {
        find_nodes(node->children[exact_idx], query + 1, matches, remaining);
    }

    // 优先级 2：模糊匹配——尝试其他字母，不消耗查询字符
    for (int i = 0; i < 26 && remaining > 0; i++) {
        if (i == exact_idx) continue;  // 已精确匹配过
        if (node->children[i]) {
            find_nodes(node->children[i], query, matches, remaining);
        }
    }
}

void Searcher::collect_texts(Node* node,
                             std::vector<const char32_t*>& results,
                             int& remaining) const {
    if (!node || remaining <= 0) return;

    if (node->text) {
        results.push_back(node->text);
        remaining--;
    }

    for (auto* child : node->children) {
        collect_texts(child, results, remaining);
        if (remaining <= 0) return;
    }
}
