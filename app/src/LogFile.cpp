#include "LogFile.h"
#include <QFile>
#include <QLockFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

LogFile::LogFile()
{

}

QString LogFile::fileName()
{
    return m_fileName;
}

void LogFile::setFileName(const QString &fileName)
{
    m_fileName = fileName;
    fileNameChanged();
}

QList<LogFile::LogRecord> LogFile::records()
{
    return m_items;
}

void LogFile::update()
{
    if (m_fileName.isEmpty()) return;

    //QLockFile lock(m_fileName);
    //lock.lock();

    m_items.clear();

    QFile f(m_fileName);
    if (f.open(QFile::ReadOnly))
    {
        QByteArray data = f.readAll();
        QString s = QString::fromUtf8(data);
        if (s.endsWith(",\n")) s = s.mid(0, s.length()-2);
        //s.replace("\n", "");
        //s = "{\"data\": [" + s + "]}";
        s = "[" + s + "]";

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(s.toLatin1(), &err);
        if (err.error != QJsonParseError::NoError)
        {
            qWarning("Parse error: %s at %d", qPrintable(err.errorString()), err.offset);
            return;
        }

        QJsonArray a = doc.array();
        foreach (QJsonValue v, a)
        {
            QJsonObject o = v.toObject();
            LogRecord r;
            r.capacity = o["capacity"].toDouble();
            r.isCharging = o["isCharging"].toBool();
            QJsonArray proc = o["proc"].toArray();
            foreach (QJsonValue v2, proc)
            {
                QJsonArray a2 = v2.toArray();
                ProcessRecord pr;
                pr.name = a2[0].toString();
                pr.pid = a2[1].toInt();
                pr.time = a2[2].toInt();
                r.proc.append(pr);
            }
            m_items.append(r);
        }
    }
    emit updated();
}

