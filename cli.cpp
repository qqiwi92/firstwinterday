#include <cassert>
#include <cctype>
#include <cstring>
#include <ios>
#include <iostream>
#include <istream>
#include <ostream>
#include <stdexcept>

void hi() { std::cout << "<HI!>\n"; }
void hello() { std::cout << "<HELLO!>\n"; }

size_t match(const char *word, const char *const *words, size_t k)
{
  for (size_t i = 0; i < k; ++i)
  {
    bool matching = std::strlen(word) == std::strlen(words[i]);
    matching = matching && !std::strcmp(word, words[i]);
    if (matching)
    {
      return i;
    }
  }
  return k;
}

std::istream &getword(std::istream &is, char *word, size_t k, bool (*c)(char), size_t &size)
{
  assert(k > 0 && "k must be greater than 0");
  if (!k || !word)
  {
    throw std::logic_error("bad buffer");
  }
  is >> std::noskipws;
  char next = 0;
  size_t i = 0;
  for (; i < k; ++i)
  {
    is >> next;
    if (c(next))
    {
      break;
    }
    word[i] = next;
  }
  if (i == k)
  {
    is.clear(is.rdstate() | std::ios::failbit);
  }
  size = i;
  word[size] = 0;
  return is >> std::noskipws;
}

bool is_space(char c) { return std::isspace(c); }
int main()
{
  constexpr size_t cmds_count = 2;
  
  void (*cmds[cmds_count])() = {hi, hello};
  const char *const cmds_text[] = {"hi", "hello"};
  constexpr size_t bsize = 255;
  char word[bsize + 1] = {};
  size_t size = 0;
  while (!(getword(std::cin, word, bsize, is_space, size).eof()))
  {
    if (std::cin.fail())
    {
      std::cerr << "<invalid input>\n";
      return 1; // нужно масками вернуть норм инпут чтобы цикла не было
                // бесконенчного
    }
    else
    {
      word[size-1] = '\0';
      if (size_t i = match(word, cmds_text, cmds_count); i < cmds_count)
      {
        cmds[i]();
      }
      else
      {
        std::cerr << "<read the docs!>\n";
      }
    }
  }
}
