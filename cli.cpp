#include <cassert>
#include <cctype>
#include <cstring>
#include <exception>
#include <ios>
#include <iostream>
#include <istream>
#include <limits>
#include <ostream>
#include <stdexcept>


void hi(std::ostream &os, std::istream &is) {
    size_t i = 0;
    is >> i;
    if (is.fail()) {
        throw std::exception();
    }
    os << "<HI!>\n";
}

void hello(std::ostream &os, std::istream &is) {
    os << "<HELLO!>\n";
}

size_t match(const char *word, const char *const *words, size_t k) {
    for (size_t i = 0; i < k; ++i) {
        if (std::strlen(word) == std::strlen(words[i]) && !std::strcmp(word, words[i])) {
            return i;
        }
    }
    return k;
}

std::istream &getword(std::istream &is, char *word, size_t k, bool (*c)(char), size_t &size) {
    assert(k > 0 && "k must be greater than 0");
    if (!k || !word) {
        throw std::logic_error("bad buffer");
    }
    is >> std::noskipws;
    char next = 0;
    size_t i = 0;
    while (is >> next && c(next)) {}
    if (c(next)) {
        size = 0;
        word[0] = '\0';
        return is;
    }
    word[i++] = next;
    for (; i < k; ++i) {
        is >> next;
        if (c(next)) {
            break;
        }
        word[i] = next;
    }
    if (i == k) {
        is.clear(is.rdstate() | std::ios::failbit);
    }
    size = i;
    word[size] = '\0';
    return is >> std::noskipws;
}

bool is_space(char c) { return std::isspace(c); }

int main() {
    constexpr size_t cmds_count = 2;
    void (*cmds[cmds_count])(std::ostream &os, std::istream &is) = {hi, hello};
    const char *const cmds_text[] = {"hi", "hello"};
    constexpr size_t bsize = 255;
    char word[bsize + 1] = {};
    size_t size = 0;
    while (!(getword(std::cin, word, bsize, is_space, size).eof())) {
        if (std::cin.fail()) {
            std::cerr << "<invalid input>\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            if (size_t i = match(word, cmds_text, cmds_count); i < cmds_count) {
                try {
                    cmds[i](std::cout, std::cin);
                } catch (const std::exception &e) {
                    std::cerr << "<INVALID COMMAND: " << e.what() << ">\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            } else {
                std::cerr << "<read the docs!>\n";
            }
        }
    }
}
