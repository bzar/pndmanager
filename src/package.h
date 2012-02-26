#ifndef PNDMANAGER_PACKAGE_H
#define PNDMANAGER_PACKAGE_H

#include "qtpndman.h"

class Package : public QPndman::Package
{
  Q_OBJECT
  Q_PROPERTY(bool installed READ getInstalled NOTIFY installedChanged);

public:
  Package(QPndman::Package const& p, bool installed, QObject* parent = 0);
  
  bool getInstalled() const;

public slots:
  void setInstalled(bool);
  void updateFrom(QPndman::Package package);

signals:
  void installedChanged(bool);
  
private:
  bool installed;
};

#endif