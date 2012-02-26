#include "mainwindow.h"
#include "installdialog.h"
#include <QListView>
#include <QToolBar>
#include <QLabel>
#include "syncworker.h"
#include "packageview.h"
#include <QScrollArea>
#include <QDebug>
#include <QStatusBar>
#include <QScrollBar>

MainWindow::MainWindow() : QMainWindow(0), packages(), 
  syncAction("Synchronize with server", this),
  crawlAction("Refresh local PNDs", this)
{
  setWindowTitle("PND Manager");
  resize(700, 400);
  
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
  connect(&packages, SIGNAL(installing(Package*,QPndman::Handle*)), packageView, SLOT(showProgressBar(Package*,QPndman::Handle*)));
  
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
  qDebug() << "Show details for" << package->getId();
}

void MainWindow::syncing(QPndman::SyncHandle* handle)
{
  syncAction.setDisabled(true);
  statusBar()->showMessage("Synchronizing with repository");
}

void MainWindow::syncDone()
{
  syncAction.setEnabled(true);
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
