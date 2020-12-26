#ifndef DATASTATS_HPP
#define DATASTATS_HPP

#include <limits>

struct DataStats
{
    double min_x = std::numeric_limits<double>::max();
    double max_x = 0.;
    double min_y = std::numeric_limits<double>::max();
    double max_y = 0.;
};

#endif // DATASTATS_HPP
