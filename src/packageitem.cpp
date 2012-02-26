#include "packageitem.h"
#include <QDebug>
#include <QHBoxLayout>

PackageItem::PackageItem(Package* package, int index, QWidget* parent): QWidget(parent),
  package(package), title(package->getTitle()), 
  installButton("Install"), removeButton("Remove"), detailsButton("Details"), progressBar(),
  bytesDownloaded(0), bytesToDownload(0)
{
  connect(&installButton, SIGNAL(clicked(bool)), this, SLOT(handleInstall()));
  connect(&removeButton, SIGNAL(clicked(bool)), this, SLOT(handleRemove()));
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
  layout->addWidget(&installButton);
  installButton.setStyleSheet("background-color: #69D772;");
  layout->addWidget(&removeButton);
  removeButton.setStyleSheet("background-color: #D76D69;");
  layout->addWidget(&detailsButton);
  setPackage(package);
}

Package* PackageItem::getPackage() const
{
  return package;
}

void PackageItem::setPackage(Package* p)
{
  package = p;
  
  
  title.setText(package->getTitle());
  if(!package->getInstalled())
  {
    installButton.show();
    removeButton.hide();
    progressBar.setValue(0);
    progressBar.setMaximum(1);
  }
  else
  {
    installButton.hide();
    removeButton.show();
    progressBar.setValue(1);
    progressBar.setMaximum(1);
  }  
}

void PackageItem::handleInstall()
{
  installButton.setDisabled(true);
  emit install(package);
}

void PackageItem::handleRemove()
{
  removeButton.setDisabled(true);
  emit remove(package);
}

void PackageItem::handleDetails()
{
  emit details(package);
}

void PackageItem::connectHandle(QPndman::Handle* handle)
{
  connect(handle, SIGNAL(bytesDownloadedChanged(qint64)), this, SLOT(setBytesDownloaded(qint64)));
  connect(handle, SIGNAL(bytesToDownloadChanged(qint64)), this, SLOT(bytesToDownloadChanged(qint64)));
}

void PackageItem::setBytesDownloaded(qint64 value)
{
  bytesDownloaded = value;
  progressBar.show();
  progressBar.setValue(value);
}
void PackageItem::bytesToDownloadChanged(qint64 value)
{
  bytesToDownload = value;
  progressBar.show();
  progressBar.setMaximum(value);
}
