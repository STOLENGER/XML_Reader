#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "workwithxml.h"

#include <iostream>
#include <QMenu>
#include <QFileDialog>
#include <QtXmlPatterns>
#include <QErrorMessage>
#include <QTabWidget>
#include <QString>



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dock = new QDockWidget("Query answer", this);
    text = new QTextEdit;

    dock->resize(450,600);
    dock->hide();
    MenuActive=new QMenu("Active");
    MenuActive->addAction("&Сделать активным файл",this,SLOT(Active()));
    MenuActive->addAction("&Закрыть файл",this,SLOT(CloseFile()));

    QMenu *Menu=new QMenu("Menu");
    Menu->addAction("&Open File",this,SLOT(OpenFile()));
    Menu->addAction("&Close File",this,SLOT(ClearTree()));
    Menu->addAction("&Закрыть все",this,SLOT(ClearAll()));
    Menu->addSeparator();
    Menu->addAction("&Выаолнить запрос XQuery",this,SLOT(OpenQuery()));
    Menu->addSeparator();
    Menu->addAction("&Exit",this,SLOT(CloseWindow()),Qt::Key_Escape);
    ui->menubar->addMenu(Menu);
    //ui->menubar->addMenu(MenuActive);

    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(Perform()));
    connect(ui->pushButton_OpenXML,SIGNAL(clicked()),this,SLOT(OpenFile()));
    connect(ui->pushButton_OpenXQ,SIGNAL(clicked()),this,SLOT(OpenQuery()));
    connect(ui->pushButton_Clear,SIGNAL(clicked()),this,SLOT(ClearAll()));

    mainLayout=new QGridLayout;
    mainLayout->addWidget(ui->treeView,0,0,3,2);
    mainLayout->addWidget(ui->textEdit,4,0,3,1);
    mainLayout->addWidget(ui->pushButton,4,1,1,1);
    mainLayout->addWidget(ui->pushButton_OpenXML,5,1,1,1);
    mainLayout->addWidget(ui->pushButton_OpenXQ,6,1,1,1);
   // mainLayout->addWidget(ui->pushButton_Clear,7,1,1,1);
    mainWidget=new QWidget;
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
    setWindowTitle("XML Parser");

    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(MenuRequested()));

    //connect( ui->treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotEditRecord()));
    //connect( ui->treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));
    //MenuActive->popup(ui->treeView->mapToGlobal(pos()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete pointerFile;
}


void MainWindow::OpenFile()
{
    QString file_way=QFileDialog::getOpenFileName(this,"Select a xml file","/Users/waff.223/xml_parsing/File_with_XML","*.xml");
    if(file_way.isEmpty())
        return;
    QString file_name=file_way.section("/",-1,-1);
    QFile file(file_way);
    QXmlSimpleReader xmlReader;
    AddressBookParser helper(ui->treeView,file_name);
    QXmlInputSource source(&file);
    xmlReader.setContentHandler(&helper);
    if(!xmlReader.parse(source))
    {
        (new QErrorMessage(this))->showMessage("Can not parse file");
    }
    if(!pointerFile)
    {
        pointerFile = new QFile(file_way);
        tree_pos = ui->treeView->itemAt(0, 0);
        tree_pos->setFont(0, QFont("Helvetica [Cronyx]", 14, QFont::Bold));
    }

}


void MainWindow::CloseFile()
{
QString newFileName =  pointerFile->fileName();
newFileName.resize(pointerFile->fileName().lastIndexOf('/') + 1);

    QModelIndex model;
    if(ui->treeView->currentIndex().row())
        model = ui->treeView->model()->index(ui->treeView->currentIndex().row() - 1,ui->treeView->currentIndex().column());
    else
        model = ui->treeView->model()->index(ui->treeView->currentIndex().row() + 1,ui->treeView->currentIndex().column());

    QString ActiveFile(ui->treeView->currentItem()->text(0));
    delete ui->treeView->currentItem();
     pointerFile->close();
    if(model.isValid())
    {
        ui->treeView->setCurrentIndex(model);
           newFileName += ui->treeView->currentItem()->data(0,0).toString();
        if(pointerFile->fileName().contains(ActiveFile))
        {
               pointerFile->setFileName(newFileName);
            ui->treeView->currentItem()->setFont(0, QFont("Helvetica [Cronyx]", 14, QFont::Bold));
        }
        tree_pos = ui->treeView->currentItem();
    }
    else
    {
        delete pointerFile;
        pointerFile=nullptr;
    }

}

void MainWindow::Active()
{
    //tree_pos = ui->treeView->itemAt(0, 0);
    //tree_pos->setFont(0, QFont("Helvetica [Cronyx]", 12));
    QString FileName =  pointerFile->fileName();
    FileName.resize(pointerFile->fileName().lastIndexOf('/') + 1);
    FileName += ui->treeView->currentItem()->data(0,0).toString();
    pointerFile->setFileName(FileName);
    ui->treeView->currentItem()->setFont(0, QFont("Helvetica [Cronyx]", 14, QFont::Bold));
    tree_pos = ui->treeView->currentItem();
}

void MainWindow::OpenQuery() {
    QString filepath = QFileDialog::getOpenFileName(this, "Select a query-file", "/Users/waff.223/xml_parsing/File_with_XML", "*.xq");
    if (filepath.isEmpty())
        return;
    QFile file(filepath);
    if (file.open(QIODevice::ReadOnly)) {
        QString query = file.readAll();
        file.close();
        ui->textEdit->setText(query);
        Perform();
    } else {
        (new QErrorMessage(this))->showMessage("Cannot open file with query!");
        return;
    }
}


void MainWindow::Perform() {
    QString query = ui->textEdit->toPlainText();
    if(query!=""){
    QXmlQuery xml_query;
   //query.replace(QString(pointerFile->fileName().right(pointerFile->fileName().size()-pointerFile->fileName().lastIndexOf("/")-1)),pointerFile->fileName());
    query.replace("filename",pointerFile->fileName());
    xml_query.setQuery(query);
    if (!xml_query.isValid()) {
        (new QErrorMessage(this))->showMessage("The query is not valid!");
        return;
    }
    QString answer;
    if (!xml_query.evaluateTo(&answer)) {
        (new QErrorMessage(this))->showMessage("Cannot evaluate the query!");
        return;
    }
    text->setText(answer);
    dock->setWidget(text);
    dock->setFloating(true);
    dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
    dock->setAllowedAreas(Qt::NoDockWidgetArea);
    dock->resize(700,700);
    dock->show();
}else{OpenQuery();}

}

void MainWindow::ClearTree()
{
    ui->treeView->clear();
    delete pointerFile;
    pointerFile=nullptr;

}

void MainWindow::CloseWindow()
{
    QApplication::quit();
}

void MainWindow::ClearAll()
{
    ClearTree();
    ui->textEdit->clear();


}
void MainWindow::MenuRequested()
{
    if(ui->treeView->currentItem() && !ui->treeView->currentItem()->parent())
        MenuActive->exec(ui->treeView->mapToGlobal(QPoint(0,0)));
}




