#include "packagemodel.h"
#include "syncworker.h"

#include <QDebug>
#include "downloadworker.h"

QString const PackageModel::REPOSITORY_URL("http://repo.openpandora.org/includes/get_data.php");

PackageModel::PackageModel(QObject* parent) : QObject(parent), 
  context(new QPndman::Context(this)),
  repository(new QPndman::Repository(context, REPOSITORY_URL)),
  localRepository(new QPndman::LocalRepository(context)),
  tmpDevice(new QPndman::Device(context, "/tmp")),
  packages(), packagesById(), devices()
{
  repository->loadFrom(tmpDevice);
  localRepository->loadFrom(tmpDevice);
  devices << tmpDevice;
  devices.append(QPndman::Device::detectDevices(context));
}

PackageModel::~PackageModel()
{
  tmpDevice->saveRepositories();
}

QList< QPndman::Device* > PackageModel::getDevices()
{
  return devices;
}

void PackageModel::crawl()
{
  qDebug() << "PackageModel::crawl";
  emit crawling();
  foreach(QPndman::Device* device, devices)
  {
    device->crawl();
  }
  repository->update();
  localRepository->update();
  updatePackages();
  emit crawlDone();
}

void PackageModel::sync()
{
  qDebug() << "PackageModel::sync";
  QPndman::SyncHandle* handle = repository->sync();
  emit syncing(handle);
  SyncWorker* worker = new SyncWorker(handle);
  handle->setParent(worker);
  connect(worker, SIGNAL(ready(QPndman::SyncHandle*)), repository, SLOT(update()));
  connect(worker, SIGNAL(ready(QPndman::SyncHandle*)), this, SLOT(updatePackages()));
  connect(worker, SIGNAL(ready(QPndman::SyncHandle*)), this, SIGNAL(syncDone()));
  worker->start();
}

void PackageModel::updatePackages()
{
  qDebug() << "PackageModel::updatePackages";
  QList<QPndman::Package> installedPackages = localRepository->getPackages();
  QList<QPndman::Package> remotePackages = repository->getPackages();
  
  QMap<QString, QPndman::Package> installed;
  foreach(QPndman::Package p, installedPackages)
  {
    installed.insert(p.getId(), p);
  }
  
  QMap<QString, QPndman::Package> remote;
  foreach(QPndman::Package p, remotePackages)
  {
    remote.insert(p.getId(), p);
  }
  
  QMutableListIterator<Package*> i(packages);
  packagesById.clear();
  while(i.hasNext())
  {
    Package* p = i.next();
    bool isInInstalled = installed.contains(p->getId());
    if(isInInstalled || remote.contains(p->getId()))
    {
      p->setInstalled(isInInstalled);
      QPndman::Package package = isInInstalled ? installed.value(p->getId()) : remote.value(p->getId());
      p->updateFrom(package);
      packagesById.insert(p->getId(), p);
    }
    else
    {
      i.remove();
      delete p;
    }
  }
  
  foreach(QPndman::Package p, installedPackages)
  {
    if(!packagesById.contains(p.getId()))
    {
      Package* package = new Package(p, true, this);
      packagesById.insert(package->getId(), package);
      packages << package;
    }
  }
  
  foreach(QPndman::Package p, remotePackages)
  {
    if(!packagesById.contains(p.getId()))
    {
      Package* package = new Package(p, false, this);
      packagesById.insert(package->getId(), package);
      packages << package;
    }
  }
  
  emit packagesChanged(packages);
}

void PackageModel::install(Package* package, QPndman::Device* device, QPndman::InstallLocation location)
{
  qDebug() << "PackageModel::install";
  QPndman::InstallHandle* handle = device->install(*package, location);
  if(!handle)
  {
    emit(error(QString("Error installing %1").arg(package->getTitle())));
    return;
  }
  DownloadWorker* worker = new DownloadWorker(handle);
  handle->setParent(worker);
  connect(handle, SIGNAL(bytesDownloadedChanged(qint64)), package, SIGNAL(bytesDownloadedChanged(qint64)));
  connect(handle, SIGNAL(bytesToDownloadChanged(qint64)), package, SIGNAL(bytesToDownloadChanged(qint64)));
  connect(handle, SIGNAL(done()), package, SLOT(setInstalled()));
  connect(handle, SIGNAL(done()), this, SLOT(crawl()));
  worker->start();
  emit installing(package, handle);
}

void PackageModel::remove(Package* package)
{
  qDebug() << "PackageModel::remove";
  foreach(QPndman::Device* device, devices)
  {
    if(device->getDevice() == package->getDevice())
    {
      if(device->remove(*package))
      {
        package->setBytesDownloaded(0);
        package->setInstalled(false);
        crawl();
      }
      else
      {
        emit error("Error removing pnd");
      }
    }
  }
}

void PackageModel::upgrade(Package* package)
{
  qDebug() << "PackageModel::upgrade";
  QPndman::UpgradeHandle* handle = package->upgrade();
  if(!handle)
  {
    emit(error(QString("Error upgrading %1").arg(package->getTitle())));
    return;
  }
  DownloadWorker* worker = new DownloadWorker(handle);
  handle->setParent(worker);
  connect(handle, SIGNAL(bytesDownloadedChanged(qint64)), package, SIGNAL(bytesDownloadedChanged(qint64)));
  connect(handle, SIGNAL(bytesToDownloadChanged(qint64)), package, SIGNAL(bytesToDownloadChanged(qint64)));
  connect(handle, SIGNAL(done()), this, SLOT(crawl()));
  worker->start();
  emit upgrading(package, handle);
}
