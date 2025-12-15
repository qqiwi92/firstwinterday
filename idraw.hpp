#ifndef IDRAW
#define IDRAW
#include <cstddef>
#include "geom.hpp"
namespace top
{
struct IDraw
{
  virtual ~IDraw() = default;
  virtual p_t begin() const = 0;
  virtual p_t next(p_t) const = 0;
};
size_t points(const IDraw &d, p_t **pts, size_t s);

} // namespace top

#endif
