﻿#ifndef PACMAN_H
#define PACMAN_H

#include <QObject>
#include <QString>

class QProcess;
class PackagesView;
class Package;

class Pacman : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Pacman)

public:
    enum AfterCompletion {
        WaitForInput,
        Shutdown,
        Reboot
    };
    Q_ENUM(AfterCompletion)

    Pacman(QObject *parent = nullptr);

    // Actions
    void setTasks(PackagesView *view);
    QString tasksCommands();
    void executeTasks();

    void installLocalPackage(const QString &fileName, bool asDepend = false);
    void syncDatabase();

    // Parameters
    bool isNoConfirm() const;
    void setNoConfirm(bool noconfirm);

    bool isForce() const;
    void setForce(bool force);

    AfterCompletion afterTasksCompletion() const;
    void setAfterTasksCompletion(const AfterCompletion &afterTasksCompletion);

signals:
    void started();
    void finished(int exitCode);

private slots:
    void getExitCode();

private:
    void appendPackagesCommand(QString &tasksCommands, const QString &pacmanTool, const QVector<Package *> &packages, const QString &action, const QString &parameters = QString());
    static QString afterCompletionCommand(AfterCompletion afterCompletion);
    void exec(const QString &commands, AfterCompletion afterCompletion);

    PackagesView *m_tasksView = nullptr;
    QProcess *m_terminal;
    AfterCompletion m_afterTasksCompletion = WaitForInput;
    bool m_noConfirm = true;
    bool m_force = false;
    bool m_updateTimeOnSuccess = false;
};

#endif // PACMAN_H
