#ifndef LOGFILE_H
#define LOGFILE_H

#include <QObject>

class LogFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
public:
    LogFile();

    QString fileName();
    void setFileName(const QString &fileName);

signals:
    void fileNameChanged();

public slots:
    void update();

private:
    QString m_fileName;
};

#endif // LOGFILE_H
