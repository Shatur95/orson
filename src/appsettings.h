﻿#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include "pacman.h"
#include "autosynctimer.h"
#include "packages-view/packagesmodel.h"

#include <QSettings>
#include <QLocale>
#include <QNetworkProxy>
#include <QTranslator>

class AppSettings : public QSettings
{
    Q_OBJECT
    Q_DISABLE_COPY(AppSettings)

public:
    AppSettings(QObject *parent = nullptr);

    // Main window
    bool isNoConfirm() const;
    void setNoConfirm(bool noConfirm);

    bool isForce() const;
    void setForce(bool force);

    Pacman::AfterCompletion afterCompletion() const;
    void setAfterCompletion(Pacman::AfterCompletion afterCompletion);

    bool isInstantSearchEnabled() const;
    void setInstantSearchEnabled(bool enabled);
    static constexpr bool defaultIsInstantSearchEnabled()
    { return true; }

    // General settings
    void setupLocale();
    QLocale::Language locale() const;
    void setLocale(QLocale::Language lang);
    void loadLocale(QLocale::Language lang);

    bool isMinimizeToTray() const;
    void setMinimizeToTray(bool visible);

    bool isStartMinimized() const;
    void setStartMinimized(bool minimized);

    bool isAutostartEnabled() const;
    void setAutostartEnabled(bool enabled);

    // Pacman settings
    QString terminal() const;
    QStringList availableTerminals() const;
    void setTerminal(const QString &terminal);

    QStringList terminalArguments(const QString &terminal) const;
    QStringList defaultArguments(const QString &terminal) const;
    void setTerminalArguments(const QString &terminal, const QStringList &arguments);

    QString pacmanTool() const;
    QStringList availablePacmanTools() const;
    void setPacmanTool(const QString &programName);
    static QString defaultPacmanTool();

    AutosyncTimer::AutosyncType autosyncType() const;
    void setAutosyncType(AutosyncTimer::AutosyncType type);
    static constexpr AutosyncTimer::AutosyncType defaultAutosyncType()
    { return AutosyncTimer::Interval; }

    int autosyncInterval() const;
    void setAutosyncInterval(int hours);
    static constexpr int defaultAutosyncInterval()
    { return 3; }

    QTime autosyncTime() const;
    void setAutosyncTime(const QTime &time);
    static QTime defaultAutosyncTime();

    QDateTime lastSync() const;
    void setLastSync(const QDateTime& dateTime);

    // Interface settings
    QString statusIconName(PackagesModel::DatabaseStatus status) const;
    void setStatusIconName(PackagesModel::DatabaseStatus status, const QString &name);
    static QString defaultStatusIconName(PackagesModel::DatabaseStatus status);

    // Connection settings
    QNetworkProxy::ProxyType proxyType() const;
    void setProxyType(QNetworkProxy::ProxyType type);

    QString proxyHost() const;
    void setProxyHost(const QString &hostName);

    quint16 proxyPort() const;
    void setProxyPort(quint16 port);

    bool isProxyAuthEnabled() const;
    void setProxyAuthEnabled(bool enabled);

    QString proxyUsername() const;
    void setProxyUsername(const QString &username);

    QString proxyPassword() const;
    void setProxyPassword(const QString &password);

    // Shortcuts
    QString changeModeShortcut() const;
    void setChangeModeShortcut(const QString &shortcut);
    static QString defaultChangeModeShortcut();

    QString searchPackagesShortcut() const;
    void setSearchPackagesShortcut(const QString &shortcut);
    static QString defaultSearchPackagesShortcut();

    // Main window settings
    QByteArray mainWindowGeometry() const;
    void setMainWindowGeometry(const QByteArray &geometry);

private:
    static QTranslator m_appTranslator;
};

#endif // APPSETTINGS_H
