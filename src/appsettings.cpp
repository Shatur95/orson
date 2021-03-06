﻿#include "appsettings.h"
#include "singleapplication.h"

#include <QStandardPaths>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QTime>

QTranslator AppSettings::m_appTranslator;

AppSettings::AppSettings(QObject *parent) :
    QSettings(parent)
{
}

bool AppSettings::isNoConfirm() const
{
    return value("NoConfirm", true).toBool();
}

void AppSettings::setNoConfirm(bool noConfirm)
{
    setValue("NoConfirm", noConfirm);
}

bool AppSettings::isForce() const
{
    return value("Force", false).toBool();
}

void AppSettings::setForce(bool force)
{
    setValue("Force", force);
}

Pacman::AfterCompletion AppSettings::afterCompletion() const
{
    return value("AfterCompletion", Pacman::WaitForInput).value<Pacman::AfterCompletion>();
}

void AppSettings::setAfterCompletion(Pacman::AfterCompletion afterCompletion)
{
    setValue("AfterCompletion", afterCompletion);
}

bool AppSettings::isInstantSearchEnabled() const
{
    return value("InteractiveSearch", defaultIsInstantSearchEnabled()).toBool();
}

void AppSettings::setInstantSearchEnabled(bool enabled)
{
    setValue("InteractiveSearch", enabled);
}

void AppSettings::setupLocale()
{
    loadLocale(locale());
    SingleApplication::installTranslator(&m_appTranslator);
}

QLocale::Language AppSettings::locale() const
{
    return value("Locale", QLocale::AnyLanguage).value<QLocale::Language>();
}

void AppSettings::setLocale(QLocale::Language lang)
{
    if (lang != locale()) {
        setValue("Locale", lang);
        loadLocale(lang);
    }
}

void AppSettings::loadLocale(QLocale::Language lang)
{
    if (lang == QLocale::AnyLanguage)
        QLocale::setDefault(QLocale::system());
    else
        QLocale::setDefault(QLocale(lang));

    m_appTranslator.load(QLocale(), "orson", "_", ":/translations");
}

bool AppSettings::isMinimizeToTray() const
{
    return value("MinimizeToTray", true).toBool();
}

void AppSettings::setMinimizeToTray(bool visible)
{
    setValue("MinimizeToTray", visible);
}

bool AppSettings::isStartMinimized() const
{
    return value("StartMinimized", true).toBool();
}

void AppSettings::setStartMinimized(bool minimized)
{
    setValue("StartMinimized", minimized);
}

