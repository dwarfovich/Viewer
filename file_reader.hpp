#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include <QObject>
#include <QString>

struct Measurement;
class QFile;
class QTextStream;

class FileReader : public QObject
{
    Q_OBJECT

    enum OrganizationAndAppParts {
        OrganizationPart = 0,
        ApplicationPart
    };

public:
    FileReader(Measurement& data);

    void readFile(const QString& filename);
    bool hasErrors() const;

    size_t default_data_reserved_size() const;
    void setDefault_data_reserved_size(const size_t &default_data_reserved_size);

signals:
    void finished();

private:
    bool readHeader(QTextStream &input);
    bool checkHeaderLineStarter(const QString& line) const;
    bool readOrganiationAndApp(QString line);
    bool readMeasurementType(QString line);
    bool readStartTime(QString line);
    bool readParameters(QTextStream& input);
    void readData(QTextStream& input);
    qint64 estimateMeasurementsCount(QTextStream& input) const;
    void clearData();

    void printHeader() const;

private:
    Measurement& data_;
    const QChar header_line_starter_ = '#';
    size_t default_data_reserved_size_ = 5'000'000;
};

#endif // FILEREADER_HPP
