#ifndef LOGFILE_H
#define LOGFILE_H

#include <QObject>
#include <QList>

class LogFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)

public:
    struct ProcessRecord {
        QString name;
        int pid;
        int time;
    };

    struct LogRecord {
        int time;
        float capacity;
        bool isCharging;
        QList<ProcessRecord> proc;
    };

public:
    LogFile();

    QString fileName();
    void setFileName(const QString &fileName);

    QList<LogRecord> records();

signals:
    void fileNameChanged();
    void updated();

public slots:
    void update();

private:
    QString     m_fileName;
    QList<LogRecord> m_items;
};

#endif // LOGFILE_H
