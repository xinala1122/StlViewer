#include "TreeView.h"
#include "qabstractitemview.h"
TreeView::TreeView(QObject *parent)	:
	AbstractView(parent)
{
    this->IsProjectLoadSuccess = false;
	Init();
}

TreeView::~TreeView()
{
	delete m_pTreeView;
}

void TreeView::Init()
{
    m_pTreeView = new QTreeView;
	m_pTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_pTreeView->setEditTriggers(0);
    m_pTreeView->setWindowTitle("Project View");
	connect(m_pTreeView, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(OnTreeViewDoubleClicked(const QModelIndex)));
	connect(m_pTreeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(OnCustomContextMenuRequested(const QPoint &)));
    this->m_treeProjectVector.clear();

    initMenu();

    this->BindFormTreeViewCallBack();
}

QWidget* TreeView::GetViewsFrame()
{
	return m_pTreeView;
}

void TreeView::OnClearTree()
{
    this->m_treeProjectVector.clear();
    this->BindFormTreeViewCallBack();
}

void TreeView::OnTreeViewLoadProjectFile()
{
	m_strJobPath = QFileDialog::getOpenFileName(0, tr(""), 0, tr(""));
    QStringList projectPathList = m_strJobPath.split('\\');
    if (m_strJobPath.isEmpty())
    {
        QMessageBox::about(NULL, "Error", "Load Project Failed");
    }
}

bool TreeView::BindFormTreeViewCallBack()
{
    QString fileName = QStringLiteral("工程名称");
    QFileInfo  info(m_strJobPath);
	
    QStandardItemModel* model = new QStandardItemModel();
    model->setHorizontalHeaderLabels(QStringList() << QStringLiteral("Project"));
	
    QStandardItem* root = new QStandardItem(QIcon(":/icon/icon/p.png"), fileName);
	root->setData(MARK_PROJECT, ROLE_MARK);
	root->setData(MARK_FOLDER, ROLE_MARK_FOLDER);

    model->appendRow(root);
    m_pTreeView->setModel(model);

    QVector<QString>::iterator ProjectItemTypeIt;
    for (ProjectItemTypeIt = this->m_treeProjectVector.begin(); ProjectItemTypeIt != this->m_treeProjectVector.end(); ++ProjectItemTypeIt)
	{
        AddRootsToTree(*ProjectItemTypeIt, root);
	}

    this->m_pTreeView->expandToDepth(0);

	return true;
}

void TreeView::OnProjectLoadFinish(QString filePath)
{
    //this->m_treeProjectVector.clear();
    QFileInfo  info(filePath);

    QString fileName = info.fileName();
    this->m_treeProjectVector.append(fileName);
    this->BindFormTreeViewCallBack();
}

QStandardItem* TreeView::GetItemFolder(EverestMannFolderType folderType, EverestMannItemType type)
{
	QStandardItem* Folder = GetFolder(folderType);
	if (nullptr == Folder)
		return nullptr;
	for (int i = 0; i < Folder->rowCount(); ++i)
	{
		QStandardItem* child = Folder->child(i);
		QVariant var = child->data(ROLE_MARK_ITEM);
		if (!var.isValid())
			continue;
		if (type == var.value<int>())
			return child;
	}
	return nullptr;

}

QStandardItem* TreeView::GetFolder(EverestMannFolderType type)
{
	QStandardItem* project = GetProjectFolder();
	if (nullptr == project)
		return nullptr;
	for (int i = 0; i < project->rowCount(); ++i)
	{
		QStandardItem* child = project->child(i);
		QVariant var = child->data(ROLE_MARK_FOLDER);
		if (!var.isValid())
			continue;
		if (type == var.value<int>())
			return child;
	}
	return nullptr;
}



QStandardItem* TreeView::GetProjectFolder()
{
	QList<QStandardItem*> roots = GetRoots();
	for (auto i = roots.begin(); i != roots.end(); ++i){
		if ((*i)->data(ROLE_MARK) == MARK_PROJECT){
			return (*i);
		}
	}
	return nullptr;
}

