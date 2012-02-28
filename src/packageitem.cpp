#include "packageitem.h"
#include <QDebug>
#include <QHBoxLayout>

PackageItem::PackageItem(Package* p, int index, QWidget* parent): QWidget(parent),
  package(0), title(), size(),
  installButton("Install"), removeButton("Remove"), upgradeButton("Upgrade"), detailsButton("Details"), progressBar()
{
  connect(&installButton, SIGNAL(clicked(bool)), this, SLOT(handleInstall()));
  connect(&removeButton, SIGNAL(clicked(bool)), this, SLOT(handleRemove()));
  connect(&upgradeButton, SIGNAL(clicked(bool)), this, SLOT(handleUpgrade()));
  connect(&detailsButton, SIGNAL(clicked(bool)), this, SLOT(handleDetails()));
  
  if(index % 2)
  {
    QPalette newPalette(palette());
    newPalette.setColor(QPalette::AlternateBase, QColor(0, 0, 0, 32));
    setBackgroundRole(QPalette::AlternateBase);
    setPalette(newPalette);
  }
  
  setAutoFillBackground(true);
  
  QHBoxLayout* layout = new QHBoxLayout;
  setLayout(layout);
  layout->addWidget(&progressBar);
  progressBar.setMaximumWidth(64);
  layout->addWidget(&title);
  title.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  layout->addWidget(&size);
  layout->addWidget(&upgradeButton);
  upgradeButton.setStyleSheet("background-color: #6992D7; width: 48px;");
  layout->addWidget(&installButton);
  installButton.setStyleSheet("background-color: #69D772; width: 48px;");
  layout->addWidget(&removeButton);
  removeButton.setStyleSheet("background-color: #D76D69; width: 48px;");
  layout->addWidget(&detailsButton);
  setPackage(p);
}

Package* PackageItem::getPackage() const
{
  return package;
}

void PackageItem::setPackage(Package* p)
{
  if(package != p)
  {
    if(package) disconnect(package);
    package = p;    
    connect(package, SIGNAL(bytesDownloadedChanged(qint64)), this, SLOT(setBytesDownloaded(qint64)));
    connect(package, SIGNAL(bytesToDownloadChanged(qint64)), this, SLOT(bytesToDownloadChanged(qint64)));
    progressBar.setMaximum(package->getBytesToDownload());
  }
  
  title.setText(package->getTitle());
  size.setText(QString("%1 KiB").arg(package->getSize()/1024));
  
  if(!package->getInstalled())
  {
    installButton.setEnabled(true);
    installButton.show();
    upgradeButton.hide();
    removeButton.hide();
    progressBar.setValue(package->getBytesDownloaded());
  }
  else
  {
    installButton.hide();
    removeButton.show();
    removeButton.setEnabled(true);
    progressBar.setValue(progressBar.maximum());

    if(package->getUpgradeCandidate().isNull())
    {
      upgradeButton.hide();
    }
    else
    {
      upgradeButton.show();
    }

  }  
}

void PackageItem::handleInstall()
{
  emit install(package);
}

void PackageItem::handleRemove()
{
  emit remove(package);
}

void PackageItem::handleUpgrade()
{
  emit upgrade(package);
}

void PackageItem::handleDetails()
{
  emit details(package);
}

void PackageItem::setBytesDownloaded(qint64 value)
{
  installButton.setEnabled(false);
  progressBar.setValue(value);
}
void PackageItem::bytesToDownloadChanged(qint64 value)
{
  progressBar.setMaximum(value);
}
