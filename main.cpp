#include <iostream>
#include <cmath>

namespace top {
  struct p_t {
    int x, y;
  };

  struct IDraw {
    virtual p_t begin() const = 0;
    virtual p_t next(p_t) const = 0;
  };

  bool operator==(p_t a, p_t b)
  {
    return a.x == b.x && a.y == b.y;
  }

  bool operator!=(p_t a, p_t b)
  {
    return !(a == b);
  }

  struct Dot: IDraw {
    Dot(int x, int y);
    explicit Dot(p_t p);
    p_t begin() const override;
    p_t next(p_t p) const override;
    p_t o;
  };

  struct VSeg: IDraw {
    VSeg(int x, int y, int l);
    VSeg(p_t p, int l);
    p_t begin() const override;
    p_t next(p_t p) const override;
    p_t start;
    int length;
  };

  struct HSeg: IDraw {
    HSeg(int x, int y, int l);
    HSeg(p_t p, int l);
    p_t begin() const override;
    p_t next(p_t p) const override;
    p_t start;
    int length;
  };

  struct Circle: IDraw {
    Circle(int x, int y, int r);
    Circle(p_t p, int r);
    p_t begin() const override;
    p_t next(p_t p) const override;
    p_t o;
    int radius;
  };

  struct frame_t {
    p_t left_bot;
    p_t right_top;
  };

  void extend(p_t ** ps, size_t s, p_t p);
  void make_f(IDraw ** b, size_t k);
  void get_points(top::IDraw & b, p_t ** ps, size_t & s);
  frame_t build_frame(const p_t * ps, size_t s);
  char * build_canvas(frame_t f);
  void paint_canvas(char * cnv, frame_t fr, const p_t * ps, size_t k, char f);
  void print_canvas(const char * cnv, frame_t fr);
}

int main()
{
  using namespace top;
  int err = 0;
  const size_t count = 3;
  IDraw * f[count] = {};
  p_t * p = nullptr;
  size_t s = 0;
  char * cnv = nullptr;
  try {
    make_f(f, 3);
    for (size_t i = 0; i < 3; ++i) {
      get_points((*f[i]), &p, s);
    }
    frame_t fr = build_frame(p, s);
    cnv = build_canvas(fr);
    paint_canvas(cnv, fr, p, s, '#');
    print_canvas(cnv, fr);
  } catch (...) {
    err = 1;
  }
  delete f[0];
  delete f[1];
  delete f[2];
  delete [] p;
  delete [] cnv;
  return err;
}

top::Dot::Dot(int x, int y):
  IDraw(),
  o{x, y}
{}

top::Dot::Dot(p_t p):
  IDraw(),
  o(p)
{}

top::p_t top::Dot::begin() const
{
  return o;
}

top::p_t top::Dot::next(p_t p) const
{
  return begin();
}

top::VSeg::VSeg(int x, int y, int l):
  IDraw(),
  start{x, y},
  length(l)
{
  if (length == 0) {
    throw std::invalid_argument("lenght can not be 0");
  }
  if (length < 0) {
    length *= -1;
    start.y -= length;
  }
}

top::VSeg::VSeg(p_t p, int l)
{
  VSeg(p.x, p.y, l);
}

top::p_t top::VSeg::begin() const
{
  return start;
}

top::p_t top::VSeg::next(p_t p) const
{
  if (p.y == start.y + length - 1) {
    return begin();
  }
  return p_t{start.x, p.y + 1};
}

top::HSeg::HSeg(int x, int y, int l):
  IDraw(),
  start{x, y},
  length(l)
{
  if (length == 0) {
    throw std::invalid_argument("lenght can not be 0");
  }
  if (length < 0) {
    length *= -1;
    start.x -= length;
  }
}

top::HSeg::HSeg(p_t p, int l)
{
  HSeg(p.x, p.y, l);
}

top::p_t top::HSeg::begin() const
{
  return start;
}

top::p_t top::HSeg::next(p_t p) const
{
  if (p.x == start.x + length - 1) {
    return begin();
  }
  return p_t{p.x + 1, start.y};
}

top::Circle::Circle(int x, int y, int r):
  IDraw(),
  o{x, y},
  radius(r)
{
  if (radius == 0) {
    throw std::invalid_argument("radius can't be 0");
  }
  if (radius < 0) {
    radius *= -1;
  }
}

top::Circle::Circle(p_t p, int r)
{
  Circle(p.x, p.y, r);
}

top::p_t top::Circle::begin() const
{
  return p_t{o.x + radius, o.y};
}

top::p_t top::Circle::next(p_t p) const
{
  if (radius == 1) {
    return begin();
  }
  const double PI = acos(-1.0);
  int dx = p.x - o.x;
  int dy = p.y - o.y;
  int angle = atan2(dy, dx) * 180.0 / PI;
  if (angle < 0) {
    angle += 360;
  }
  for (int i = 0; i < 360; ++i) {
    int new_angle = (angle + i) % 360;
    int new_x = o.x + radius * cos(new_angle);
    int new_y = o.y + radius * sin(new_angle);
    p_t new_p{new_x, new_y};
    if (new_p != p) {
      return new_p;
    }
  }
  return p;
}

void top::make_f(top::IDraw ** b, size_t k)
{
  b[0] = new Dot(0, 0);
  b[1] = new Dot(-1, -5);
  b[2] = new Dot(7, 7);
}

void top::extend(p_t ** ps, size_t s, p_t p)
{
  p_t * res = new p_t[s + 1];
  for (size_t i = 0; i < s; ++i) {
    res[i] = (*ps)[i];
  }
  res[s] = p;
  delete [] *ps;
  *ps = res;
}

void top::get_points(top::IDraw & b, p_t ** ps, size_t & s)
{
  p_t p = b.begin();
  extend(ps, s, p);
  size_t delta = 1;
  while (b.next(p) != b.begin()) {
    p = b.next(p);
    extend(ps, s + delta, p);
    ++delta;
  }
  s = delta;
}


top::frame_t top::build_frame(const top::p_t * ps, size_t s)
{
  if (!s) {
    throw std::logic_error("bad size");
  }
  int minx = ps[0].x, maxx = minx;
  int miny = ps[0].y, maxy = miny;
  for (size_t i = 1; i < s; ++i) {
    minx = std::min(minx, ps[i].x);
    maxx = std::max(maxx, ps[i].x);
    miny = std::min(miny, ps[i].y);
    maxy = std::max(maxy, ps[i].y);
  }
  p_t aa{minx, miny};
  p_t bb{maxx, maxy};
  return {aa, bb};
}

char * top::build_canvas(top::frame_t f)
{
  // count cols rows (max - min + 1)
}

void top::paint_canvas(char * cnv, top::frame_t fr, const top::p_t * ps, size_t k, char f)
{
  // different cords
}

void top::print_canvas(const char * cnv, top::frame_t fr)
{
    // ouput
}