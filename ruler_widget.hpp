#ifndef RULERWIDGET_HPP
#define RULERWIDGET_HPP

#include "tick_parameters.hpp"

#include <QWidget>

class RulerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RulerWidget(QWidget* parent = nullptr);

    Qt::Orientation orientation() const;
    void setOrientation(const Qt::Orientation& orientation);

public slots:
    void setRange(const std::pair<qreal, qreal>& range);

protected:
    void paintEvent(QPaintEvent* event) override;
    virtual void paintVerticalRuler();
    virtual void paintHorizontalRuler();

private:
    QString tickText(qreal value) const;

private:
    Qt::Orientation orientation_ = {};
    qreal min_value_ = 0.;
    qreal max_value_ = 0.;
    TickParameters parameters_x;
    TickParameters parameters_y;
};

#endif // RULERWIDGET_HPP
