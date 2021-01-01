#ifndef HEADER_HPP
#define HEADER_HPP

#include <QString>
#include <QDateTime>

struct Header
{
    QString filename;
    QString organization;
    QString application;
    QString measurement_type;
    QDateTime start_time;
    QString duration;
    std::vector<QString> parameters;
};

#endif // HEADER_HPP
