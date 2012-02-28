#ifndef PNDMANAGER_PACKAGEITEM_H
#define PNDMANAGER_PACKAGEITEM_H

#include "package.h"
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>

class PackageItem : public QWidget
{
  Q_OBJECT
public:
  PackageItem(Package* p, int index, QWidget* parent = 0);

  Package* getPackage() const;
  
public slots:
  void setPackage(Package* p);
  void setBytesDownloaded(qint64);
  void bytesToDownloadChanged(qint64);
  
private slots:
  void handleInstall();
  void handleRemove();
  void handleUpgrade();
  void handleDetails();
  
signals:
  void install(Package* package);
  void remove(Package* package);
  void upgrade(Package* package);
  void details(Package* package);
  
private:
  Package* package;
  
  QLabel title;
  QLabel size;
  QPushButton installButton;
  QPushButton removeButton;
  QPushButton upgradeButton;
  QPushButton detailsButton;
  QProgressBar progressBar;
};
#endif