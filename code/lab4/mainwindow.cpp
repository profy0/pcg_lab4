#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    n = 20;

    ui->tableWidget->setColumnCount(n);
    ui->tableWidget->setRowCount(n);

    ui->spinBox->setValue(20);
    ui->spinBox->setMaximum(20);
    ui->spinBox->setMinimum(0);
    ui->spinBox_2->setMaximum(10);
    ui->spinBox_2->setValue(5);

    for(int i=0;i<n;i++) {
        ui->tableWidget->setColumnWidth(i, ui->tableWidget->width() / n );
        ui->tableWidget->setRowHeight(i, ui->tableWidget->height() / n );
    }

    firstDot = new QTableWidgetItem;
    line = new QTableWidgetItem;

    firstDot->setBackground(QBrush(Qt::lightGray));
    line->setBackground(QBrush(Qt::gray));

    countOfDots = 0;
    type = 0;

    emit on_pushButton_clicked();

}



void MainWindow::calcLine(int x2, int y2)
{
    if(abs(y2 - y) < abs(x2 - x)) {
        if(x > x2) {
            calcLowLine(x2, y2, x, y);
        } else {
            calcLowLine(x, y, x2, y2);
        }
    } else {
        if(y > y2) {
            calcHighLine(x2, y2, x, y);
        } else {
            calcHighLine(x, y, x2, y2);
        }
    }
}

void MainWindow::calcLowLine(int x, int y, int x2, int y2)
{
    int dx = x2 - x;
    int dy = y2 - y;
    int py = 1;
    if(dy<0) {
        py = -1;
        dy = -dy;
    }

    int error = 2 * dy - dx;
    int yy = y;
    for(int xx=x;xx<=x2;xx++) {
        dots.push_back({xx,yy});
        if(error>0) {
            yy = yy + py;
            error += 2 * (dy - dx);
        } else {
            error += 2 * dy;
        }
    }
}

void MainWindow::calcHighLine(int x, int y, int x2, int y2)
{
    int dx = x2 - x;
    int dy = y2 - y;
    int px = 1;
    if(dx<0) {
        px = -1;
        dx = -dx;
    }

    int error = 2 * dx - dy;
    int xx = x;
    for(int yy=y;yy<=y2;yy++) {
        dots.push_back({xx,yy});
        if(error>0) {
            xx = xx + px;
            error += 2 * (dx - dy);
        } else {
            error += 2 * dx;
        }
    }
}

