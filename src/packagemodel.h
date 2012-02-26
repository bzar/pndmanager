#ifndef PNDMANAGER_PACKAGEMODEL_H
#define PNDMANAGER_PACKAGEMODEL_H

#include <QObject>
#include <QMap>
#include <QList>
#include "qtpndman.h"
#include "package.h"

class PackageModel : public QObject
{
  Q_OBJECT
public:
  PackageModel(QObject* parent = 0);
  ~PackageModel();

  QList<QPndman::Device*> getDevices();
public slots:
  void syncAndCrawl();
  void crawl();
  void sync();
  void install(Package* package, QPndman::Device* device, QPndman::InstallLocation location);
  void remove(Package* package);
  
signals:
  void packagesChanged(QList<Package*> const& packages);
  void installing(Package* package, QPndman::Handle* handle);
  
private slots:
  void updatePackages();
  
private:
  static QString const REPOSITORY_URL;
  
  QPndman::Context* context;
  QPndman::Repository* repository;
  QPndman::LocalRepository* localRepository;
  QPndman::Device* tmpDevice;

  QList<Package*> packages;
  QMap<QString, Package*> packagesById;
  QList<QPndman::Device*> devices;
};
#endif