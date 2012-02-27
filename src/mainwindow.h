#ifndef PNDMANAGER_MAINWINDOW_H
#define PNDMANAGER_MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QProgressBar>
#include "packagemodel.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  MainWindow();
  ~MainWindow();

private slots:
  void install(Package* package);
  void showDetails(Package* package);
  void syncing(QPndman::SyncHandle* handle);
  void syncDone();
  void crawling();
  void crawlDone();
  
  void updateSyncProgressBarValue(qint64);
  void updateSyncProgressBarMaximum(qint64);
private:
  PackageModel packages;

  QAction syncAction;
  QAction crawlAction;
  QProgressBar syncProgressBar;
};
#endif