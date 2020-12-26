#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "measurement.hpp"
#include "plot_drawer.hpp"
#include "file_reader.hpp"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class PlotWidget;
class PreviewPlotFrameItem;
class PreviewPlotScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void loadFile();
    void onPreviewFrameItemPosChanged(const QPointF& new_pos);

private:
    void updatePlot();
    int showReadingErrorsMessage(const FileReader& reader) const;

private slots:
    void onPreviewViewSizeChanged(const QSize& size);
    void onFrameItemChanged(const QRectF& rect);
    void onDataReadFinished();

private:
    Measurement measurement_;
    Ui::MainWindow* ui_;
    PlotDrawer plot_drawer_;
    PreviewPlotScene* preview_scene_;
    PlotWidget* plot_widget_;
    FileReader file_reader_;
};
#endif // MAINWINDOW_HPP
