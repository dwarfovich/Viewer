#ifndef HEADER_HPP
#define HEADER_HPP

#include <QString>
#include <QDateTime>

class QTextStream;

struct Header
{
public:
    Header();

    friend QTextStream &operator>> (QTextStream& is, Header& header);

    QString organization_;
    QString application_;
    QString measurement_type_;
    QDateTime start_time_;
    std::vector<QString> parameters_;
};

#endif // HEADER_HPP
