#ifndef PACKAGESMODEL_H
#define PACKAGESMODEL_H

#include <QAbstractItemModel>
#include <QNetworkAccessManager>
#include <QtConcurrent>

#include <alpm.h>

class Package;

class PackagesModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Mode {
        Repo,
        AUR
    };

    explicit PackagesModel(QObject *parent = nullptr);
    ~PackagesModel() override;

    // Model-specific functions
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    // Switch between AUR and ALPM
    Mode mode() const;
    void setMode(Mode mode);

    // Other
    QVector<Package *> packages() const;
    QVector<Package *> outdatedPackages() const;
    void reloadRepoPackages();
    void aurSearch(const QString &text, const QString &queryType);
    void loadMoreAurInfo(Package *package);

signals:
    void databaseStatusChanged(const QString &text);
    void databaseLoaded();
    void firstPackageAvailable();
    void packageChanged(Package *package);

private:
    void loadRepoPackages();
    int loadLocalDatabase();
    void loadSyncDatabase(const QString &databaseName);
    void loadAurDatabase();

    template<typename T1, typename T2>
    void sortPackages(QVector<Package *> &container, Qt::SortOrder order, T1 firstMember, T2 secondMember);

    template<typename T>
    void sortPackages(QVector<Package *> &container, Qt::SortOrder order, T member);

    // ALPM stuff
    alpm_handle_t *m_handle = nullptr;
    alpm_errno_t m_error = ALPM_ERR_OK;

    Mode m_mode = Repo;
    int m_updates = 0;
    QVector<Package *> m_repoPackages;
    QVector<Package *> m_aurPackages;
    QVector<Package *> m_outdatedPackages;
    QFuture<void> m_loadingDatabases;
    QNetworkAccessManager m_manager;
};

#endif // PACKAGESMODEL_H
