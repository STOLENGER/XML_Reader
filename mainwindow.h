#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QDockWidget>
#include <QTextEdit>
#include <QTreeWidget>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGridLayout* mainLayout;
    QWidget* mainWidget;
    QDockWidget* dock;
    QTextEdit* text;
    QFile* pointerFile=nullptr;
    QTreeWidgetItem* tree_pos;
    QMenu *MenuActive;
private slots:
    void OpenFile();
    void OpenQuery();
    void Perform();
    void ClearTree();
    void CloseWindow();
    void ClearAll();
    void Active();
    void CloseFile();
    void MenuRequested();

};
#endif // MAINWINDOW_H
