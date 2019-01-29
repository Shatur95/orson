#include "terminal.h"

#include <QFileInfo>
#include <QDebug>

constexpr char waitForInput[] = " && echo && read -s -p 'Successfully! To close this window, press <Enter>...'";

Terminal::Terminal()
{
    m_terminal.setProcessChannelMode(QProcess::MergedChannels);
    m_terminal.setProgram("konsole");

    // Add data available signal
    connect(&m_terminal, &QProcess::readyReadStandardOutput, [&] {
        emit dataAvailable(m_terminal.readAllStandardOutput());
    });

    // Add finished signal
    connect(&m_terminal, qOverload<int>(&QProcess::finished), this, &Terminal::finished);
}

void Terminal::setTasks(TasksModel *model)
{
    m_commands.clear();
    constexpr char pacmanProgram[] = "pikaur ";

    foreach (Task *category, model->categories()) {
        if (category->children().isEmpty())
            continue;

        // Concatenate categories
        if (!m_commands.isEmpty())
            m_commands.append(" && ");

        // Pacman command
        m_commands.append(pacmanProgram);
        switch (category->categoryType()) {
        case Task::InstallExplicity:
        case Task::InstallAsDepend:
        case Task::Reinstall:
            m_commands.append("-S ");
            break;
        case Task::MarkAsDepend:
        case Task::MarkAsExplicity:
            m_commands.append("-D ");
            break;
        case Task::Uninstall:
            m_commands.append("-R ");
            break;
        default:
            qFatal("Unknown category");
        }

        // Add packages list
        foreach (Task *task, category->children())
            m_commands.append(task->name() + " ");

        // Reason
        switch (category->categoryType()) {
        case Task::InstallAsDepend:
        case Task::MarkAsDepend:
            m_commands.append("--asdeps ");
            break;
        case Task::MarkAsExplicity:
            m_commands.append("--asexplicit ");
            break;
        default:
            break;
        }

        if (m_noConfirm)
            m_commands.append("--noconfirm");
    }
}

void Terminal::executeTasks()
{
    auto [terminal, terminalArguments] = getTerminalProgram();
    m_terminal.setProgram(terminal);
    m_terminal.setArguments(terminalArguments << m_commands + waitForInput);
    m_terminal.start();
}

void Terminal::updateDatabase()
{
    const QString command = QStringLiteral("sudo pacman -Sy");

    auto [terminal, terminalArguments] = getTerminalProgram();
    m_terminal.setProgram(terminal);
    m_terminal.setArguments(terminalArguments << command + waitForInput);
    m_terminal.start();
}

QPair<QString, QStringList> Terminal::getTerminalProgram()
{
    QPair<QString, QStringList> program;
    if (QFileInfo::exists("/usr/bin/konsole"))
        program = {"konsole", {"--hide-menubar", "--hide-tabbar", "-e"}};
    else if (QFileInfo::exists("/usr/bin/gnome-terminal"))
        program = {"gnome-terminal", {"--hide-menubar", "--"}};
    else
        qFatal("Unable to found terminal program");

    // Add shell execution
    program.second << "$SHELL" << "-c";
    return program;
}

bool Terminal::isNoConfirm() const
{
    return m_noConfirm;
}

void Terminal::setNoConfirm(bool noconfirm)
{
    m_noConfirm = noconfirm;
}

QString Terminal::commands() const
{
    return m_commands;
}
