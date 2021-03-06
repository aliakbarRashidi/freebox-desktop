#include "fstaskwidget.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QToolBar>
#include <QAction>
FSTaskWidget::FSTaskWidget(FbxAPI *fbx, QWidget *parent) :
    QMainWindow(parent)
{
    mFbx = fbx;
    //    mActionModel   =  new FSActionTaskModel(mFbx);
    mDownloadModel = new FSDownloadTaskModel(mFbx);
    mUploadModel   = new FSUploadTaskModel(mFbx);

    mDownloadView  = new QListView;
    mUploadView    = new QListView;
    //    mActionView    = new QListView;

    mDelegate = new FSTaskDelegate;
    mTimer = new QTimer;
    mTabWidget = new QTabWidget;


    //    mTimer->setInterval(1000);

    //    mActionView->setAlternatingRowColors(true);
    //    mActionView->setItemDelegate(mDelegate);
    //    mActionView->setModel(mActionModel);

    mDownloadView->setAlternatingRowColors(true);
    mDownloadView->setItemDelegate(mDelegate);
    mDownloadView->setModel(mDownloadModel);

    mUploadView->setAlternatingRowColors(true);
    mUploadView->setItemDelegate(mDelegate);
    mUploadView->setModel(mUploadModel);



    mTabWidget->addTab(mDownloadView,QIcon(":inbox_download"),"Download");
    mTabWidget->addTab(mUploadView,QIcon(":inbox_upload"),"Upload");
    //    mTabWidget->addTab(mActionView,QIcon(""),"Actions");


    //    setLayout(new QVBoxLayout);
    QToolBar* toolbar = new QToolBar();
    toolbar->setFloatable(false);
    toolbar->setMovable(false);
    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbar->setIconSize(QSize(16,16));

    addToolBar(Qt::BottomToolBarArea,toolbar);

    QAction* clearTask = toolbar->addAction(QIcon(":broom.png"),"Effacer les terminés");
    QAction* remTask   = toolbar->addAction(QIcon(":cancel.png"),"Arreter");

    connect(mDownloadModel,SIGNAL(countChanged()),this,SLOT(computeTasksCount()));
    connect(mUploadModel,SIGNAL(countChanged()),this,SLOT(computeTasksCount()));
    connect(clearTask,SIGNAL(triggered()),this,SLOT(clearTasks()));
    connect(remTask,SIGNAL(triggered()),this,SLOT(removeSelectedTasks()));

    setContextMenuPolicy(Qt::ActionsContextMenu);
    resize(400,300);
    setMinimumSize(400,300);
    setWindowTitle("Gestionnaire de tâche");
    setCentralWidget(mTabWidget);

    //    setWindowFlags ( Qt::CustomizeWindowHint | Qt::WindowTitleHint);


    connect(mDownloadView,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(downloadClicked(QModelIndex)));

}


int FSTaskWidget::count()
{
    return mDownloadModel->count();
}

void FSTaskWidget::clearTasks()
{

    qDebug()<<"clear task";
    switch (mTabWidget->currentIndex())
    {
    case 0 : mDownloadModel->clearFinished(); break;
    case 1 : mUploadModel->clearFinished(); break;
        //    case 2 : mDownloadModel->clearFinished(); break;


    }

}

void FSTaskWidget::removeSelectedTasks()
{


    switch (mTabWidget->currentIndex())
    {
    case 0 : mDownloadModel->removeTask(mDownloadView->currentIndex()); break;
    case 1 : mUploadModel->removeTask(mUploadView->currentIndex()); break;



    }
}

void FSTaskWidget::computeTasksCount()
{

    int count = mDownloadModel->count() + mUploadModel->count();
    qDebug()<<"COMPUTE COUNT..."<<count;

    emit countChanged(count);
}

void FSTaskWidget::downloadClicked(const QModelIndex &index)
{


    QDesktopServices::openUrl("file:///"+index.data().toString());

}


