#include "geom.hpp"

bool top::operator==(p_t a, p_t b) { return a.x == b.x && a.y == b.y; }
bool top::operator!=(p_t a, p_t b) { return !(a == b); }
