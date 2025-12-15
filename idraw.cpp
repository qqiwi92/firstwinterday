#include "idraw.hpp"
#include "geom.hpp"

using top::p_t;

namespace
{
// или статик сюда добавляем
void extend(p_t **pts, size_t s, p_t p)
{
  p_t *res = new p_t[s + 1];
  for (size_t i = 0; i < s; ++i)
  {
    res[i] = (*pts)[i];
  }
  res[s] = p;
  delete[] *pts;
  *pts = res;
}

} // namespace
size_t top::points(const IDraw &d, p_t **pts, size_t s)
{
  p_t p = d.begin();
  extend(pts, s, p);
  size_t delta = 1;

  for (;;)
  {
    p_t nxt = d.next(p);
    if (nxt == d.begin())
      break;
    p = nxt;
    extend(pts, s + delta, p);
    ++delta;
  }
  return delta;
}
