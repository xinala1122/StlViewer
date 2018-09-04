#ifndef ROOTVIEWCONTROLLER_H
#define ROOTVIEWCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QTimer>
#include <QList>
#include "AbstractView.h"
#include "mainwindow.h"

typedef enum
{
    eViewMain,
    eViewTreeView,
    eViewFlowTool,
    eViewTabView,
    eViewCmd,
    eViewLog,
    eViewSummary,
    eViewUnknown,
    eViewNum,
}eViewType;

class QWidget;
class MainWindow;
class RootViewController : public QObject
{
    Q_OBJECT

public:
    explicit RootViewController(MainWindow* mainWindow, QObject *parent = 0);
    virtual ~RootViewController();
    void Show(MainWindow *mainWindow);
signals:
    void SigFilePath(QString);
    void SigClearTree();

public slots:

protected slots:
    void SwitchView(int viewTypeIdx);
    void SwitchView(eViewType viewType);

protected:
    AbstractView *GetViewWidget(eViewType viewType);
    QWidget *GetViewWidgets(eViewType viewType);
    void Init();

private:
    QMap<eViewType, AbstractView*> m_viewMap;
    QList<QWidget*>   m_showingWidgets;
};

#endif // ROOTVIEWCONTROLER_H
