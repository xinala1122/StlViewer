#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVector3D>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include "RootViewController.h"
#include "face.h"
#include "mywidget.h"

class RootViewController;
class mywidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    const GLfloat *constData() const { return m_data.constData(); }
    int count() const { return m_count; }
    int vertexCount() const { return m_count / 6; }

    void SwitchWidget(QWidget *view);
    void SaveLayout();
    void LoadLayout();
    modelSize model_getSize();
    void model_draw();
protected:
    virtual void closeEvent(QCloseEvent *event);
signals:
    void SigOpenStlFile();
    void SigFilePath(QString);
    void SigClearTree();
private slots:
    void onAddNew();
    void onOpenProject();

private:
    myWidget                        *m_myWidget;
    RootViewController              *m_rootView;
    QMap<int, Qt::DockWidgetArea>   m_mapDockArea;
    QMap<QWidget*, QDockWidget*>    m_mapDock;
    modelSize size;
    QVector3D center;
    QVector<QVector3D> m_triAngel;
    QList<face*> faceList;
    QVector<GLfloat> m_data;
    int m_count;
};

#endif
