#include <fstream>
#include <iostream>
#include <sstream>

#include "internal/utilities/svg.h"

void SVG::writeToStream(std::ostream& os) const
{
    os << "bla";
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
