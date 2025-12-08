#include <iostream>
namespace top
{
struct p_t
{
  int x, y;
};
bool operator==(p_t, p_t);
bool operator!=(p_t, p_t);
struct f_t
{
  p_t aa, bb;
};

struct IDraw
{
  virtual ~IDraw() = default;
  virtual p_t begin() const = 0;
  virtual p_t next(p_t) const = 0;
};

struct Dot : IDraw
{
  Dot(int x, int y);
  explicit Dot(p_t dd);
  p_t begin() const override;
  p_t next(p_t) const override;
  p_t d;
};

size_t points(const IDraw &d, p_t **pts, size_t s);

f_t frame(const p_t *pts, size_t s);

char *canvas(f_t fr, char fill);

void paint(char *cnv, f_t fr, p_t p, char fill);

void flush(std::ostream &os, const char *cnv, f_t fr);
void extend(p_t **pts, size_t s, p_t p);
size_t rows(f_t);
size_t cols(f_t);
} // namespace top

int main()
{
  using top::Dot;
  using top::f_t;
  using top::IDraw;
  using top::p_t;
  size_t error = 0;
  IDraw *shps[3] = {};
  p_t *pts = nullptr;
  size_t s = 0;
  try
  {
    shps[0] = new Dot(0, 0);
    shps[1] = new Dot(5, 7);
    shps[2] = new Dot(-100, -2);
    for (size_t i = 0; i < 3; ++i)
    {
      s += points(*(shps[i]), &pts, s);
    }
    f_t fr = frame(pts, s);
    char *cnv = canvas(fr, '.');
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
  return error;
}
top::Dot::Dot(p_t dd) : IDraw(), d{dd} {}
top::Dot::Dot(int x, int y) : IDraw(), d{x, y} {}
top::p_t top::Dot::begin() const { return d; }
top::p_t top::Dot::next(p_t prev) const
{
  if (prev != begin())
  {
    throw std::logic_error("wrong prev");
  }
  return d;
}
bool top::operator==(p_t a, p_t b) { return a.x == b.x && a.y == b.y; }
bool top::operator!=(p_t a, p_t b) { return !(a == b); }

void top::extend(p_t **pts, size_t s, p_t p)
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
size_t top::points(const IDraw &d, p_t **pts, size_t s)
{
  p_t p = d.begin();
  extend(pts, s, p);
  size_t delta = 1;

  while (d.next(p) != d.begin())
  {
    p = d.next(p);
    extend(pts, s + delta, p);
    ++delta;
  }
  return delta;
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

char *top::canvas(f_t fr, char fill)
{
  char *cnv = new char[rows(fr) * cols(fr)];
  for (size_t i = 0; i < rows(fr) * cols(fr); ++i)
  {
    cnv[i] = fill;
  }
  return cnv;
}

void top::paint(char *cnv, f_t fr, p_t p, char fill)
{
  int dx = p.x - fr.aa.x;
  int dy = fr.bb.y - p.y;
  cnv[dy * cols(fr) + dx] = fill;
}

void top::flush(std::ostream &os, const char *cnv, f_t fr)
{
  for (size_t i = 0; i < rows(fr); ++i)
  {
    for (size_t j = 0; j < cols(fr); ++j)
    {
      os << cnv[i * cols(fr) + j];
    }
    os << '\n';
  }
}
