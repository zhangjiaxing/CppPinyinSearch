#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "types.h"
#include <string>
#include <map>

class Dictionary {
public:
    Dictionary() = default;

    [[nodiscard]] bool load(const std::string& filename);

    [[nodiscard]] SyllableList lookup(char32_t ch) const;

    [[nodiscard]] size_t size() const { return data_.size(); }

private:
    std::map<char32_t, SyllableList> data_;
};

#endif // DICTIONARY_H
