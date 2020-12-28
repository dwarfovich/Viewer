#ifndef PLOTDRAWER_HPP
#define PLOTDRAWER_HPP

#include <QObject>
#include <QPixmap>
#include <QRectF>
#include <QPen>

struct Measurement;

class PlotDrawer: public QObject
{
    Q_OBJECT

    struct PlotParameters {
        QPixmap& pixmap;
        int width = 0;
        int height = 0;
        size_t rarefaction = 0;
        size_t first_point = 0;
        size_t last_point = 0;
        QPointF min_values = {};
        QPointF max_values = {};
        QPen pen = {Qt::green};
        QColor background_color = Qt::black;
    };

public:
    PlotDrawer(const Measurement* measurement = nullptr);

    void drawPlot(const PlotParameters& parameters);
    void drawPreview(int width, int height);
    void drawMainPlot(size_t first_point, size_t last_point, int width, int height);
    const QPixmap& plot() const;
    const QPixmap& plotPreview() const;
    void setMeasurement(const Measurement *measurement);
    const Measurement* measurement() const;


private:
    const Measurement* measurement_;
    size_t default_rarefraction_ = 500;
    QPixmap plot_;
    QPixmap plot_preview_;
};

#endif // PLOTDRAWER_HPP
