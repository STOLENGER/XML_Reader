#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "workwithxml.h"

#include <QMenu>
#include <QFileDialog>
#include <QtXmlPatterns>
#include <QErrorMessage>
#include <QTabWidget>
#include <QString>
#include <QTreeWidget>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dock = new QDockWidget("Query answer", this, Qt::Popup);
    text = new QTextEdit;

    dock->resize(450,600);
    dock->hide();

    QMenu *Menu=new QMenu("Menu");
    Menu->addAction("&Open File",this,SLOT(OpenFile()));
    Menu->addAction("&Close File",this,SLOT(ClearTree()));
    Menu->addSeparator();
    Menu->addAction("&Execute XQuery",this,SLOT(OpenQuery()));
    Menu->addSeparator();
    Menu->addAction("&Exit",this,SLOT(CloseWindow()),Qt::Key_Escape);
    ui->menubar->addMenu(Menu);

    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(ExecuteQuery()));
    connect(ui->pushButton_OpenXML,SIGNAL(clicked()),this,SLOT(OpenFile()));
    connect(ui->pushButton_OpenXQ,SIGNAL(clicked()),this,SLOT(OpenQuery()));

    mainLayout=new QGridLayout;
    mainLayout->addWidget(ui->treeView,0,0,3,2);
    mainLayout->addWidget(ui->textEdit,4,0,3,1);
    mainLayout->addWidget(ui->pushButton,4,1,1,1);
    mainLayout->addWidget(ui->pushButton_OpenXML,5,1,1,1);
    mainLayout->addWidget(ui->pushButton_OpenXQ,6,1,1,1);
    mainWidget=new QWidget;
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
    setWindowTitle("XML Parser");

}

MainWindow::~MainWindow()
{
    delete ui;
    delete mainLayout;
    delete mainWidget;
    delete dock;
    delete text;
}


void MainWindow::OpenFile()
{
    ClearTree();
    file_way=QFileDialog::getOpenFileName(this,"Select a xml file","/Users/waff.223/xml_parsing/File_with_XML","*.xml");
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
        ExecuteQuery();
    } else {
        (new QErrorMessage(this))->showMessage("Cannot open file with query!");
        return;
    }
}


void MainWindow::ExecuteQuery() {
    QString query = ui->textEdit->toPlainText();
    QXmlQuery xml_query;
    xml_query.setQuery("doc(\""+file_way+'\"'+")"+query);
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
    dock->show();

}

void MainWindow::ClearTree()
{
    ui->treeView->clear();
}

void MainWindow::CloseWindow()
{
    QApplication::quit();
}

