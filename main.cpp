#include <cstddef>
#include <iostream>

namespace top
{
struct p_t
{
  int x, y;
};

void add(p_t &p, int dx, int dy)
{
  p.x += dx;
  p.y += dy;
}

struct IMv
{
  virtual void mv(int dx, int dy) = 0;
  virtual ~IMv() = default;
};

struct Pnt : IMv
{
  p_t a;
  Pnt(p_t aa) : a(aa) {}
  void mv(int dx, int dy) override { add(a, dx, dy); }
};

struct Iarea
{
  virtual double area() const = 0;
  virtual ~Iarea() = default;
};

struct Circle : Iarea
{
  Pnt center;
  double rad;
  double area() const override
  {
    const double pi = 3.14159265358979323846;
    return pi * rad * rad;
  }
};

struct Square : Iarea
{
  Pnt leftTop;
  double side;
  double area() const override { return side * side; }
};

struct Rectangle : Iarea
{
  Pnt leftTop, rightBot;
  double area() const override
  {
    return static_cast<double>(rightBot.a.x - leftTop.a.x) *
           static_cast<double>(rightBot.a.y - leftTop.a.y);
  }
};

struct IDraw
{
  virtual p_t begin() const = 0;
  virtual p_t next(p_t) const = 0;
};

bool operator==(p_t a, p_t b) { return a.x == b.x && a.y == b.y; }
bool operator!=(p_t a, p_t b) { return !(a == b); }

struct Dot : IDraw
{
  p_t o;
  Dot(int x, int y) : o{x, y} {}
  p_t begin() const override { return o; }
  p_t next(p_t) const override { return o; }
};
struct frame_t
{
  p_t left_bot;
  p_t right_top;
};

void make_f(IDraw **b, size_t k);
void get_points(IDraw *b, p_t **ps, size_t s);
frame_t build_frame(const p_t ps ,size_t s);
char * build_canvas (frame_t f) {}
} // namespace top
using namespace top;
int main()
{
  IDraw *f[3] = {};
  p_t *p = nullptr;
  try
  {
    make_f(f, 3);
    for (size_t i = 0; i < 3; ++ i){ 
        get_points (f[i], *p, 3);
    }
  }
}
