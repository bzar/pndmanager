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
  foreach(QPndman::Device* device, devices)
  {
    device->crawl();
  }
  localRepository->update();
  updatePackages();
}

void PackageModel::sync()
{
  qDebug() << "PackageModel::sync";
  QPndman::SyncHandle* handle = repository->sync();
  SyncWorker* worker = new SyncWorker(handle);
  connect(worker, SIGNAL(ready(QPndman::SyncHandle*)), repository, SLOT(update()));
  connect(worker, SIGNAL(ready(QPndman::SyncHandle*)), this, SLOT(updatePackages()));
  worker->start();
}

void PackageModel::syncAndCrawl()
{
  qDebug() << "PackageModel::syncAndCrawl";
  foreach(QPndman::Device* device, devices)
  {
    device->crawl();
  }
  QPndman::SyncHandle* handle = repository->sync();
  SyncWorker* worker = new SyncWorker(handle);
  connect(worker, SIGNAL(ready(QPndman::SyncHandle*)), repository, SLOT(update()));
  connect(worker, SIGNAL(ready(QPndman::SyncHandle*)), this, SLOT(updatePackages()));
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
    if(installed.contains(p->getId()))
    {
      p->setInstalled(true);
      QPndman::Package package = installed.value(p->getId());
      p->updateFrom(package);
      packagesById.insert(p->getId(), p);
    }
    else if(remote.contains(p->getId()))
    {
      p->setInstalled(false);
      QPndman::Package package = remote.value(p->getId());
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
  QPndman::Handle* handle = device->install(*package, location);
  DownloadWorker* worker = new DownloadWorker(handle);
  connect(handle, SIGNAL(done()), this, SLOT(crawl()));
  worker->start();
  emit installing(package, handle);
}

void PackageModel::remove(Package* package)
{
  qDebug() << "PackageModel::remove";
  qDebug() << "PackageModel::remove, looking for" << package->getDevice();
  foreach(QPndman::Device* device, devices)
  {
    if(device->getDevice() == package->getDevice())
    {
      qDebug() << "PackageModel::remove, removing from" << device->getDevice();
      QPndman::Handle* handle = device->remove(*package);
      DownloadWorker* worker = new DownloadWorker(handle);
      connect(handle, SIGNAL(done()), this, SLOT(crawl()));
      worker->start();
    }
  }
  updatePackages();
}
