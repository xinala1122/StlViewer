#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QDockWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <QFileDialog>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <gl/GL.h>

MainWindow::MainWindow()
{
    this->m_myWidget = NULL;
    QMenuBar *menuBar = new QMenuBar;
    QIcon icon(":/Root/picture/SingmedizinLogo.ico");
    this->setWindowIcon(icon);
    this->setWindowTitle(tr("Singmedizin LiverViewer"));

    QMenu *menuFile = menuBar->addMenu(tr("&File"));
    QAction *openProject = new QAction(menuFile);
    openProject->setText(tr("打开文件"));
    menuFile->addAction(openProject);
    connect(openProject, &QAction::triggered, this, &MainWindow::onOpenProject);

//    QMenu *menuWindow = menuBar->addMenu(tr("&View"));
//    QAction *addNew = new QAction(menuWindow);
//    addNew->setText(tr("Add new 3D view"));
//    menuWindow->addAction(addNew);
//    connect(addNew, &QAction::triggered, this, &MainWindow::onAddNew);

    setMenuBar(menuBar);

    this->m_mapDockArea.insert(0, Qt::LeftDockWidgetArea);
    this->m_mapDockArea.insert(1, Qt::TopDockWidgetArea);
    this->m_mapDockArea.insert(2, Qt::TopDockWidgetArea);
    this->m_mapDockArea.insert(3, Qt::BottomDockWidgetArea);
    this->m_mapDockArea.insert(4, Qt::BottomDockWidgetArea);
    this->m_mapDockArea.insert(5, Qt::BottomDockWidgetArea);
    this->m_mapDockArea.insert(6, Qt::BottomDockWidgetArea);
    this->m_mapDockArea.insert(7, Qt::BottomDockWidgetArea);

    onAddNew();
    this->m_rootView = new RootViewController(this);
    connect(this, SIGNAL(SigFilePath(QString)), this->m_rootView, SIGNAL(SigFilePath(QString)));
    connect(this, SIGNAL(SigClearTree()), this->m_rootView, SIGNAL(SigClearTree()));

}
void MainWindow::onOpenProject()
{
    if (this->m_myWidget != NULL)
    {
        emit SigOpenStlFile();
    }
}

void MainWindow::onAddNew()
{
    if (!centralWidget())
    {
        this->m_myWidget = new myWidget(this);
        connect(this, SIGNAL(SigOpenStlFile()), this->m_myWidget, SLOT(slot_openStl()));
        connect(this->m_myWidget, SIGNAL(signal_loadStl(QString)), this, SIGNAL(SigFilePath(QString)));
        connect(this->m_myWidget, SIGNAL(SigClearTree()), this, SIGNAL(SigClearTree()));
        setCentralWidget(this->m_myWidget);
    }
    else
        QMessageBox::information(0, tr("Cannot add new window"), tr("Already occupied. Undock first."));
}

void MainWindow::SwitchWidget(QWidget *view)
{
    if (view != 0)
    {
        QString widgetTitle = view->windowTitle();
        QDockWidget *dock = new QDockWidget(widgetTitle, this, 0);
        dock->setObjectName(widgetTitle);
        dock->setWindowTitle(dock->objectName());

        dock->setWidget(view);
        addDockWidget(this->m_mapDockArea[this->m_mapDock.count()], dock);

        if (!this->m_mapDock.contains(view))
        {
            this->m_mapDock.insert(view, dock);
        }
    }
}

void MainWindow::SaveLayout()
{
    QString fileName = QCoreApplication::applicationDirPath();
    fileName += "/WidgetPlace";
    //QFileDialog::getSaveFileName(this, tr("Save layout"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QString msg = tr("Failed to open %1\n%2")
                        .arg(QDir::toNativeSeparators(fileName), file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }

    QByteArray geo_data = saveGeometry();
    QByteArray layout_data = saveState();

    bool ok = file.putChar((uchar)geo_data.size());
    if (ok)
        ok = file.write(geo_data) == geo_data.size();
    if (ok)
        ok = file.write(layout_data) == layout_data.size();

    if (!ok) {
        QString msg = tr("Error writing to %1\n%2")
                        .arg(QDir::toNativeSeparators(fileName), file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
}

void MainWindow::LoadLayout()
{
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Load layout"));
    QString fileName = QCoreApplication::applicationDirPath();
    fileName += "/EvtITEWidgetPlace";
    QFile file(fileName);
    if (fileName.isEmpty())
        return;
    if (!file.exists())
        return;
    if (!file.open(QFile::ReadOnly)) {
        QString msg = tr("Failed to open %1\n%2")
                        .arg(QDir::toNativeSeparators(fileName), file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }

    uchar geo_size;
    QByteArray geo_data;
    QByteArray layout_data;

    bool ok = file.getChar((char*)&geo_size);
    if (ok) {
        geo_data = file.read(geo_size);
        ok = geo_data.size() == geo_size;
    }
    if (ok) {
        layout_data = file.readAll();
        ok = layout_data.size() > 0;
    }

    if (ok)
        ok = restoreGeometry(geo_data);
    if (ok)
        ok = restoreState(layout_data);

    if (!ok) {
        QString msg = tr("Error reading %1").arg(QDir::toNativeSeparators(fileName));
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    bool result = true;
    QMessageBox::warning(this, tr("Warning"), tr("Are you sure you want to shutdown the system?"), QMessageBox::Yes, QMessageBox::Cancel);
    if(result)
    {
        this->SaveLayout();
        this->close();
    }
    else
    {
        event->ignore();
    }
}
