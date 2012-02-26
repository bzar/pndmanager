#include "package.h"

Package::Package(QPndman::Package const& p, bool installed, QObject* parent):
  QPndman::Package(p), installed(installed)
{
  setParent(parent);
}

bool Package::getInstalled() const
{
  return installed;
}

void Package::setInstalled(bool value)
{
  if(installed != value)
  {
    installed = value;
    emit installedChanged(installed);
  }
}

void Package::updateFrom(QPndman::Package package)
{
  setPndmanPackage(package.getPndmanPackage());
  setPath(package.getPath());
  setId(package.getId());
  setIcon(package.getIcon());
  setInfo(package.getInfo());
  setMd5(package.getMd5());
  setUrl(package.getUrl());
  setVendor(package.getVendor());
  setDevice(package.getDevice());
  setSize(package.getSize());
  setModified(package.getModified());
  setRating(package.getRating());
  setAuthor(package.getAuthor());
  setVersion(package.getVersion());
  setApplications(package.getApplications());
  setTitles(package.getTitles());
  setDescriptions(package.getDescriptions());
  setCategories(package.getCategories());
}
