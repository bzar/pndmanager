#include "mainwindow.h"
#include "installdialog.h"
#include <QListView>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include "syncworker.h"
#include "packageview.h"
#include <QScrollArea>
#include <QDebug>

MainWindow::MainWindow() : QMainWindow(0), packages()
{
  QToolBar* toolBar = new QToolBar("Toolbar!", this);
  QAction* syncAction = new QAction("Synchronize with server", toolBar);
  connect(syncAction, SIGNAL(triggered(bool)), &packages, SLOT(sync()));
  toolBar->addAction(syncAction);

  QAction* crawlAction = new QAction("Refresh local PNDs", toolBar);
  toolBar->addAction(crawlAction);
  connect(crawlAction, SIGNAL(triggered(bool)), &packages, SLOT(crawl()));
  addToolBar(toolBar);
  
  QScrollArea* packageScrollArea = new QScrollArea(this);
  packageScrollArea->setWidgetResizable(true);
  PackageView* packageView = new PackageView(this);
  connect(&packages, SIGNAL(packagesChanged(QList<Package*>)), packageView, SLOT(setPackages(QList<Package*>)));
  connect(&packages, SIGNAL(installing(Package*,QPndman::Handle*)), packageView, SLOT(showProgressBar(Package*,QPndman::Handle*)));
  connect(packageView, SIGNAL(install(Package*)), this, SLOT(install(Package*)));
  connect(packageView, SIGNAL(remove(Package*)), &packages, SLOT(remove(Package*)));
  connect(packageView, SIGNAL(details(Package*)), this, SLOT(showDetails(Package*)));
  packageScrollArea->setWidget(packageView); 
  
  setCentralWidget(packageScrollArea);
  
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
