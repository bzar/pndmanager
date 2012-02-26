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
  PackageItem(Package* package, int index, QWidget* parent = 0);

  Package* getPackage() const;
  
public slots:
  void setPackage(Package* p);
  void connectHandle(QPndman::Handle* handle);
  void setBytesDownloaded(qint64);
  void bytesToDownloadChanged(qint64);
  
private slots:
  void handleInstall();
  void handleRemove();
  void handleDetails();
  
signals:
  void install(Package* package);
  void remove(Package* package);
  void details(Package* package);
  
private:
  Package* package;
  
  QLabel title;
  QPushButton installButton;
  QPushButton removeButton;
  QPushButton detailsButton;
  QProgressBar progressBar;
  
  qint64 bytesDownloaded;
  qint64 bytesToDownload;
};
#endif