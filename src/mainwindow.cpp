#include "mainwindow.h"
#include "installdialog.h"
#include "packagedetails.h"
#include "syncworker.h"
#include "packageview.h"

#include <QListView>
#include <QToolBar>
#include <QLabel>
#include <QScrollArea>
#include <QDebug>
#include <QStatusBar>
#include <QScrollBar>

MainWindow::MainWindow() : QMainWindow(0), packages(), 
  syncAction("Synchronize with server", this),
  crawlAction("Refresh local PNDs", this),
  syncProgressBar()
{
  setWindowTitle("PND Manager");
  resize(700, 400);

  statusBar()->addPermanentWidget(&syncProgressBar);
  syncProgressBar.hide();
  syncProgressBar.setMaximumSize(128, 16);
  QToolBar* toolBar = new QToolBar("Toolbar!", this);
  addToolBar(toolBar);

  toolBar->addAction(&syncAction);
  toolBar->addAction(&crawlAction);
  connect(&syncAction, SIGNAL(triggered(bool)), &packages, SLOT(sync()));
  connect(&crawlAction, SIGNAL(triggered(bool)), &packages, SLOT(crawl()));
  
  QScrollArea* packageScrollArea = new QScrollArea(this);
  PackageView* packageView = new PackageView(this);
  packageScrollArea->setWidget(packageView);
  packageScrollArea->setWidgetResizable(true);
  
  setCentralWidget(packageScrollArea);
  
  connect(&packages, SIGNAL(packagesChanged(QList<Package*>)), packageView, SLOT(setPackages(QList<Package*>)));
  
  connect(&packages, SIGNAL(syncing(QPndman::SyncHandle*)), this, SLOT(syncing(QPndman::SyncHandle*)));
  connect(&packages, SIGNAL(syncDone()), this, SLOT(syncDone()));
  connect(&packages, SIGNAL(crawling()), this, SLOT(crawling()));
  connect(&packages, SIGNAL(crawlDone()), this, SLOT(crawlDone()));
  
  connect(packageView, SIGNAL(install(Package*)), this, SLOT(install(Package*)));
  connect(packageView, SIGNAL(remove(Package*)), &packages, SLOT(remove(Package*)));
  connect(packageView, SIGNAL(details(Package*)), this, SLOT(showDetails(Package*)));
  
  packages.crawl();
}

MainWindow::~MainWindow()
{
}

void MainWindow::install(Package* package)
{
  QStringList deviceMounts;
  foreach(QPndman::Device* device, packages.getDevices())
  {
    deviceMounts << device->getMount();
  }
  
  InstallDialog dialog(deviceMounts);
  if(dialog.exec())
  {
   foreach(QPndman::Device* device, packages.getDevices())
    {
      QString mount = dialog.getDeviceMount();
      if(device->getMount() == mount)
      {
        packages.install(package, device, dialog.getInstallLocation());
        break;
      }
    }
  }
}

void MainWindow::showDetails(Package* package)
{
  PackageDetails* details = new PackageDetails(package);
  connect(details, SIGNAL(install(Package*)), this, SLOT(install(Package*)));
  connect(details, SIGNAL(remove(Package*)), &packages, SLOT(remove(Package*)));
  connect(&packages, SIGNAL(packagesChanged(QList<Package*>)), details, SLOT(update()));
  details->show();
}

void MainWindow::syncing(QPndman::SyncHandle* handle)
{
  syncAction.setDisabled(true);
  connect(handle, SIGNAL(bytesDownloadedChanged(qint64)), this, SLOT(updateSyncProgressBarValue(qint64)));
  connect(handle, SIGNAL(bytesToDownloadChanged(qint64)), this, SLOT(updateSyncProgressBarMaximum(qint64)));
  syncProgressBar.setValue(handle->getBytesDownloaded());
  syncProgressBar.setMaximum(handle->getBytesToDownload());
  syncProgressBar.show();
  statusBar()->showMessage("Synchronizing with repository");
}

void MainWindow::syncDone()
{
  syncAction.setEnabled(true);
  syncProgressBar.hide();
  statusBar()->showMessage("Successfully synchronized with repository");
}

void MainWindow::crawling()
{
  crawlAction.setDisabled(true);
  statusBar()->showMessage("Finding local PNDs");
}

void MainWindow::crawlDone()
{
  crawlAction.setEnabled(true);
  statusBar()->showMessage("Local PNDs updated");
}

void MainWindow::updateSyncProgressBarValue(qint64 value)
{
  syncProgressBar.setValue(value);
}

void MainWindow::updateSyncProgressBarMaximum(qint64 maximum)
{
  syncProgressBar.setMaximum(maximum);
}
