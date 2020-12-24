#include "data.hpp"

Data::Data()
{}

Header Data::header() const
{
    return header_;
}

void Data::setHeader(const Header &header)
{
    header_ = header;
}
