#include "dot.hpp"
#include "geom.hpp"
#include "idraw.hpp"
#include <cmath>
#include <iostream>
namespace top
{

struct HorizontalLine : IDraw
{
  HorizontalLine(p_t left_end, size_t len);
  p_t begin() const override;
  p_t next(p_t) const override;
  p_t left_end;
  size_t len;
};

struct VerticalLine : IDraw
{
  VerticalLine(p_t top_end, size_t len);
  p_t begin() const override;
  p_t next(p_t) const override;
  p_t top_end;
  size_t len;
};

struct DiagonalLine : IDraw
{
  DiagonalLine(p_t left_bottom, size_t len);
  p_t begin() const override;
  p_t next(p_t) const override;
  p_t left_bottom;
  size_t len; // not true
};

struct Rectangle : IDraw
{
  Rectangle(p_t left_bottom, p_t right_top);
  p_t begin() const override;
  p_t next(p_t) const override;
  p_t left_bottom, right_top;
};

struct Square : IDraw
{
  Square(p_t left_bottom, int side); // так то отрицательная сторона тоже норм
  p_t begin() const override;
  p_t next(p_t) const override;
  Rectangle shape;
};

f_t frame(const p_t *pts, size_t s);

} // namespace top

top::p_t top::HorizontalLine::begin() const { return left_end; }

top::p_t top::HorizontalLine::next(p_t prev) const
{
  if (prev.x - left_end.x + 1 >= static_cast<int>(len))
  {
    return left_end;
  }

  return {prev.x + 1, prev.y};
}

top::p_t top::VerticalLine::begin() const { return top_end; }

top::p_t top::VerticalLine::next(p_t prev) const
{
  if (top_end.y - prev.y + 1 >= static_cast<int>(len))
  {
    return top_end;
  }

  return {prev.x, prev.y - 1};
}

top::p_t top::DiagonalLine::begin() const { return left_bottom; }

top::p_t top::DiagonalLine::next(p_t prev) const
{
  if (prev.x - left_bottom.x + 1 >= static_cast<int>(len))
  {
    return left_bottom;
  }

  return {prev.x + 1, prev.y + 1};
}

top::p_t top::Rectangle::begin() const { return {left_bottom.x, right_top.y}; }

top::p_t top::Rectangle::next(p_t prev) const
{
  if (prev.y == right_top.y && prev.x < right_top.x)
  {
    return {prev.x + 1, prev.y};
  }
  else if (prev.x == right_top.x && prev.y > left_bottom.y)
  {
    return {prev.x, prev.y - 1};
  }
  else if (prev.y == left_bottom.y && prev.x > left_bottom.x)
  {
    return {prev.x - 1, prev.y};
  }
  else if (prev.x == left_bottom.x && prev.y < right_top.y)
  {
    return {prev.x, prev.y + 1};
  }
  else
  {
    return begin();
  }
}

top::p_t top::Square::begin() const
{
  return {shape.left_bottom.x, shape.right_top.y};
}

top::p_t top::Square::next(p_t prev) const { return shape.next(prev); }

top::HorizontalLine::HorizontalLine(top::p_t left_end, size_t len)
    : IDraw(), left_end{left_end}, len{len}
{
}

top::VerticalLine::VerticalLine(top::p_t top_end, size_t len)
    : IDraw(), top_end{top_end}, len{len}
{
}

top::DiagonalLine::DiagonalLine(p_t s, size_t l) : left_bottom{s}, len{l} {}

top::Rectangle::Rectangle(p_t left_bottom, p_t right_top)
    : left_bottom(left_bottom), right_top(right_top)
{
}

top::Square::Square(p_t left_bottom, int side)
    : shape(left_bottom, {left_bottom.x + side, left_bottom.y + side})
{
}

top::f_t top::frame(const p_t *pts, size_t s)
{
  if (!s)
  {
    throw std::logic_error("wrong size");
  }
  int minx = pts[0].x, maxx = minx;
  int miny = pts[0].y, maxy = miny;
  for (size_t i = 1; i < s; ++i)
  {
    minx = std::min(minx, pts[i].x);
    maxx = std::max(maxx, pts[i].x);
    miny = std::min(miny, pts[i].y);
    maxy = std::max(maxy, pts[i].y);
  }
  p_t aa{minx, miny};
  p_t bb{maxx, maxy};
  return {aa, bb};
}

size_t top::rows(f_t fr) { return (fr.bb.y - fr.aa.y + 1); }

size_t top::cols(f_t fr) { return (fr.bb.x - fr.aa.x + 1); }

int main()
{
  using top::DiagonalLine;
  using top::Dot;
  using top::f_t;
  using top::HorizontalLine;
  using top::IDraw;
  using top::p_t;
  using top::Rectangle;
  using top::Square;
  using top::VerticalLine;
  size_t error = 0;
  IDraw *shps[100] = {};
  p_t *pts = nullptr;
  size_t s = 0;
  try
  {
    shps[0] = new Square({0, -2}, 0);
    shps[1] = new HorizontalLine({4, 4}, 2);
    shps[2] = new Dot(-10, -2);
    shps[3] = new VerticalLine({-1, -3}, 3);
    for (size_t i = 0; i < 1; ++i)
    {
      s += points(*(shps[i]), &pts, s);
    }
    f_t fr = frame(pts, s);
    char *cnv = canvas(fr, '~');
    for (size_t i = 0; i < s; ++i)
    {
      paint(cnv, fr, pts[i], '#');
    }
    flush(std::cout, cnv, fr);
    delete[] cnv;
  }
  catch (...)
  {
    error = 2;
    std::cerr << "something went wrong\n";
  }
  delete[] pts;
  delete shps[0];
  delete shps[1];
  delete shps[2];
  delete shps[3];
  return error;
}
