#include "geom.hpp"
#ifndef CANVAS_HPP
#define CANVAS_HPP
namespace top
{
    char *canvas(f_t fr, char fill);
    void paint(char *cnv, f_t fr, p_t p, char fill);
    void flush(std::ostream &os, const char *cnv, f_t fr);


}

#endif