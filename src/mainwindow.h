#ifndef PNDMANAGER_MAINWINDOW_H
#define PNDMANAGER_MAINWINDOW_H

#include <QMainWindow>
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
  
private:
  PackageModel packages;
  
};
#endif