#include <cctype>
#include <iostream>
#include <istream>
#include <ostream>

void hi() { std::cout << "<HI!>\n"; }
size_t match(const char * word, const char* const * words, size_t k);
std::istream getword(std::istream&is, char* word, size_t k, bool(*c)(char));
bool is_space(char c) {
    return std::isspace(c);
}
int main()
{
  constexpr size_t cmds_count = 1;
  void (*cmds[cmds_count])() = {hi};
  const char * const cmds_text[] = {"hi"};
  constexpr size_t bsize = 255;
  char word[bsize+1] = {};
  while (!(getword(std::cin, word, bsize, is_space).eof()))
  {
    if (std::cin.fail())
    {
      std::cerr << "<invalid input>\n";
      return 1; // нужно масками вернуть норм инпут чтобы цикла не было
                // бесконенчного
    }
    else if (size_t i = match(word, cmds_text, cmds_count); i < cmds_count)
    {
      cmds[i]();
    }
    else
    {
      std::cerr << "<read the docs!>\n";
    }
  }
}
