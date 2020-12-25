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
    PlotDrawer(const Measurement& measurement, const QRectF& size = {});

    void draw();
    void generatePreview(int width, int height);
    const QPixmap& plot() const;
    const QPixmap& plot_preview() const;

private:
    const Measurement& measurement_;
    size_t default_rarefraction_ = 500;
    QPixmap plot_;
    QPixmap plot_preview_;
    QRectF size_;
};

#endif // PLOTDRAWER_HPP
