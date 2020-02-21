#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plot/qcustomplot.h"
#include "ui_mainwindow.h"
#include "main/global.h"
#include "opencv2/opencv.hpp"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void addPoint(double x, int y); // 增加数据
    void clearData();     // 清楚数据
    void plot(); // 画曲线
    void setupRealtimeDataDemo(QCustomPlot *customPlot);

private slots:
    void realtimeDataSlot();
    void clickedGreph(QMouseEvent *event);
    void on_btn_stop_clicked();
    void on_btn_start_clicked();
public:
    double y1,y2;
private:
    Ui::MainWindow *ui;
    QString demoName;
    QTimer dataTimer;
    QVector<double> qv_x, qv_y, qv_t1, qv_t2,qv_x3,qv_x4,qv_t3,qv_t4;
    double programe_start_time;
    int history_num_ = 5000;
    int range_num_ = 5;
    bool mouse_flag_ = 0;
    bool stop_plot_flag_ = 0;

};



#endif // MAINWINDOW_H
