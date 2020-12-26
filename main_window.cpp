#include "main_window.hpp"
#include "ui_main_window.h"
#include "measurement.hpp"
#include "file_reader.hpp"
#include "plot_drawer.hpp"
#include "plot_widget.hpp"
#include "preview_plot_frame_item.hpp"
#include "preview_plot_scene.hpp"

#include <QGraphicsScene>
#include <QAction>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QScrollBar>
#include <QTimer>

#include <QDebug>
#define DEB qDebug()

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
    , ui_{new Ui::MainWindow}
    , preview_scene_{new PreviewPlotScene {plot_drawer_, this}}
    , plot_widget_{new PlotWidget {plot_drawer_, this}}
{
    ui_->setupUi(this);
    ui_->previewPlotView->setScene(preview_scene_);
    ui_->previewPlotView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui_->previewPlotView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui_->splitter->setSizes({10, 200});
    ui_->scrollArea->setWidget(plot_widget_);

    plot_drawer_.setMeasurement(&measurement_);

    connect(ui_->previewPlotView, &PreviewPlotView::frameItemPosChanged,
            this, &MainWindow::onPreviewFrameItemPosChanged);
    connect(ui_->previewPlotView, &PreviewPlotView::sizeChanged,
            this, &MainWindow::onPreviewViewSizeChanged);
    connect(plot_widget_, &PlotWidget::scaleChangeRequest,
            preview_scene_, &PreviewPlotScene::onPlotScaleRequest);
    connect(ui_->previewPlotView, &PreviewPlotView::scaleChangeRequest,
            preview_scene_, &PreviewPlotScene::onPlotScaleRequest);
    connect(preview_scene_, &PreviewPlotScene::frameItemChanged,
            this, &MainWindow::onFrameItemChanged);

    resize(1400, 900);
    connect(ui_->actionLoad, &QAction::triggered, this, &MainWindow::loadFile);
    connect(ui_->actionQuit, &QAction::triggered, this, &MainWindow::close);

    show();
    loadFile();
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::loadFile()
{
    auto filename = QFileDialog::getOpenFileName(this, tr("Select file"), "C:/Boo/Code/Viewer/SampleFiles");
    FileReader reader {measurement_};
    reader.readFile(filename);
    updatePlot();
}

void MainWindow::onPreviewFrameItemPosChanged(const QPointF &delta_pos)
{
    auto* frame_item = preview_scene_->frameItem();
    qreal new_x = 0.;
    if (delta_pos.x() < 0.) {
        new_x = std::max(frame_item->x() + delta_pos.x(), 0.);
    } else {
        new_x = std::min(delta_pos.x(), qreal(ui_->previewPlotView->width() - frame_item->width()));
    }
    frame_item->setX(new_x);

    updatePlot();
}

void MainWindow::updatePlot()
{
    preview_scene_->updatePreview(ui_->previewPlotView->width(), ui_->previewPlotView->height());
    auto* frame_item = preview_scene_->frameItem();
    frame_item->setHeight(ui_->previewPlotView->height());
    Q_ASSERT(frame_item);
    qreal preview_width = ui_->previewPlotView->width();
    int first = (frame_item->x() * 100) / preview_width;
    int last = ((frame_item->x() + frame_item->width()) * 100) / preview_width;
    int plot_area_width = plot_widget_->width();
    int plot_area_height = plot_widget_->height();
    plot_drawer_.generatePlotArea(first, last, plot_area_width, plot_area_height);
    plot_widget_->drawArea(first, last);
}

void MainWindow::onPreviewViewSizeChanged(const QSize &size)
{
    updatePlot();
}

void MainWindow::onFrameItemChanged(const QRectF &rect)
{
    updatePlot();
}
