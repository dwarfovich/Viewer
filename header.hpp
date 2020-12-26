#ifndef HEADER_HPP
#define HEADER_HPP

#include <QString>
#include <QDateTime>

class QTextStream;

struct Header
{
    QString organization;
    QString application;
    QString measurement_type;
    QDateTime start_time;
    QString duration;
    std::vector<QString> parameters;
};

#endif // HEADER_HPP
