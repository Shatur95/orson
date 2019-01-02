#include "packagestreeview.h"

#include <QHeaderView>

PackagesTreeView::PackagesTreeView(QWidget *parent) :
    QTreeView(parent)
{
    // Setup items
    setModel(m_model);
    sortByColumn(-1, Qt::AscendingOrder); // Show item unsorted by default
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // Add current package changed signal
    connect(selectionModel(), &QItemSelectionModel::currentChanged, [&](const QModelIndex &current) {
        auto package = static_cast<Package *>(current.internalPointer());
        emit currentPackageChanged(package);
    });
}

void PackagesTreeView::setPackageHidden(Package *package, bool hide)
{
    setRowHidden(package->index(), QModelIndex(), hide);
}

void PackagesTreeView::showAllPackages()
{
    for (int i = 0; i < m_model->packages().count(); ++i)
        setRowHidden(i, QModelIndex(), false);
}

void PackagesTreeView::scrollToPackage(Package *package)
{
    const QModelIndex index = m_model->index(package->index(), 0, QModelIndex());
    scrollTo(index);
}

PackagesModel *PackagesTreeView::model() const
{
    return m_model;
}

Package *PackagesTreeView::currentPackage() const
{
    return static_cast<Package *>(currentIndex().internalPointer());
}

void PackagesTreeView::setCurrentPackage(Package *package)
{
    const QModelIndex index = m_model->index(package->index(), 0, QModelIndex());
    setCurrentIndex(index);
}
