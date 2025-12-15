#ifndef DOT_HPP
#define DOT_HPP
#include "geom.hpp"
#include "idraw.hpp"
namespace top {
    struct Dot : IDraw
    {
      Dot(int x, int y);
      explicit Dot(p_t dd);
      p_t begin() const override;
      p_t next(p_t) const override;
    private:
      p_t d;
    };

}


#endif