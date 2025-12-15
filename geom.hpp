#ifndef GEOM_HPP
#define GEOM_HPP

namespace top
{
struct p_t
{
  int x, y;
};
bool operator==(p_t, p_t);
bool operator!=(p_t, p_t);

} // namespace top
#endif