QList<QStandardItem*> TreeView::GetRoots()
{
	QList<QStandardItem*> roots;
	QStandardItemModel* model = GetTreeModel();
	for (int i = 0; i < model->rowCount(); ++i)
	{
		roots.append(model->item(i));
	}
	return roots;
}

QStandardItemModel* TreeView::GetTreeModel()
{
	return qobject_cast<QStandardItemModel*>(m_pTreeView->model());
}



void TreeView::AddRootsToTree(QString itemName, QStandardItem* root)
{
    QString rootName = itemName;

    QStandardItem* folder = new QStandardItem(QIcon(":/icon/icon/h-f.png"), rootName);
    folder->setData(MARK_FOLDER, ROLE_MARK);
    //folder->setData(folderType, ROLE_MARK_FOLDER);
    root->appendRow(folder);
}

void TreeView::initMenu()
{
    QAction* ac;
	m_itemMenu = new QMenu(m_pTreeView);
    ac = new QAction(QStringLiteral("打开文件"), m_pTreeView);
	connect(ac, SIGNAL(triggered()), this, SLOT(actionOpenSlot()));
	m_itemMenu->addAction(ac);
	ac = new QAction(QStringLiteral("在explorer中显示"), m_pTreeView);
    connect(ac, SIGNAL(triggered()), this, SLOT(actionExplorerSlot()));
	m_itemMenu->addAction(ac);
}

void TreeView::OnCustomContextMenuRequested(const QPoint &pos)
{
	QModelIndex index = m_pTreeView->indexAt(pos);
	if (!index.isValid())
	{
		return;
	}
    StandardItemModelEx::ergodicAllItem(GetTreeModel()
        , std::bind(&TreeView::callback_clearColor, this, std::placeholders::_1));
    GetTreeModel()->itemFromIndex(index)->setData(QColor(232, 209, 57, 200), Qt::BackgroundRole);
	QVariant var = index.data(ROLE_MARK);
	QString str = index.data().toString();
	if (var.isValid())
	{
        if (MARK_ITEM == var.toInt())
			m_itemMenu->exec(QCursor::pos());
	}
}

void TreeView::OnTreeViewClicked(const QModelIndex &index)
{

	if (!index.isValid())
		return;
    StandardItemModelEx::ergodicAllItem(GetTreeModel()
        , std::bind(&TreeView::callback_clearColor, this, std::placeholders::_1));
    GetTreeModel()->itemFromIndex(index)->setData(QColor(232, 209, 57, 200), Qt::BackgroundRole);
}

void TreeView::callback_clearColor(QStandardItem* item)
{
    item->setData(QVariant(), Qt::BackgroundRole);
}

void TreeView::OnTreeViewDoubleClicked(const QModelIndex &index)
{
	if (!index.isValid())
		return;
    StandardItemModelEx::ergodicAllItem(GetTreeModel()
        , std::bind(&TreeView::callback_clearColor, this, std::placeholders::_1));
    GetTreeModel()->itemFromIndex(index)->setData(QColor(232, 209, 57, 200), Qt::BackgroundRole);

	QVariant var = GetTreeModel()->QStandardItemModel::data(index);
	QString filename = var.toString();
    emit SendMsgToTabView(filename);
}


void TreeView::actionOpenSlot()
{

}

void TreeView::actionExplorerSlot()
{
    QProcess process;

    QFileInfo fileInfo(strPathIncludeFileName);

    QString filePath = QFileInfo(m_strJobPath).path() + "/" + fileInfo.fileName();

    QFileInfo fileInfoPath(filePath);

    if (!fileInfoPath.isFile())
    {
        filePath = QFileInfo(m_strJobPath).path() + "/" + strPathIncludeFileName;
    }

    #ifdef WIN32
    filePath.replace("/", "\\");
    #endif
    process.startDetached("explorer /select," + filePath);
}