void MainWindow::setNumbers()
{
    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++) {
            QTableWidgetItem *now = ui->tableWidget->item(i,j);
            now->setText("(" + QString::number(i-n/2+1) +  "," + QString::number(j-n/2+1) + ")");
            now->setTextAlignment(Qt::AlignBottom | Qt::AlignHCenter);
            QFont f;
            f.setPointSize(6);
            now->setFont(f);
            ui->tableWidget->setItem(i,j, new QTableWidgetItem(*now));
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::drawCircle(int x0, int y0, int radius) {
    int x = 0;
    int y = radius;
    int delta = 1 - 2 * radius;
    int error = 0;
    while(y >= 0) {
        dots.push_back({x0 + x, y0 + y});
        dots.push_back({x0 + x, y0 - y});
        dots.push_back({x0 - x, y0 + y});
        dots.push_back({x0 - x, y0 - y});
        error = 2 * (delta + y) - 1;
        if(delta < 0 && error <= 0) {
            ++x;
            delta += 2 * x + 1;
            continue;
        }
        error = 2 * (delta - x) - 1;
        if(delta > 0 && error > 0) {
            --y;
            delta += 1 - 2 * y;
            continue;
        }
        ++x;
        delta += 2 * (x - y);
        --y;
    }
}

int round(float n)
{
    if (n - (int)n < 0.5)
        return (int)n;
    return (int)(n + 1);
}

void MainWindow::DDALine(int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int step;
    if (abs(dx) > abs(dy))
        step = abs(dx);
    else
        step = abs(dy);
    float x_incr = (float)dx / step;
    float y_incr = (float)dy / step;

    float x = x0;
    float y = y0;

    for (int i = 0; i < step; i++) {
        dots.push_back({round(x),round(y)});
        x += x_incr;
        y += y_incr;
    }
}

void MainWindow::iteration(int x0, int y0, int x1, int y1)
{

    if(abs(x0-x1)>abs(y0-y1)) {
        if(x0>x1) {
                std::swap(x0, x1);
                std::swap(y0, y1);
        }

        double a = 0;

        if(x0-x1!=0) a = (double)(y0 - y1) / (double)(x0 - x1);
       // float b = float(y0*x1 - x0*y1) / float(x1 - x0);
        double b = double(y1) - a * x1;

        while(x0<=x1) {
            dots.push_back({x0,y0});
            x0++;y0 = round((double)a * x0 + b);

        }
    } else {
        if(y0>y1) {
                std::swap(x0, x1);
                std::swap(y0, y1);
        }

        double a = 0;

        if(x0-x1!=0) a = (double)(y0 - y1) / (double)(x0 - x1);
       // float b = float(y0*x1 - x0*y1) / float(x1 - x0);
        double b = double(y1) - a * x1;

        while(y0<=y1) {
            dots.push_back({x0,y0});
            y0++;
            if(x0-x1!=0) x0 = round((double)(double(y0 - b) / a));

        }
    }



}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    if(countOfDots >= 2) return;
    if(countOfDots == 0) {
        ui->tableWidget->setItem(row, column, new QTableWidgetItem(*firstDot));
        x = row;
        y = column;
        countOfDots++;
        if(type == 1) {
            drawCircle(x,y,circleSize);
            countOfDots = 2;
            for(auto now : dots) {
                if(now.second<ui->tableWidget->columnCount() && now.second >= 0)
                ui->tableWidget->setItem(now.first, now.second, new QTableWidgetItem(*line));
            }
        }
        setNumbers();
        return;
    }

    dots.push_back({x,y});
    dots.push_back({row, column});
    if(type == 0) calcLine(row, column);
    if(type == 2) iteration(x,y,row, column);
    if(type == 3) DDALine(x, y, row, column);
    qDebug() << ui->tableWidget->columnCount();
    for(auto now : dots) {
        qDebug() << now << "\n";
       // if(now.second<ui->tableWidget->columnCount())
        ui->tableWidget->setItem(now.first, now.second, new QTableWidgetItem(*line));
    }

    countOfDots++;
    setNumbers();

}

void MainWindow::on_pushButton_clicked()
{
    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++) {
            ui->tableWidget->setItem(i,j, new QTableWidgetItem());
        }
    }
    dots.clear();
    countOfDots = 0;
    setNumbers();
}

void MainWindow::on_action_triggered()
{
    type = 0;
    ui->label->setText("Bresengham");
    emit on_pushButton_clicked();
}

void MainWindow::on_action_2_triggered()
{
    type = 1;
    ui->label->setText("Bresengham circle");
    emit on_pushButton_clicked();
}

void MainWindow::on_action_3_triggered()
{
    type = 2;
    ui->label->setText("Iteration");
    emit on_pushButton_clicked();
}

void MainWindow::on_action_4_triggered()
{
    type = 3;
    ui->label->setText("DDA");
    emit on_pushButton_clicked();
}

void MainWindow::paintEvent(QPaintEvent *e) {

}

void MainWindow::on_spinBox_textChanged(const QString &arg1)
{

    n = ui->spinBox->text().toInt();

    ui->tableWidget->setColumnCount(n);
    ui->tableWidget->setRowCount(n);

    for(int i=0;i<n;i++) {
        ui->tableWidget->setColumnWidth(i, ui->tableWidget->width() / n );
        ui->tableWidget->setRowHeight(i, ui->tableWidget->height() / n );
    }

    emit on_pushButton_clicked();
}

void MainWindow::on_spinBox_2_textChanged(const QString &arg1)
{
    circleSize = ui->spinBox_2->text().toInt();
    emit on_pushButton_clicked();
}
