#include "ProcessDataModel.h"
#include <QMap>
#include <QSet>

bool comparator(const QPair<QString, float>& e1, const QPair<QString, float>& e2)
{
    return (e1.second > e2.second);
}

ProcessDataModel::ProcessDataModel()
{
    m_items.append(qMakePair(QString("localhost"), 0.7));
    m_items.append(qMakePair(QString("windowsTrueApp"), 0.1));
    m_items.append(qMakePair(QString("dont_kill_me_please"), 1));
    sort();
    m_logFile = 0;
}

QHash<int, QByteArray> ProcessDataModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "name";
    roles[Qt::UserRole] = "load";
    return roles;

}

int ProcessDataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.size();
}

QVariant ProcessDataModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= m_items.size()) return QVariant();

    if (role == Qt::DisplayRole)
    {
        return m_items[row].first;
    }
    if (role == Qt::UserRole)
    {
        return m_items[row].second;
    }
    return QVariant();
}

void ProcessDataModel::setLogFile(LogFile *logFile)
{
    qWarning("setLogFile");
    m_logFile = logFile;
    connect(m_logFile, SIGNAL(updated()), this, SLOT(logFileChanged()));
}

typedef QPair<QString, int> ProcessId;

void ProcessDataModel::logFileChanged()
{
    layoutAboutToBeChanged();

    qWarning("LogFile changed");
    if (m_logFile == 0) return;

    m_items.clear();

    QMap<QString, int> oldTime;
    QMap<ProcessId, int> lastTime;
    QSet<ProcessId> lastPids;

    QList<LogFile::LogRecord> records = m_logFile->records();
    foreach(const LogFile::LogRecord &r, records)
    {
        QSet<ProcessId> newPids;
        QMap<ProcessId, int> newTime;
        foreach (const LogFile::ProcessRecord &rp, r.proc)
        {
            ProcessId id = qMakePair(rp.name, rp.pid);
            newPids.insert(id);
            newTime[id] = rp.time;
        }

        // Check for gone processes
        foreach (const ProcessId &id, lastPids)
        {
            if (!newPids.contains(id))
            {
                oldTime[id.first] = oldTime.value(id.first, 0) + lastTime[id];
            }
        }

        lastPids = newPids;
        lastTime = newTime;
    }

    foreach (const ProcessId &id, lastPids)
    {
        oldTime[id.first] = oldTime.value(id.first, 0) + lastTime[id];
    }

    float totalTime = 0;
    foreach (const QString &name, oldTime.keys())
    {
        totalTime += oldTime[name];
    }
    foreach (const QString &name, oldTime.keys())
    {
        QPair<QString, float> item = qMakePair(name, oldTime[name] / totalTime);
        m_items.append(item);
    }

    sort();

    layoutChanged();
}

void ProcessDataModel::sort()
{
    qSort(m_items.begin(), m_items.end(), comparator);
}

