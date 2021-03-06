#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "measurement.hpp"
#include "plot_drawer.hpp"
#include "file_reader.hpp"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QProgressDialog;
QT_END_NAMESPACE

class PlotWidget;
class PreviewPlotFrameItem;
class PreviewPlotScene;
class DataReadProgressDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void loadFile();
    void onPreviewFrameItemPosChanged();
    void updatePreviewPlot();
    void updatePlot();
    void showFileInfo();

private:
    int showReadingErrorsMessage(const FileReader& reader) const;

private slots:
    void onPreviewViewSizeChanged();
    void onFrameItemChanged();
    void onDataReadFinished();
    void onPlotWidgetPointChanged(size_t point, const QPointF& value);

private:
    Measurement measurement_;
    Ui::MainWindow* ui_;
    PlotDrawer plot_drawer_;
    PreviewPlotScene* preview_scene_;
    PlotWidget* plot_widget_;
    FileReader file_reader_;
    QProgressDialog* data_read_progress_dialog_;
};
#endif // MAINWINDOW_HPP
