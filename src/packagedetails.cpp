#include "packagedetails.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QFont>

PackageDetails::PackageDetails(Package* p, QWidget* parent): QWidget(parent),
  package(0), title(), description(), icon(),
  installButton("Install"), removeButton("Remove"), progressBar()
{
  connect(&installButton, SIGNAL(clicked(bool)), this, SLOT(handleInstall()));
  connect(&removeButton, SIGNAL(clicked(bool)), this, SLOT(handleRemove()));
  
  QVBoxLayout* layout = new QVBoxLayout;
  setLayout(layout);
  layout->addWidget(&title);
  title.setFont(QFont("serif", 24, 1, false));
  layout->addWidget(&description);
  description.setWordWrap(true);
  layout->addWidget(&icon);

  layout->addWidget(&progressBar);
  layout->addWidget(&installButton);
  installButton.setStyleSheet("background-color: #69D772;");
  layout->addWidget(&removeButton);
  removeButton.setStyleSheet("background-color: #D76D69;");
  setPackage(p);
}

Package* PackageDetails::getPackage() const
{
  return package;
}

void PackageDetails::setPackage(Package* p)
{
  if(package != p)
  {
    if(package) disconnect(package);
    package = p;    
    connect(package, SIGNAL(bytesDownloadedChanged(qint64)), this, SLOT(setBytesDownloaded(qint64)));
    connect(package, SIGNAL(bytesToDownloadChanged(qint64)), this, SLOT(bytesToDownloadChanged(qint64)));
    progressBar.setMaximum(package->getBytesToDownload());
    update();
  }  
}

void PackageDetails::update()
{
  title.setText(package->getTitle());
  description.setText(package->getDescription());
  icon.setText(package->getIcon());
  
  if(!package->getInstalled())
  {
    installButton.setEnabled(true);
    installButton.show();
    removeButton.hide();
    progressBar.setValue(package->getBytesDownloaded());
  }
  else
  {
    installButton.hide();
    removeButton.show();
    removeButton.setEnabled(true);
    progressBar.setValue(progressBar.maximum());
  }  

}

void PackageDetails::handleInstall()
{
  emit install(package);
}

void PackageDetails::handleRemove()
{
  emit remove(package);
}

void PackageDetails::setBytesDownloaded(qint64 value)
{
  installButton.setEnabled(false);
  progressBar.setValue(value);
}
void PackageDetails::bytesToDownloadChanged(qint64 value)
{
  progressBar.setMaximum(value);
}
