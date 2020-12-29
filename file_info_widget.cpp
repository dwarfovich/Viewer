#include "file_info_widget.hpp"
#include "ui_file_info_widget.h"
#include "measurement.hpp"
#include "header.hpp"

#include <QVBoxLayout>
#include <QLineEdit>

FileInfoWidget::FileInfoWidget(QWidget *parent, Qt::WindowFlags flags)
    : QWidget{parent, flags}
    , ui_{new Ui::FileInfoWidget}
    , groupBoxLayout_{new QVBoxLayout {}}
{
    ui_->setupUi(this);
    ui_->groupBox->setLayout(groupBoxLayout_);
}

FileInfoWidget::~FileInfoWidget()
{
    delete ui_;
}

void FileInfoWidget::setMeasurement(const Measurement* measurement)
{
    measurement_ = measurement;
    updateInfo();
}

void FileInfoWidget::updateInfo()
{
    if (!measurement_) {
        return;
    }

    ui_->lineEditFilename->setText(measurement_->header.filename);
    ui_->lineEditOrganization->setText(measurement_->header.organization);
    ui_->lineEditApplication->setText(measurement_->header.application);
    ui_->lineEditMeasurementType->setText(measurement_->header.measurement_type);
    ui_->lineEditStartTime->setText(measurement_->header.start_time.toString());

    for (auto parameter : parameters_) {
        delete parameter;
    }
    parameters_.clear();

    for (const auto& parameter : measurement_->header.parameters) {
        auto* parameterLine = new QLineEdit {};
        parameterLine->setText(parameter);
        parameterLine->setReadOnly(true);
        groupBoxLayout_->addWidget(parameterLine);
        parameters_.push_back(parameterLine);
    }
}
