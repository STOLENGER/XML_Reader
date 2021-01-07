#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QDockWidget>
#include <QTextEdit>
#include <QTreeWidget>

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
    QString file_way;
private slots:
    void OpenFile();
    void OpenQuery();
    void ExecuteQuery();
    void ClearTree();
    void CloseWindow();
};
#endif // MAINWINDOW_H
