#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "files-view/filesmodel.h"
#include "packages-view/packagesmodel.h"

#include <QMainWindow>
#include <QButtonGroup>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_searchEdit_returnPressed();
    void on_packagesTreeView_currentPackageChanged(Package *package);
    void on_packageTabsWidget_currentChanged(int index);

    void selectPackage(QAbstractButton* button);

private:
    // Package info tabs
    void loadPackageInfo(const Package *package);
    void loadPackageDeps(const Package *package);
    void loadPackageFiles(const Package *package);

    void loadDepsButtons(int row, const QList<alpm_depend_t *> &deps);

    Ui::MainWindow *ui;
    QButtonGroup depsButtonGroup;
};

#endif // MAINWINDOW_H
