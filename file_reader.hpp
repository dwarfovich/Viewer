#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include <measurement.hpp>

#include <QObject>
#include <QString>

struct Measurement;
class QFile;
class QTextStream;

class FileReader : public QObject
{
    Q_OBJECT

    enum HeaderLines {
        OrganizationAndApp = 0,
        MeasurementType,
        StartTime,
        Duration,
        Parameters
    };

    enum OrganizationAndAppParts {
        OrganizationPart = 0,
        ApplicationPart
    };

public:
    FileReader();

    void readFile(const QString& filename);
    bool hasErrors() const;

    size_t dataReservedSize() const;
    void setDataReservedSize(const size_t &size);
    Measurement takeMeasurement();
    const QStringList& headerErrors() const;
    const QStringList& dataErrors() const;

signals:
    void finished();

private:
    QStringList readHeaderLines(QTextStream& input) const;
    QStringList parseHeaderLines(const QStringList& lines);
    bool isHeaderLine(const QString& line) const;
    void parseOrganiationAndApp(QString line);
    void parseMeasurementType(QString line);
    void parseStartTime(QString line);
    void parseDuration(QString line);
    void parseParameters(const QStringList& header_lines, int first_parameters_line);
    void readData(QTextStream& input);
    void clearMeasurement();

    void printHeader() const;

private:
    Measurement measurement_;
    const QChar header_line_starter_ = '#';
    size_t data_reserved_size_ = 5'000'000;
    QStringList header_errors_;
    QStringList data_errors_;
};

#endif // FILEREADER_HPP
