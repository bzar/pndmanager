#ifndef PNDMANAGER_PACKAGEDETAILS_H
#define PNDMANAGER_PACKAGEDETAILS_H

#include "package.h"
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>

class PackageDetails : public QWidget
{
  Q_OBJECT
public:
  PackageDetails(Package* p, QWidget* parent = 0);

  Package* getPackage() const;
  
public slots:
  void setPackage(Package* p);
  void setBytesDownloaded(qint64);
  void bytesToDownloadChanged(qint64);
  void update();
  
private slots:
  void handleInstall();
  void handleRemove();
  
signals:
  void install(Package* package);
  void remove(Package* package);
  
private:
  Package* package;
  
  QLabel title;
  QLabel description;
  QLabel icon;
  QPushButton installButton;
  QPushButton removeButton;
  QProgressBar progressBar;
};
#endif