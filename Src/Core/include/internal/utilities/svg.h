#pragma once

#include <string>
#include <ostream>

class SVG
{
public:
    SVG()
    {}

    void writeToStream(std::ostream & os) const;
    void writeToFile(const std::string & filename) const;
    std::string writeToString() const;
private:
};