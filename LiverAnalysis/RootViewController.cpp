#include "TreeView.h"
#include "MainWindow.h"
#include "RootViewController.h"
#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QSplashScreen>
#include <QMessageBox>
#include <QProgressDialog>

RootViewController::RootViewController(MainWindow *mainWindow, QObject *parent) :
    QObject(parent)
{
	this->m_showingWidgets.clear();
    this->SwitchView(eViewTreeView);
    this->Init();
    this->Show(mainWindow);
}

RootViewController::~RootViewController()
{
    qDeleteAll(this->m_viewMap.begin(),this->m_viewMap.end());
    this->m_viewMap.clear();
}


void RootViewController::Init()
{
    //this->m_pMainWindow->Init();
    //this->Show();
}

void RootViewController::Show(MainWindow *mainWindow)
{
    mainWindow->SwitchWidget(this->GetViewWidget(eViewTreeView)->GetViewsFrame());
    //mainWindow->LoadLayout();
    mainWindow->setMinimumSize(800, 600);
    mainWindow->showMaximized();
}

void RootViewController::SwitchView(int viewTypeIdx)
{
    if(viewTypeIdx >= eViewMain && viewTypeIdx < eViewNum)
    {
        this->SwitchView((eViewType)viewTypeIdx);
    }
}

void RootViewController::SwitchView(eViewType viewType)
{
    AbstractView* pViews = this->GetViewWidget(viewType);
    if(0 != pViews)
    {
        QWidget* pWidget = pViews->GetViewsFrame();
        if(0 != pWidget)
        {
            bool isShowing = true;
			if (!this->m_showingWidgets.contains(pWidget))
            {
				this->m_showingWidgets.append(pWidget);	
                //this->m_pMainWindow->SwitchWidget(pWidget, isShowing);
            }
            else
            {
                //this->m_pMainWindow->SwitchWidget(pWidget, !isShowing);
				this->m_showingWidgets.removeAt(this->m_showingWidgets.indexOf(pWidget));
            }
        }
    }
}

AbstractView *RootViewController::GetViewWidget(eViewType viewType)
{
    AbstractView* pViews =0;
    if(this->m_viewMap.contains(viewType))
    {
        pViews = this->m_viewMap.value(viewType);
    }
    else
    {
		switch (viewType)
		{
		case eViewTreeView:
		{
			TreeView * pTreeView = new TreeView();
            connect(this, SIGNAL(SigFilePath(QString)), pTreeView, SLOT(OnProjectLoadFinish(QString)));
            connect(this, SIGNAL(SigClearTree()), pTreeView, SLOT(OnClearTree()));

			pViews = pTreeView;
			break;
		}
        case eViewUnknown:
            break;
        default:
            break;
        }

        if(0 != pViews)
        {
            this->m_viewMap.insert(viewType, pViews);
        }
    }

    return pViews;
}

QWidget *RootViewController::GetViewWidgets(eViewType viewType)
{
    QWidget* pWidget = 0;

    AbstractView* pViews = this->GetViewWidget(viewType);

    if(0 != pViews)
    {
        pWidget = pViews->GetViewsFrame();
    }

    return pWidget;
}

