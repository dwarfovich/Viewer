#ifndef DATA_HPP
#define DATA_HPP

#include "header.hpp"

#include <QPointF>

#include <vector>

class Data
{
    friend class FileReader;
public:
    Data();

    Header header() const;
    void setHeader(const Header &header);

public:
    std::vector<QPointF> data_;
    Header header_;
};

#endif // DATA_HPP
