#include "LogFile.h"
#include <QFile>
#include <QLockFile>
#include <QJsonDocument>

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

void LogFile::update()
{
    if (m_fileName.isEmpty()) return;

    QLockFile lock(m_fileName);
    lock.lock();

    QFile f(m_fileName);
    if (f.open(QFile::ReadOnly))
    {
        QByteArray data = f.readAll();
        QString s = "[" + QString(data) + "]";

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(s.toLatin1(), &err);
        if (err.error != QJsonParseError::NoError)
        {
            qWarning("Parse error: %s", qPrintable(err.errorString()));
            return;
        }
    }
}

