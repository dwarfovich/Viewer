#include "main_window.hpp"
#include "ui_main_window.h"
#include "measurement.hpp"
#include "file_reader.hpp"
#include "plot_drawer.hpp"
#include "plot_widget.hpp"
#include "preview_plot_frame_item.hpp"
#include "preview_plot_scene.hpp"
#include "file_info_widget.hpp"

#include <QProgressDialog>
#include <QAction>
#include <QFileDialog>
#include <QScrollBar>
#include <QTimer>
#include <QMessageBox>

#include <QDebug>
#define DEB qDebug()

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
    , ui_{new Ui::MainWindow}
    , preview_scene_{new PreviewPlotScene {plot_drawer_, this}}
    , plot_widget_{new PlotWidget {plot_drawer_, this}}
    , data_read_progress_dialog_{new QProgressDialog {tr("Loading file"), {}, 0, 100, this}}
{
    ui_->setupUi(this);
    data_read_progress_dialog_->setWindowFlags(data_read_progress_dialog_->windowFlags() & ~Qt::WindowCloseButtonHint);
    data_read_progress_dialog_->close();

    ui_->previewPlotView->setScene(preview_scene_);
//    ui_->previewPlotView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    ui_->previewPlotView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui_->splitter->setSizes({10, 200});
    ui_->scrollArea->setWidget(plot_widget_);

    plot_drawer_.setMeasurement(&measurement_);

    ui_->verticalRulerWidget->setOrientation(Qt::Vertical);
    ui_->horizontalRulerWidget->setOrientation(Qt::Horizontal);

    connect(preview_scene_, &PreviewPlotScene::frameItemPosChanged,
            this, &MainWindow::onPreviewFrameItemPosChanged);
    connect(ui_->previewPlotView, &PreviewPlotView::sizeChanged,
            this, &MainWindow::onPreviewViewSizeChanged);
    connect(plot_widget_, &PlotWidget::scaleChangeRequest,
            preview_scene_, &PreviewPlotScene::onPlotScaleRequest);
    connect(ui_->previewPlotView, &PreviewPlotView::scaleChangeRequest,
            preview_scene_, &PreviewPlotScene::onPlotScaleRequest);
    connect(preview_scene_, &PreviewPlotScene::frameItemChanged,
            this, &MainWindow::onFrameItemChanged);
    connect(&file_reader_, &FileReader::finished,
            this, &MainWindow::onDataReadFinished);
    connect(&file_reader_, &FileReader::progressChanged,
            data_read_progress_dialog_, &QProgressDialog::setValue);
    connect(plot_widget_, &PlotWidget::verticalRangeChanged,
            ui_->verticalRulerWidget, &RulerWidget::setRange);
    connect(plot_widget_, &PlotWidget::horizontalRangeChanged,
            ui_->horizontalRulerWidget, &RulerWidget::setRange);
    connect(plot_widget_, &PlotWidget::selectedPointChanged,
            this, &MainWindow::onPlotWidgetPointChanged);


    connect(ui_->actionFileInfo, &QAction::triggered, this, &MainWindow::showFileInfo);
    connect(ui_->actionLoad, &QAction::triggered, this, &MainWindow::loadFile);
    connect(ui_->actionQuit, &QAction::triggered, this, &MainWindow::close);


    resize(1400, 900);
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
//    QString filename = "C:/Boo/Code/Viewer/SampleFiles/blocks.ssd";
    if (!filename.isEmpty()) {
        file_reader_.readFile(filename);
    }
}

void MainWindow::onPreviewFrameItemPosChanged()
{
    updatePlot();
}

void MainWindow::updatePreviewPlot()
{
//    DEB << ui_->previewPlotView->width() << ui_->previewPlotView->height();
    preview_scene_->updatePreview(ui_->previewPlotView->width(), ui_->previewPlotView->height());
}

void MainWindow::updatePlot()
{
    auto* frame_item = preview_scene_->frameItem();
    Q_ASSERT(frame_item);
    plot_widget_->drawArea(*frame_item);
}

void MainWindow::showFileInfo()
{
    auto* widget = new FileInfoWidget {nullptr, Qt::Window};
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->setMeasurement(&measurement_);
    widget->show();
}

int MainWindow::showReadingErrorsMessage(const FileReader &reader) const
{
    QString caption = tr("Warning");
    QString text;
    if (!reader.headerErrors().empty()){
        text.append(tr("There are following header errors:\n"));
        for (const auto& error : reader.headerErrors()) {
            text.append(error + '\n');
        }
    }
    if (!reader.dataErrors().empty()) {
        text.append(tr("There are following data errors:\n"));
        for (const auto& error : reader.dataErrors()) {
            text.append(error + '\n');
        }
    }

    text.append(tr("Proceed anyway?"));

    int reply = QMessageBox::question(nullptr, caption, text);
    return reply;
}

void MainWindow::onPreviewViewSizeChanged()
{
    if (measurement_.data.size() > 0){
        updatePlot();
    }
}

void MainWindow::onFrameItemChanged()
{
    updatePlot();
}

void MainWindow::onDataReadFinished()
{
    data_read_progress_dialog_->close();
    if (!file_reader_.headerErrors().empty() || !file_reader_.dataErrors().empty()) {
        auto reply = showReadingErrorsMessage(file_reader_);
        if (reply != QMessageBox::Yes) {
            return;
        }
    }

    measurement_ = file_reader_.takeMeasurement();
    Q_ASSERT(!measurement_.data.empty());

    updatePreviewPlot();
    preview_scene_->resetFrameItem(ui_->previewPlotView->height(), ui_->previewPlotView->width());
    updatePlot();
}

void MainWindow::onPlotWidgetPointChanged(size_t point, const QPointF& value)
{
    auto* bar = statusBar();
    QString text = tr("Point [") + QString::number(point) + tr("]: ");
    text += "(" + QString::number(value.x()) + "; " + QString::number(value.y()) + ")";
    bar->showMessage(text);
}
