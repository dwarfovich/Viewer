#ifndef PLOTDRAWER_HPP
#define PLOTDRAWER_HPP

#include <QObject>
#include <QPixmap>
#include <QRectF>

struct Measurement;

class PlotDrawer: public QObject
{
    Q_OBJECT

public:
    PlotDrawer(const Measurement& measurement, const QRectF& size);

    void draw();

    QPixmap plot() const;

private:
    const Measurement& measurement_;
    QPixmap plot_;
    QRectF size_;
};

#endif // PLOTDRAWER_HPP
