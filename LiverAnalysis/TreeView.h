#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QObject>
#include <QTreeView>
#include <QStandardItem>
#include <QFileInfo>
#include <QFileDialog>
#include <QMenu>
#include <QTableView>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QModelIndex>
#include <QProcess>
#include <QMessageBox>
#include "StandardItemModelEx.h"
#include "AbstractView.h"



#define ROLE_MARK Qt::UserRole + 1
#define ROLE_MARK_FOLDER Qt::UserRole + 2
#define ROLE_MARK_ITEM Qt::UserRole + 3
#define ROLE_MARK_LEAF Qt::UserRole + 4

//对应ROLE_MARK
#define MARK_PROJECT 1
#define MARK_FOLDER 2
#define MARK_ITEM 3
#define MARK_LEAF 4

enum EverestMannFolderType
{
    MARK_FOLDER_TESTERCONFIG = 0,
    MARK_FOLDER_SIGNAL,
    MARK_FOLDER_SIGNALGROUPS,
    MARK_FOLDER_SOCKETMAP,
    MARK_FOLDER_USERVARS,
    MARK_FOLDER_SPECSET,
    MARK_FOLDER_LEVELS,
    MARK_FOLDER_TIMING,
    MARK_FOLDER_TIMINGMAP,
    MARK_FOLDER_PATTERNBURST,
    MARK_FOLDER_TESTBLOCK,
    MARK_FOLDER_BINDEFINITION,
    MARK_FOLDER_FLOWS,
    MARK_FOLDER_TESTPROGRAM,
    MARK_FOLDER_RUNRESULTMAP,
    MARK_FOLDER_TESTITEMLIMIT,
    MARK_FOLDER_UNDEFINED = -1
};

enum EverestMannItemType
{
    MARK_ITEM_TESTERCONFIG = 0,
    MARK_ITEM_SIGNAL,
    MARK_ITEM_SIGNALGROUPS,
    MARK_ITEM_SOCKETMAP,
    MARK_ITEM_USERVARS,
    MARK_ITEM_SPECSET,
    MARK_ITEM_LEVELS,
    MARK_ITEM_TIMING,
    MARK_ITEM_TIMINGMAP,
    MARK_ITEM_PATTERNBURST,
    MARK_ITEM_TESTBLOCK,
    MARK_ITEM_BINDEFINITION,
    MARK_ITEM_FLOWS,
    MARK_ITEM_TESTPROGRAM,
    MARK_ITEM_RUNRESULTMAP,
    MARK_ITEM_TESTITEMLIMIT,
    MARK_ITEM_UNDEFINED = -1
};

enum EverestMannLeafType
{
    MARK_LEAF_TESTERCONFIG = 0,
    MARK_LEAF_SIGNAL,
    MARK_LEAF_SIGNALGROUPS,
    MARK_LEAF_SOCKETMAP,
    MARK_LEAF_USERVARS,
    MARK_LEAF_SPECSET,
    MARK_LEAF_LEVELS,
    MARK_LEAF_TIMING,
    MARK_LEAF_TIMINGMAP,
    MARK_LEAF_PATTERNBURST,
    MARK_LEAF_TESTBLOCK,
    MARK_LEAF_BINDEFINITION,
    MARK_LEAF_FLOWS,
    MARK_LEAF_TESTPROGRAM,
    MARK_LEAF_RUNRESULTMAP,
    MARK_LEAF_TESTITEMLIMIT,
    MARK_LEAF_UNDEFINED = -1
};

class TreeModelFlowsItem;

class TreeView : public AbstractView

{
	Q_OBJECT

public:
	explicit TreeView(QObject *parent = 0);
	~TreeView();
	void Init();
	QWidget* GetViewsFrame();
	QStandardItem* GetItemFolder(EverestMannFolderType folderType, EverestMannItemType type);
	QStandardItem* GetFolder(EverestMannFolderType type);
	QStandardItem* GetProjectFolder();
	QList<QStandardItem*> GetRoots();
	QStandardItemModel* GetTreeModel();
	TreeModelFlowsItem* GetTreeModelFlowsItem();

    bool BindFormTreeViewCallBack();

	void initMenu();
	void callback_clearColor(QStandardItem* item);
    void AddRootsToTree(QString itemName, QStandardItem* root);
public:
	QMenu *m_itemMenu;
	QString m_strJobPath;
	QStandardItemModel *mode0;
	QStandardItemModel *mode1;
	QStandardItemModel *mode2;
	QStandardItemModel *modeTree;
public slots:
	void OnCustomContextMenuRequested(const QPoint &pos);
	void OnTreeViewClicked(const QModelIndex &index);
	void OnTreeViewDoubleClicked(const QModelIndex &index);
    void actionOpenSlot();
    void actionExplorerSlot();
	void OnTreeViewLoadProjectFile();
    void OnProjectLoadFinish(QString filePath);

    void OnClearTree();
signals:
    void SendMsgToTabView(QString fileName);

private:

	QTreeView *m_pTreeView;
	bool IsProjectLoadSuccess;
	QString strPathWithoutFileName;
	QString strPathIncludeFileName;
    QVector<QString>        m_treeProjectVector;

};

#endif // TREEVIEW_H
