#ifndef PACKAGE_H
#define PACKAGE_H

#include "alpm.h"

#include <QString>
#include <QDateTime>
#include <QUrl>

class Package
{
public:
    Package();
    void setSyncData(alpm_pkg_t *data);
    void setLocalData(alpm_pkg_t *data);

    QString name() const;
    QString repo() const;
    QString version() const;
    QString description() const;
    QString arch() const;
    QString url() const;
    QString packager() const;
    QStringList licenses() const;
    QStringList groups() const;
    QStringList files() const;
    QList<alpm_depend_t*> provides() const;
    QList<alpm_depend_t*> replaces() const;
    QList<alpm_depend_t*> conflicts() const;
    QList<alpm_depend_t*> depends() const;
    QList<alpm_depend_t*> optdepends() const;
    QList<alpm_depend_t*> checkdepends() const;
    QList<alpm_depend_t*> makedepends() const;
    QDateTime buildDate() const;
    QDateTime installDate() const;
    alpm_pkgreason_t reason() const;
    long downloadSize() const;
    long installedSize() const;
    bool hasScript() const;
    bool isInstalled() const;

    static QString depmodString(alpm_depmod_t mod);

private:
    alpm_pkg_t *m_syncData = nullptr;
    alpm_pkg_t *m_localData = nullptr;
    bool m_isInstalled = false;

};

#endif // PACKAGE_H
