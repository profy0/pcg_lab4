#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QTableWidgetItem>
#include <vector>
#include <QTableWidget>

class QTableWidget;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void calcLine(int, int);
    void calcLowLine(int, int, int, int);
    void calcHighLine(int, int, int, int);
    void setNumbers();
    void drawCircle(int, int, int);
    void DDALine(int, int, int, int);
    void iteration(int, int, int, int);
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent*) override;

private slots:
    void on_tableWidget_cellClicked(int row, int column);

    void on_pushButton_clicked();

    void on_action_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_action_4_triggered();

    void on_spinBox_textChanged(const QString &arg1);

    void on_spinBox_2_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    int countOfDots;
    QTableWidgetItem *firstDot;
    QTableWidgetItem *line;
    int x,y;
    std::vector <std::pair<int,int> > dots;
    int type;
    int n;
    int circleSize;
};
#endif // MAINWINDOW_H
