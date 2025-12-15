#ifndef GEOM_HPP
#define GEOM_HPP
#include <cstddef>
namespace top
{
struct p_t
{
  int x, y;
};
struct f_t
{
  p_t aa, bb;
};
bool operator==(p_t, p_t);
bool operator!=(p_t, p_t);

size_t rows(f_t);
size_t cols(f_t);
} // namespace top
#endif
