#include "file_reader.hpp"
#include "data.hpp"
#include "header.hpp"

#include <QTextStream>
#include <QFile>
#include <QMessageBox>

#include <QDebug>
#define DEB qDebug()

FileReader::FileReader(Data &data)
    : data_{data}
{}

void FileReader::readFile(const QString &filename)
{
    QFile file {filename};

    if (file.open(QFile::ReadOnly)) {
        clearData();
        QTextStream input {&file};
        auto result = readHeader(input);
        readData(input);
        emit finished();
    } else {
        static const QString caption = QObject::tr("Critical error");
        static const QString text = QObject::tr("Cannot open file");
        QMessageBox::critical(nullptr, caption, text);
    }
}

bool FileReader::hasErrors() const
{
    return false;
}

bool FileReader::readHeader(QTextStream &input)
{
//    QTextStream input {&file};
    auto pos = input.pos();
    QChar starter;
    input >> starter;
    if (starter == header_line_starter_) {
        QString line = input.readLine();
        bool result = readOrganiationAndApp(line);
        if (!result) {
            return false;
        }
        line = input.readLine();
        result = readMeasurementType(line);
        if (!result) {
            return false;
        }
        line = input.readLine();
        result = readStartTime(line);
        if (!result) {
            return false;
        }
        result = readParameters(input);
    } else {
        input.seek(pos);
    }

//    printHeader();

//    return istream;
    return true;
}

bool FileReader::checkHeaderLineStarter(const QString &line) const
{
    return line.front() == header_line_starter_;
}

bool FileReader::readOrganiationAndApp(QString line)
{
    const auto& parts = line.split(",");
    const int expected_lines = 2;
    if (parts.size() == expected_lines) {
        data_.header_.organization_ = parts[OrganizationAndAppParts::OrganizationPart].simplified();
        data_.header_.application_ = parts[OrganizationAndAppParts::ApplicationPart].simplified();
        return true;
    } else {
        static const QString caption = QObject::tr("Warning");
        static const QString text = QObject::tr("Cannot parse organization and application. Proceed anyway?");
        auto reply = QMessageBox::warning(nullptr, caption, text, QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            data_.header_.organization_ = line.simplified();
            return true;
        } else {
            return false;
        }
    }
}

bool FileReader::readMeasurementType(QString line)
{
    bool result = checkHeaderLineStarter(line);
    line.remove(0, 1);
    data_.header_.measurement_type_ = line.simplified();

    return true;
}

bool FileReader::readStartTime(QString line)
{
    line.remove(0, 2);
    static const QString time_format = "ddd MMM dd hh:mm:ss yyyy";
    static const QLocale locale {"en_US"};
    data_.header_.start_time_ = locale.toDateTime(line, time_format);

    return true;
}

bool FileReader::readParameters(QTextStream &input)
{
    bool is_parameters_line = true;
    while (is_parameters_line) {
        auto pos = input.pos();
        QString line = input.readLine();
        if (checkHeaderLineStarter(line)) {
            line.remove(0, 1);
            data_.header_.parameters_.push_back(line.simplified());
        } else {
            input.seek(pos);
            is_parameters_line = false;
        }
    }

    return true;
}

void FileReader::readData(QTextStream &input)
{
    QTime timer;
    auto& data = data_.data_;
    timer.start();
    data.reserve(default_data_reserved_size());
    QString d = input.readAll();
    int first = 0;
    bool is_end = false;
    while (!is_end) {
        // first value
        int delimiter_pos = d.indexOf(' ', first);
        QStringRef first_value (&d, first, delimiter_pos - first);
        QPointF point;
        point.setY(first_value.toDouble());
        // second value
        int end_of_line = d.indexOf('\n', delimiter_pos + 1);
        QStringRef second_value (&d, delimiter_pos + 1, end_of_line - delimiter_pos - 1);
        point.setX(second_value.toDouble());
        data.push_back(point);
        first = end_of_line + 2;
        if (first >= d.size() - 1) {
            is_end = true;
        }
    }

    qreal secs = timer.elapsed() / qreal(1000);
    DEB << "Time: " << secs;
    DEB << data.size();
}

qint64 FileReader::estimateMeasurementsCount(QTextStream &input) const
{
    auto start = input.pos();
    return {};
}

void FileReader::clearData()
{
    data_.data_.clear();
    data_.header_.parameters_.clear();
}

void FileReader::printHeader() const
{
    const auto& header = data_.header_;
    DEB << "Organization:" << header.organization_;
    DEB << "Application:" << header.application_;
    DEB << "Measurement type:" << header.measurement_type_;
    DEB << "Start time:" << header.start_time_;
    for (const auto& str : header.parameters_) {
        DEB << str;
    }
}

size_t FileReader::default_data_reserved_size() const
{
    return default_data_reserved_size_;
}

void FileReader::setDefault_data_reserved_size(const size_t &default_data_reserved_size)
{
    default_data_reserved_size_ = default_data_reserved_size;
}
