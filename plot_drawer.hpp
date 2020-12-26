#ifndef PLOTDRAWER_HPP
#define PLOTDRAWER_HPP

#include <QObject>
#include <QPixmap>
#include <QRectF>

#include <functional>

struct Measurement;

class PlotDrawer: public QObject
{
    Q_OBJECT

public:
    PlotDrawer(const Measurement* measurement = nullptr);

    void generatePreview(int width, int height);
    void generatePlotArea(int first, int last, int width, int height);
    const QPixmap& plot() const;
    const QPixmap& plotPreview() const;
    const Measurement *measurement() const;
    void setMeasurement(const Measurement *measurement);

private:
    const Measurement* measurement_;
    size_t default_rarefraction_ = 500;
    QPixmap plot_;
    QPixmap plot_preview_;
    QRectF size_;
};

#endif // PLOTDRAWER_HPP
