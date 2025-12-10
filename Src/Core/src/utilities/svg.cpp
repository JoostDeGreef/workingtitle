#include <fstream>
#include <iostream>
#include <sstream>

#include "internal/utilities/svg.h"

void SVG::writeToStream(std::ostream& os) const
{
    // header
    os << "<svg xmlns = \"http://www.w3.org/2000/svg\"\n"
          "     xmlns:xlink = \"http://www.w3.org/1999/xlink\"\n"
          "     width = \"" << width << "\" height = \"" << height << "\" viewBox = \"" << viewBox << "\">\n";
    // footer
    os << "</svg>\n";
}

void SVG::writeToFile(const std::string& filename) const
{
    std::fstream f(filename, std::fstream::out | std::fstream::trunc);
    writeToStream(f);
}

std::string SVG::writeToString() const
{
    std::stringstream s;
    writeToStream(s);
    return s.str();
}

void SVG::addShape(const Shape& shape)
{
}
