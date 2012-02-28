#ifndef PNDMANAGER_PACKAGEVIEW_H
#define PNDMANAGER_PACKAGEVIEW_H

#include <QWidget>

#include "packagemodel.h"
#include "packageitem.h"

class PackageView : public QWidget
{
  Q_OBJECT
public:
  PackageView(QWidget* parent = 0);
  virtual ~PackageView();
  
public slots:
  void setPackages(QList<Package*> packages);
signals: 
  void install(Package* package);
  void remove(Package* package);
  void upgrade(Package* package);
  void details(Package* package);
  
private:
  QList<PackageItem*> packageItems;
};
#endif