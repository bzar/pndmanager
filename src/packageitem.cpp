#include "packageitem.h"
#include <QDebug>
#include <QHBoxLayout>

PackageItem::PackageItem(Package* p, int index, QWidget* parent): QWidget(parent),
  package(0), title(), 
  installButton("Install"), removeButton("Remove"), detailsButton("Details"), progressBar()
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
    progressBar.setMaximum(1);
    progressBar.setValue(0);
  }
  
  title.setText(package->getTitle());
  if(!package->getInstalled())
  {
    installButton.setEnabled(true);
    installButton.show();
    removeButton.hide();
  }
  else
  {
    installButton.hide();
    removeButton.show();
    removeButton.setEnabled(true);
    progressBar.setValue(progressBar.maximum());
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
