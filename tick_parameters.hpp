#ifndef TICKPARAMETERS_HPP
#define TICKPARAMETERS_HPP

#include <Qt>

struct TickParameters {
    qreal ticks_delta = 20.;
    qreal tick_short_length = 10.;
    qreal tick_long_length = 15.;
    qreal tick_text_offset_x = 5.;
    qreal tick_text_offset_y = 10.;
};

#endif // TICKPARAMETERS_HPP
