# piny

基于拼音的汉字搜索工具，支持拼音模糊匹配、成语接龙等功能。

## 项目结构

| 文件 | 说明 |
|------|------|
| `types.h` | 基础类型定义：`Syllable`、`SyllableList`、`ToneStyle` |
| `dictionary.h/cpp` | `Dictionary` — 拼音词典，从文件加载 Unicode → 拼音映射 |
| `tone.h/cpp` | 拼音声调转换（声调符号 ↔ 数字），自由函数无状态 |
| `utils.h/cpp` | UTF-32/UTF-8 互转，无 locale 依赖 |
| `searcher.h/cpp` | `Searcher` — 基于 Trie 树的拼音搜索引擎，支持模糊匹配 |
| `main.cpp` | 入口 & 成语接龙互动游戏 |
| `pinyin.txt` | 拼音数据文件（来源：[mozillazg/pinyin-data](https://github.com/mozillazg/pinyin-data)） |
| `test/chengyu.txt` | 成语列表，每行一个成语 |

## 编译

```bash
make
```

使用 C++17 标准，无外部依赖。输出可执行文件 `piny`。

## 核心模块

### Dictionary — 拼音词典

从 `pinyin.txt` 加载 Unicode 字符到拼音列表的映射。文件格式：

```
U+6211: wǒ  # 我
U+7231: ài  # 爱
U+4E60: xí  # 习
```

### tone — 声调转换

支持四种声调表示法的相互转换：

| 表示法 | 示例 | 说明 |
|--------|------|------|
| `ToneStyle::Mark` | zhāng | 原始声调符号（来自词典） |
| `ToneStyle::Number` | zha1ng | 声调用数字标记在元音后 |
| `ToneStyle::NumberEnd` | zhang1 | 声调数字统一放在末尾 |
| `ToneStyle::None` | zhang | 去掉声调 |

### Searcher — 拼音搜索引擎

基于 Trie 树的拼音搜索核心：

- **`add()`** — 将文本按拼音展开插入搜索树。每个汉字转换为对应的拼音列表，多音字会展开为多条路径
- **`find()`** — 输入拼音序列，返回匹配的文本列表（精确匹配优先，再展开模糊匹配）
- **`match_pinyin()`** — 判断一段拼音序列能否完整匹配一段文本（静态方法）

### utils — 工具函数

- `to_utf8()` / `to_utf32()` — UTF-32 与 UTF-8 字符串互转，手工编码实现

## 运行

当前目录下执行：

```bash
./piny
```

默认启动**成语接龙**互动模式：输入汉字或成语，程序输出该字的拼音并列出以此拼音开头的成语。

## 数据结构

拼音搜索使用 **Trie 树（前缀树）**：

- 每个节点有 26 个分支（对应 a–z 拼音字母）
- 每个节点有 `ref` 计数和指向原文本的指针
- 拼音按字母逐层插入，多音字产生多条搜索路径
- 搜索时先精确匹配，再展开模糊（同层其他字母）
