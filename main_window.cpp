#include "main_window.hpp"
#include "ui_main_window.h"
#include "data.hpp"
#include "file_reader.hpp"

#include <QAction>
#include <QFileDialog>
#include <QTextStream>
//#include <QFuture>
//#include <QtConcurrent/QtConcurrent>

#include <QDebug>
#define DEB qDebug()

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);

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
    FileReader reader {data_};
    reader.readFile(filename);
}

