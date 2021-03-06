#include "file.h"

#include <QMimeDatabase>
#include <QDir>

File::File()
{
    m_nameColumn = QStringLiteral("Name");
    m_sizeColumn = QStringLiteral("Size");
    m_typeColumn = QStringLiteral("Type");
    m_info.setFile(QDir::rootPath());
}

File::File(const QString &path, File *parent) :
    m_info(path)
{
    m_nameColumn = m_info.fileName();
    if (!m_info.exists() && parent->isReadable()) {
        m_icon = QIcon::fromTheme("dialog-error");
        m_typeColumn = QStringLiteral("Missing");
        m_missing = true;
    } else if (m_info.isFile()) {
        const QMimeDatabase mimeDatabase;
        const QMimeType type = mimeDatabase.mimeTypeForFile(m_info);

        if (m_info.isReadable())
            m_icon = QIcon::fromTheme(type.iconName());
        else
            m_icon = QIcon::fromTheme("lock");

        m_sizeColumn = QString::number(m_info.size());
        m_typeColumn = type.name();
    } else if (m_info.isDir()) {
        if (m_info.isReadable())
            m_icon = QIcon::fromTheme("folder");
        else
            m_icon = QIcon::fromTheme("lock");

        m_typeColumn = QStringLiteral("Folder");
    } else {
        // No access to read any information
        m_typeColumn = QStringLiteral("No access");
        m_icon = QIcon::fromTheme("lock");
    }

    parent->addChild(this);
}

File::~File()
{
    qDeleteAll(m_children);
}

int File::row() const
{
    if (m_parent != nullptr)
        return m_parent->m_children.indexOf(const_cast<File *>(this));

    return 0;
}

QString File::text(int column) const
{
    switch (column) {
    case 0:
        return m_nameColumn;
    case 1:
        return m_sizeColumn;
    case 2:
        return m_typeColumn;
    default:
        qFatal("Unknown column");
    }
}

File *File::parent() const
{
    return m_parent;
}

void File::addChild(File *child)
{
    child->m_parent = this;
    m_children.append(child);
}

void File::removeChildren()
{
    qDeleteAll(m_children);
    m_children.clear();
}

QString File::name() const
{
    return m_nameColumn;
}

QIcon File::icon() const
{
    return m_icon;
}

QString File::path() const
{
    return m_info.filePath();
}

bool File::isFile() const
{
    return m_info.isFile();
}

bool File::isReadable() const
{
    return m_info.isReadable();
}

bool File::isMissing() const
{
    return m_missing;
}

QVector<File *> File::children() const
{
    return m_children;
}
