#ifndef UWPREMOVE_H
#define UWPREMOVE_H

#pragma once
#include <QAbstractListModel>
#include <QVariant>
#include <QDebug>
#include <QtQml>
#include <QProcess>
#include <QStringList>
#include <QQueue>

struct UwpAppItem {
    QString name;
    bool isChecked;
};

class UwpRemove : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
public:
    explicit UwpRemove(QObject* parent = nullptr);

    enum AppRoles {
        NameRole = Qt::UserRole + 1,
        CheckedRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void deleteSelectedApps();
    Q_INVOKABLE void selectAll();
    void runScan();
private slots:
    void startNextDelete();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
private:
    QStringList whiteList = {
        "Microsoft.BingNews", "Microsoft.BingWeather", "Microsoft.BingSports",
        "Microsoft.BingFinance", "Microsoft.GamingApp", "Microsoft.XboxSpeechToTextOverlay",
        "Microsoft.XboxIdentityProvider", "Microsoft.GetHelp", "Microsoft.Getstarted",
        "Microsoft.MicrosoftSolitaireCollection", "Microsoft.MixedReality.Portal", "Microsoft.Office.OneNote",
        "Microsoft.People", "Microsoft.SkypeApp", "Microsoft.MicrosoftStickyNotes",
        "Microsoft.YourPhone", "Microsoft.ZuneMusic", "Microsoft.ZuneVideo", "Microsoft.MicrosoftOfficeHub",
        "Microsoft.Microsoft365", "Microsoft.windowscommunicationsapps", "Microsoft.Todos",
        "Clipchamp.Clipchamp", "Microsoft.PowerAutomateDesktop", "Microsoft.WindowsFeedbackHub",
        "Microsoft.WindowsSoundRecorder", "Microsoft.549981C3F5F10", "Microsoft.DisneyPlus",
        "Microsoft.SpotifyMusic", "Microsoft.CandyCrushSaga", "Microsoft.CandyCrushSodaSaga",
        "Microsoft.KingCandyCrushBubbles", "Bytedance.TikTok", "4DF9EC11.Netflix",
        "King.com.BubbleWitch3Saga", "Microsoft.Bestaudio", "Microsoft.MSPaint",
        "Microsoft.ScreenSketch", "Microsoft.3DBuilder", "Microsoft.Print3D",
        "Microsoft.Microsoft3DViewer", "Microsoft.WindowsAlarms", "Microsoft.WindowsMaps",
        "Microsoft.OneConnect", "Microsoft.Wallet", "Microsoft.WindowsCamera"
    };

    QList<UwpAppItem> m_apps;

    QProcess* m_process;
    QQueue<QString> m_deleteQueue;
};

#endif // UWPREMOVE_H