bool AppSettings::isAutostartEnabled() const
{
    return QFileInfo::exists(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart/orson.desktop");
}

void AppSettings::setAutostartEnabled(bool enabled)
{
    QFile autorunFile(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart/orson.desktop");

    if (enabled) {
        // Create autorun file
        if (!autorunFile.exists()) {
            const QString desktopFileName = QStringLiteral("/usr/share/applications/orson.desktop");

            if (!QFile::copy(desktopFileName, autorunFile.fileName()))
                qCritical() << tr("Unable to create autorun file from ") + desktopFileName;
        }
    } else {
        // Remove autorun file
        if(autorunFile.exists())
            autorunFile.remove();
    }
}

QString AppSettings::terminal() const
{
    return value("Terminal", availableTerminals().first()).toString();
}

QStringList AppSettings::availableTerminals() const
{
    QStringList terminals;
    if (QFileInfo::exists("/usr/bin/konsole"))
        terminals.append("konsole");
    if (QFileInfo::exists("/usr/bin/gnome-terminal"))
        terminals.append("gnome-terminal");
    if (QFileInfo::exists("/usr/bin/mate-terminal"))
        terminals.append("mate-terminal");
    if (QFileInfo::exists("/usr/bin/xfce4-terminal"))
        terminals.append("xfce4-terminal");
    if (QFileInfo::exists("/usr/bin/lxterminal"))
        terminals.append("lxterminal");
    if (QFileInfo::exists("/usr/bin/deepin-terminal"))
        terminals.append("deepin-terminal");
    if (QFileInfo::exists("/usr/bin/pantheon-terminal"))
        terminals.append("pantheon-terminal");
    if (QFileInfo::exists("/usr/bin/alacritty"))
        terminals.append("alacritty");
    if (QFileInfo::exists("/usr/bin/cool-retro-term"))
        terminals.append("cool-retro-term");
    if (QFileInfo::exists("/usr/bin/kitty"))
        terminals.append("kitty");
    if (QFileInfo::exists("/usr/bin/qterminal"))
        terminals.append("qterminal");
    if (QFileInfo::exists("/usr/bin/sakura"))
        terminals.append("sakura");
    if (QFileInfo::exists("/usr/bin/terminator"))
        terminals.append("terminator");
    if (QFileInfo::exists("/usr/bin/termite"))
        terminals.append("termite");
    if (QFileInfo::exists("/usr/bin/tilix"))
        terminals.append("tilix");
    if (QFileInfo::exists("/usr/bin/xterm"))
        terminals.append("xterm");

    if (terminals.isEmpty())
        terminals.append(QString());

    return terminals;
}

void AppSettings::setTerminal(const QString &terminal)
{
    setValue("Terminal", terminal);
}

QStringList AppSettings::terminalArguments(const QString &terminal) const
{
    return value(terminal, defaultArguments(terminal)).toStringList();
}

QStringList AppSettings::defaultArguments(const QString &terminal) const
{
    if (terminal == "konsole")
        return { "--hide-menubar", "--hide-tabbar", "-e" };

    if (terminal == "gnome-terminal")
        return { "--hide-menubar", "--" };

    if (terminal == "mate-terminal" || terminal == "xfce4-terminal")
        return { "--hide-menubar", "-x" };

    return { "-e" };
}

void AppSettings::setTerminalArguments(const QString &terminal, const QStringList &arguments)
{
    setValue(terminal, arguments);
}

QString AppSettings::pacmanTool() const
{
    return value("PacmanTool", defaultPacmanTool()).toString();
}

QStringList AppSettings::availablePacmanTools() const
{
    QStringList pacmanTools = { defaultPacmanTool() };

    if (QFileInfo::exists("/usr/bin/aura"))
        pacmanTools.append("aura");
    if (QFileInfo::exists("/usr/bin/pacaur"))
        pacmanTools.append("pacaur");
    if (QFileInfo::exists("/usr/bin/packer"))
       pacmanTools.append("packer");
    if (QFileInfo::exists("/usr/bin/pakku"))
        pacmanTools.append("pakku");
    if (QFileInfo::exists("/usr/bin/pikaur"))
        pacmanTools.append("pikaur");
    if (QFileInfo::exists("/usr/bin/trizen"))
        pacmanTools.append("trizen");
    if (QFileInfo::exists("/usr/bin/wrapaur"))
        pacmanTools.append("wrapaur");
    if (QFileInfo::exists("/usr/bin/yay"))
        pacmanTools.append("yay");


    return pacmanTools;
}

void AppSettings::setPacmanTool(const QString &programName)
{
    setValue("PacmanTool", programName);
}

QString AppSettings::defaultPacmanTool()
{
    return QStringLiteral("sudo pacman");
}

AutosyncTimer::AutosyncType AppSettings::autosyncType() const
{
    return value("AutosyncType", defaultAutosyncType()).value<AutosyncTimer::AutosyncType>();
}

void AppSettings::setAutosyncType(AutosyncTimer::AutosyncType type)
{
    setValue("AutosyncType", type);
}

int AppSettings::autosyncInterval() const
{
    return value("AutosyncInterval", defaultAutosyncInterval()).toInt();
}

void AppSettings::setAutosyncInterval(int hours)
{
    setValue("AutosyncInterval", hours);
}

QTime AppSettings::autosyncTime() const
{
    return value("AutosyncTime", defaultAutosyncTime()).toTime();
}

void AppSettings::setAutosyncTime(const QTime &time)
{
    setValue("AutosyncTime", time);
}

QTime AppSettings::defaultAutosyncTime()
{
    return QTime(12, 0);
}

QDateTime AppSettings::lastSync() const
{
    return value("LastSync", QDateTime()).toDateTime();
}

void AppSettings::setLastSync(const QDateTime& dateTime)
{
    setValue("LastSync", dateTime);
}

QString AppSettings::statusIconName(PackagesModel::DatabaseStatus status) const
{
    switch (status) {
    case PackagesModel::NoUpdates:
        return value("NoUpdatesIcon", defaultStatusIconName(status)).toString();
    case PackagesModel::Loading:
        return value("LoadingIcon", defaultStatusIconName(status)).toString();
    case PackagesModel::UpdatesAvailable:
        return value("UpdatesAvailableIcon", defaultStatusIconName(status)).toString();
    }

    return QString();
}

void AppSettings::setStatusIconName(PackagesModel::DatabaseStatus status, const QString &name)
{
    switch (status) {
    case PackagesModel::NoUpdates:
        setValue("NoUpdatesIcon", name);
        break;
    case PackagesModel::Loading:
        setValue("LoadingIcon", name);
        break;
    case PackagesModel::UpdatesAvailable:
        setValue("UpdatesAvailableIcon", name);
        break;
    }
}

QString AppSettings::defaultStatusIconName(PackagesModel::DatabaseStatus status)
{
    switch (status) {
    case PackagesModel::NoUpdates:
        return QStringLiteral("update-none");
    case PackagesModel::Loading:
        return QStringLiteral("state-sync");
    case PackagesModel::UpdatesAvailable:
        return QStringLiteral("update-high");
    }

    return QString();
}

QNetworkProxy::ProxyType AppSettings::proxyType() const
{
    return static_cast<QNetworkProxy::ProxyType>(value("Connection/ProxyType", QNetworkProxy::DefaultProxy).toInt());
}

void AppSettings::setProxyType(QNetworkProxy::ProxyType type)
{
    setValue("Connection/ProxyType", type);
}

QString AppSettings::proxyHost() const
{
    return value("Connection/ProxyHost").toString();
}

void AppSettings::setProxyHost(const QString &hostName)
{
    setValue("Connection/ProxyHost", hostName);
}

quint16 AppSettings::proxyPort() const
{
    return value("Connection/ProxyPort", 8080).value<quint16>();
}

void AppSettings::setProxyPort(quint16 port)
{
    setValue("Connection/ProxyPort", port);
}

bool AppSettings::isProxyAuthEnabled() const
{
    return value("Connection/ProxyAuthEnabled", false).toBool();
}

void AppSettings::setProxyAuthEnabled(bool enabled)
{
    setValue("Connection/ProxyAuthEnabled", enabled);
}

QString AppSettings::proxyUsername() const
{
    return value("Connection/ProxyUsername").toString();
}

void AppSettings::setProxyUsername(const QString &username)
{
    setValue("Connection/ProxyUsername", username);
}

QString AppSettings::proxyPassword() const
{
    return value("Connection/ProxyPassword").toString();
}

void AppSettings::setProxyPassword(const QString &password)
{
    setValue("Connection/ProxyPassword", password);
}

QString AppSettings::changeModeShortcut() const
{
    return value("Shortcuts/ChangeMode", defaultChangeModeShortcut()).toString();
}

void AppSettings::setChangeModeShortcut(const QString &shortcut)
{
    setValue("Shortcuts/ChangeMode", shortcut);
}

QString AppSettings::defaultChangeModeShortcut()
{
    return QStringLiteral("Ctrl+M");
}

QString AppSettings::searchPackagesShortcut() const
{
    return value("Shortcuts/SearchPackages", defaultSearchPackagesShortcut()).toString();
}

void AppSettings::setSearchPackagesShortcut(const QString &shortcut)
{
    setValue("Shortcuts/SearchPackages", shortcut);
}

QString AppSettings::defaultSearchPackagesShortcut()
{
    return QStringLiteral("Ctrl+F");
}

QByteArray AppSettings::mainWindowGeometry() const
{
    return value("WindowGeometry").toByteArray();
}

void AppSettings::setMainWindowGeometry(const QByteArray &geometry)
{
    setValue("WindowGeometry", geometry);
}
