#include <cstddef>
#include <iostream>
#include <stdexcept>

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
        virtual ~IDraw() = default;
    };

    bool operator==(p_t a, p_t b) { return a.x == b.x && a.y == b.y; }
    bool operator!=(p_t a, p_t b) { return !(a == b); }

    struct Dot : IDraw
    {
        p_t o;
        Dot(int x, int y) : o{x, y} {}
        Dot(p_t p) : o(p) {}
        p_t begin() const override { return o; }
        p_t next(p_t) const override { return begin(); }
    };

    struct VSeg : IDraw
    {
        p_t start;
        int length;
        VSeg(int x, int y, int l) : start{x, y}, length(l)
        {
            if (length == 0)
                throw std::invalid_argument("length cannot be 0");
            if (length < 0)
            {
                length *= -1;
                start.y -= length;
            }
        }
        VSeg(p_t p, int l) : VSeg(p.x, p.y, l) {}
        p_t begin() const override { return start; }
        p_t next(p_t p) const override
        {
            if (p.y == start.y + length - 1)
                return begin();
            return p_t{start.x, p.y + 1};
        }
    };

    struct HSeg : IDraw
    {
        p_t start;
        int length;
        HSeg(int x, int y, int l) : start{x, y}, length(l)
        {
            if (length == 0)
                throw std::invalid_argument("length cannot be 0");
            if (length < 0)
            {
                length *= -1;
                start.x -= length;
            }
        }
        HSeg(p_t p, int l) : HSeg(p.x, p.y, l) {}
        p_t begin() const override { return start; }
        p_t next(p_t p) const override
        {
            if (p.x == start.x + length - 1)
                return begin();
            return p_t{p.x + 1, start.y};
        }
    };

    struct frame_t
    {
        p_t left_bot;
        p_t right_top;
    };

    void make_f(IDraw **b, size_t k);
    void get_points(IDraw *b, p_t **ps, size_t &s);
    frame_t build_frame(const p_t *ps, size_t s);
    char *build_canvas(frame_t f);
    void paint_canvas(char *cnv, frame_t fr, const p_t *ps, size_t k, char f);
    void print_canvas(const char *cnv, frame_t fr);
} // namespace top

void top::make_f(top::IDraw **b, size_t k)
{
    b[0] = new Dot(0, 0);
    b[1] = new VSeg(0, 0, 5);
    b[2] = new HSeg(0, 0, 5);
}

void top::get_points(top::IDraw *b, p_t **ps, size_t &s)
{
    // Collect points from the IDraw object
    // Allocate or reallocate *ps as needed
    // Update s with the new size
    // (Implementation depends on your logic)
}

top::frame_t top::build_frame(const top::p_t *ps, size_t s)
{
    // Find min and max for x and y
    // Return frame_t
    // (Implementation depends on your logic)
}

char *top::build_canvas(top::frame_t f)
{
    // Calculate number of columns and rows (max - min + 1)
    // Allocate and return canvas
    // (Implementation depends on your logic)
}

void top::paint_canvas(char *cnv, top::frame_t fr, const top::p_t *ps, size_t k, char f)
{
    // Translate coordinates and paint
    // (Implementation depends on your logic)
}

void top::print_canvas(const char *cnv, top::frame_t fr)
{
    // Print canvas to std::cout
    // (Implementation depends on your logic)
}

int main()
{
    using namespace top;
    IDraw *f[3] = {};
    p_t *p = nullptr;
    size_t s = 0;
    int err = 0;
    char *cnv = nullptr;
    try
    {
        make_f(f, 3);
        for (size_t i = 0; i < 3; ++i)
        {
            get_points(f[i], &p, s);
        }
        frame_t fr = build_frame(p, s);
        cnv = build_canvas(fr);
        paint_canvas(cnv, fr, p, s, '#');
        print_canvas(cnv, fr);
    }
    catch (...)
    {
        err = 1;
    }
    delete f[0];
    delete f[1];
    delete f[2];
    delete[] p;
    delete[] cnv;
    return err;
}
