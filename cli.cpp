#include <iostream>

void hi() { std::cout << "<HI!>\n"; }
int main()
{
  constexpr size_t cmds_cout = 1;
  void (*cmds[cmds_cout])() = {hi};
  size_t i = 0;
  while (!(std::cin >> i).eof())
  {
    if (std::cin.fail())
    {
      std::cerr << "<invalid input>\n";
      return 1; // нужно масками вернуть норм инпут чтобы цикла не было
                // бесконенчного
    }
    else if (i < cmds_cout)
    {
      cmds[i]();
    }
    else
    {
      std::cerr << "<read the docs!>\n";
    }
  }
}
