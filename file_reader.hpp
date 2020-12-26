#ifndef FILEREADER_HPP
#define FILEREADER_HPP

#include "measurement.hpp"
#include "data_read_worker.hpp"

#include <QObject>
#include <QString>
#include <QThread>

#include <memory>

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

public:
    FileReader();
    ~FileReader();

    void readFile(const QString& filename);
    bool hasErrors() const;
    Measurement takeMeasurement();
    const QStringList& headerErrors() const;
    const QStringList& dataErrors() const;

signals:
    void progressChanged(qreal progress);
    void finished();
    void readingStarted(const QString& text);

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

private slots:
    void getWorkerResults();

private:
    QThread* worker_thread_;
    std::unique_ptr<DataReadWorker> worker_;
    Measurement measurement_;
    QStringList header_errors_;
    QStringList data_errors_;
    const QChar header_line_starter_ = '#';
};

#endif // FILEREADER_HPP
