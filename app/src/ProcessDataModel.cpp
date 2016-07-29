#include "ProcessDataModel.h"

bool comparator(const QPair<QString, float>& e1, const QPair<QString, float>& e2)
{
    return (e1.second < e2.second);
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

void ProcessDataModel::logFileChanged()
{
    layoutAboutToBeChanged();

    qWarning("LogFile changed");
    if (m_logFile == 0) return;

    m_items.clear();

    /*QMap<QString, int> oldTime;

    QList<LogFile::LogRecord> records = m_logFile->records();
    foreach(const LogFile::LogRecord &r, records)
    {
        foreach (const LogFile::ProcessRecord &rp, r.proc)
        {
            //
        }

        //m_items.append(item);
    }*/

    layoutChanged();
}

void ProcessDataModel::sort()
{
    qSort(m_items.begin(), m_items.end(), comparator);
}

