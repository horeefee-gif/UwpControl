#include "uwpremove.h"
#include <QRegularExpression>

UwpRemove::UwpRemove(QObject* parent) : QAbstractListModel(parent) {
    m_process = new QProcess(this);

    connect(m_process, &QProcess::finished, this, &UwpRemove::onProcessFinished);

    runScan();
}

int UwpRemove::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return m_apps.count();
}

QVariant UwpRemove::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_apps.count()) return QVariant();

    const UwpAppItem &item = m_apps[index.row()];
    if (role == NameRole) return item.name;
    if (role == CheckedRole) return item.isChecked;

    return QVariant();
}

bool UwpRemove::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || index.row() >= m_apps.count()) return false;

    if (role == CheckedRole) {
        m_apps[index.row()].isChecked = value.toBool();
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

QHash<int, QByteArray> UwpRemove::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "appName";
    roles[CheckedRole] = "appChecked";
    return roles;
}

void UwpRemove::deleteSelectedApps() {
    if (m_process->state() != QProcess::NotRunning) {
        return;
    }

    m_deleteQueue.clear();

    for (const UwpAppItem& app : m_apps) {
        if (app.isChecked) {
            m_deleteQueue.enqueue(app.name);
        }
    }

    if (m_deleteQueue.isEmpty()) {
        return;
    }

    startNextDelete();
}

void UwpRemove::runScan() {
    QProcess process;
    QString command = "powershell";
    QStringList arguments;
    arguments << "-NoProfile" << "-Command" << "[Console]::OutputEncoding = [System.Text.Encoding]::Unicode; Get-AppxPackage -AllUsers | Select-Object -ExpandProperty Name";

    process.start(command, arguments);

    if (!process.waitForFinished(30000)) return;

    QByteArray rawData = process.readAllStandardOutput();

    QString output = QString::fromUtf16(reinterpret_cast<const char16_t*>(rawData.constData()), rawData.size() / 2);

    if (!output.isEmpty() && output.at(0) == QChar(0xFEFF)) {
        output.remove(0, 1);
    }

    QStringList uwpApps = output.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);

    beginResetModel();
    m_apps.clear();

    for (const QString& app : uwpApps) {
        QString cleanApp = app.trimmed();
        if (cleanApp.isEmpty()) continue;

        bool isWhiteListed = false;

        for (const QString& whiteItem : whiteList) {
            if (cleanApp.contains(whiteItem, Qt::CaseInsensitive)) {
                isWhiteListed = true;
                break;
            }
        }
        if (!isWhiteListed) continue;

        bool exists = false;
        for (const UwpAppItem& item : m_apps) {
            if (item.name == cleanApp) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            m_apps.append({cleanApp, false});
        }
    }
    endResetModel();
}

void UwpRemove::startNextDelete() {
    if (m_deleteQueue.isEmpty()) {
        runScan();
        return;
    }

    QString appName = m_deleteQueue.dequeue();

    QString command = "powershell";
    QStringList arguments;

    arguments << "-NoProfile" << "-Command" << QString("Get-AppxPackage -Name '%1' -AllUsers | Remove-AppxPackage -AllUsers").arg(appName);

    m_process->start(command, arguments);
}

void UwpRemove::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);

    QByteArray errors = m_process->readAllStandardError();

    if (!errors.isEmpty()) {
        qDebug() << errors;
    }

    startNextDelete();
}

void UwpRemove::selectAll() {
    if (m_apps.isEmpty()) {
        return;
    }

    for (UwpAppItem& app : m_apps) {
        app.isChecked = true;
    }

    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(m_apps.count() - 1, 0);
    emit dataChanged(topLeft, bottomRight, {CheckedRole});
}
