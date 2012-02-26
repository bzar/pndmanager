#include "packageview.h"
#include <QVBoxLayout>
#include <QDebug>

PackageView::PackageView(QWidget* parent) : QWidget(parent)
{
  QVBoxLayout* layout = new QVBoxLayout;
  layout->setSpacing(0);
  setLayout(layout);
}
PackageView::~PackageView()
{

}

bool titleLessThan(QPndman::Package const* p1, QPndman::Package const* p2)
{
  return p1->getTitle().toLower() < p2->getTitle().toLower();
}
void PackageView::setPackages(QList<Package*> packages)
{
  qDebug() << "PackageView::setPackages";
  qSort(packages.begin(), packages.end(), titleLessThan);
  
  int i = 0;
  
  foreach(Package* p, packages)
  {
    PackageItem* item = 0;
    if(i < packageItems.size())
    {
      item = packageItems.at(i);
      item->setPackage(p);
    }
    else
    {
      item = new PackageItem(p, i, this);
      connect(item, SIGNAL(install(Package*)), this, SIGNAL(install(Package*)));
      connect(item, SIGNAL(remove(Package*)), this, SIGNAL(remove(Package*)));
      connect(item, SIGNAL(details(Package*)), this, SIGNAL(details(Package*)));
      
      packageItems << item;
      layout()->addWidget(item);
    }
    ++i;
  }
  
  for(; i < packageItems.size(); ++i)
  {
    PackageItem* item = packageItems.takeLast();
    delete item;
  }
}

void PackageView::showProgressBar(Package* package, QPndman::Handle* handle)
{
  qDebug() << "PackageView::showProgressBar";
  foreach(PackageItem* item, packageItems)
  {
    if(item->getPackage() == package)
    {
      item->connectHandle(handle);
      break;
    }
  }
}
