#ifndef MEASUREMENT_HPP
#define MEASUREMENT_HPP

#include "header.hpp"
#include "data_stats.hpp"

#include <QPointF>

#include <vector>

struct Measurement
{
    Header header;
    std::vector<QPointF> data;
    DataStats stats;
};

#endif // MEASUREMENT_HPP
