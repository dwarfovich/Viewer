#include "ruler_widget.hpp"

#include <QPainter>
#include <QPen>

#include <QDebug>
#define DEB qDebug()

RulerWidget::RulerWidget(QWidget* parent)
    : QWidget{parent}
{
    parameters_y.tick_text_offset_y = 3.;
}

Qt::Orientation RulerWidget::orientation() const
{
    return orientation_;
}

void RulerWidget::setRange(const std::pair<qreal, qreal>& range)
{
    min_value_ = range.first;
    max_value_ = range.second;
    update();
}

void RulerWidget::setOrientation(const Qt::Orientation& orientation)
{
    orientation_ = orientation;
}

void RulerWidget::paintEvent(QPaintEvent* event)
{
    if (orientation_ == Qt::Vertical) {
        paintVerticalRuler();
    } else {
        paintHorizontalRuler();
    }
}

void RulerWidget::paintVerticalRuler()
{
    QPainter painter {this};
    painter.drawLine(width() - 1, 0, width() - 1, height());
    int ticks = ceil(height() / parameters_y.ticks_delta);
    for (int i = 0; i <= ticks; ++i) {
        qreal y = parameters_y.ticks_delta * i;
        if (i % 2 == 0) {
            painter.drawLine(width() - parameters_y.tick_long_length, y, width(), y);
            if (i != 0) {
                qreal y_range = max_value_ - min_value_;
                qreal delta = y_range / ticks;
                qreal tick = delta * i + min_value_;
                QString text = tickText(tick);
                const auto& metrics = painter.fontMetrics();;
                int text_width = metrics.width(text);
                qreal text_x = width() - parameters_y.tick_long_length - text_width - parameters_y.tick_text_offset_x;
                qreal text_y = y + metrics.height() / 2 - parameters_y.tick_text_offset_y;
                painter.drawText(text_x, text_y, text);
            }
        } else {
            painter.drawLine(width() - parameters_y.tick_short_length, y, width(), y);
        }
    }
}

void RulerWidget::paintHorizontalRuler()
{
    QPainter painter {this};
    painter.drawLine(0, 0, width(), 0);
    int ticks = ceil(width() / parameters_x.ticks_delta);
    for (int i = 0; i <= ticks; ++i) {
        qreal x = parameters_x.ticks_delta * i;
        if (i % 2 == 0) {
            painter.drawLine(x, 0, x, parameters_x.tick_long_length);
            painter.save();
            painter.translate(x , parameters_x.tick_long_length + parameters_x.tick_text_offset_y);
            painter.rotate(45.);
            qreal x_range = max_value_ - min_value_;
            qreal delta = x_range / ticks;
            qreal tick = delta * i + min_value_;
            QString text = tickText(tick);
            painter.drawText(0, 0, text);
            painter.restore();
        } else {
            painter.drawLine(x, 0, x, parameters_x.tick_short_length);
        }
    }
}

QString RulerWidget::tickText(qreal value) const
{
    if (value == 0) {
        return QString::number(0, 'f', 1);
    } else {
        return QString::number(value, 'e', 3);
    }
}
