#include "main_window.hpp"
#include "ui_main_window.h"
#include "measurement.hpp"
#include "file_reader.hpp"
#include "plot_drawer.hpp"

#include <QGraphicsScene>
#include <QAction>
#include <QFileDialog>
#include <QGraphicsPixmapItem>

#include <QDebug>
#define DEB qDebug()

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
    , ui_{new Ui::MainWindow}
    , scene_{new QGraphicsScene {this}}
{
    ui_->setupUi(this);
    ui_->mainGraphicsView->setScene(scene_);
    ui_->overallGraphicsView->setScene(scene_);
    ui_->overallGraphicsView->fitInView(scene_->sceneRect());
    ui_->splitter->setSizes({10, 200});

    connect(ui_->actionLoad, &QAction::triggered, this, &MainWindow::loadFile);
    connect(ui_->actionQuit, &QAction::triggered, this, &MainWindow::close);

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
    populateScene();
}

void MainWindow::populateScene()
{
    scene_->clear();
    PlotDrawer drawer {measurement_, {0., 0., 1920., 1080.}};
    drawer.draw();
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem {drawer.plot()};
    scene_->addItem(item);
//    QRect
    ui_->overallGraphicsView->fitInView(scene_->sceneRect());
}

