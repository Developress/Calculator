#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void NewDigit();
    void onOperationClicked();
    void on_Dot_clicked();
    void on_clear_clicked();

    void on_Equal_clicked();

    void on_plusMinus_clicked();

    void on_clear_pressed();

    void on_procent_clicked();

private:
    Ui::MainWindow *ui;
    void executeOperation(int counter, char operation);
    double getLeftOperand(int index, int *startIndex);
    double getRightOperand(int index, int *startIndex);
    int getPositionOfOperation();
};
#endif // MAINWINDOW_H
