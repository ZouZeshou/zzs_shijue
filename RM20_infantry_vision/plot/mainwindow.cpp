#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setGeometry(200, 400, 1050, 500);
    setupRealtimeDataDemo(ui->plot);
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    connect(ui->plot, SIGNAL(mousePress(QMouseEvent*)), SLOT(clickedGreph(QMouseEvent*)));

    programe_start_time = cv::getTickCount();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addPoint(double x, int y)
{
    //cout<<"add point function work\r\n"<<endl;
    double t_tmp = cv::getTickCount();
    double t = (t_tmp - programe_start_time)/cv::getTickFrequency();

    switch (y) {
    case 0:
        qv_x.push_back(x);
        qv_t1.push_back(t);
        break;

    case 1:
        qv_y.push_back(x);
        qv_t2.push_back(t);
        break;
    case 2:
        qv_x3.push_back(x);
        qv_t3.push_back(t);
        break;
    case 3:
        qv_x4.push_back(x);
        qv_t4.push_back(t);
        break;
    case 4:
        qv_x5.push_back(x);
        qv_t5.push_back(t);
        break;

    case 5:
        qv_x6.push_back(x);
        qv_t6.push_back(t);
        break;
    case 6:
        qv_x7.push_back(x);
        qv_t7.push_back(t-0.4);
        break;
    case 7:
        qv_x8.push_back(x);
        qv_t8.push_back(t);
        break;

    }
    if(qv_x.size()>history_num_)
    {
        qv_x.pop_front();
        qv_t1.pop_front();
    }

    if(qv_y.size()>history_num_)
    {
        qv_y.pop_front();
        qv_t2.pop_front();
    }
    if(qv_x3.size()>history_num_)
    {
        qv_x3.pop_front();
        qv_t3.pop_front();
    }
    if(qv_x4.size()>history_num_)
    {
        qv_x4.pop_front();
        qv_t4.pop_front();
    }
    if(qv_x5.size()>history_num_)
    {
        qv_x5.pop_front();
        qv_t5.pop_front();
    }
    if(qv_x6.size()>history_num_)
    {
        qv_x6.pop_front();
        qv_t6.pop_front();
    }
    if(qv_x7.size()>history_num_)
    {
        qv_x7.pop_front();
        qv_t7.pop_front();
    }
    if(qv_x8.size()>history_num_)
    {
        qv_x8.pop_front();
        qv_t8.pop_front();
    }

    if(mouse_flag_ == 0)
    {
        ui->plot->xAxis->rescale(); // set Range
        ui->plot->yAxis->rescale(); // set Range
        //    ui->plot->graph(0)->rescaleValueAxis(false, true);
        ui->plot->xAxis->setRange(ui->plot->xAxis->range().upper, range_num_, Qt::AlignRight);  // move
    }
}

void MainWindow::plot()
{
    ui->plot->graph(0)->setData(qv_t1, qv_x);
    ui->plot->graph(1)->setData(qv_t2, qv_y);
    ui->plot->graph(2)->setData(qv_t3, qv_x3);
    ui->plot->graph(3)->setData(qv_t4, qv_x4);
    ui->plot->graph(4)->setData(qv_t5, qv_x5);
    ui->plot->graph(5)->setData(qv_t6, qv_x6);
    ui->plot->graph(6)->setData(qv_t7, qv_x7);
    ui->plot->graph(7)->setData(qv_t8, qv_x8);
    ui->plot->replot();
}

void MainWindow::clickedGreph(QMouseEvent *event)
{
    QPoint point = event->pos();
    double m_x = ui->plot->xAxis->pixelToCoord(point.x());
    double m_y = ui->plot->yAxis->pixelToCoord(point.y());
    qDebug()<< m_x<< m_y;

//    int button_flag = event->buttons();
//    QVector<double> qv_tmp;
//    if(button_flag == 1) // left button
//    {
//        mouse_flag_ = 1;
//    }else
//    {
//        mouse_flag_ = 0;
//    }
}

void MainWindow::on_btn_stop_clicked()
{
    stop_plot_flag_ = 1;
}

void MainWindow::on_btn_start_clicked()
{
    stop_plot_flag_ = 0;
}
void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
  demoName = "Real Time Data Demo";
  customPlot->addGraph(); // red line
  customPlot->graph(0)->setPen(QPen(QColor(255, 0, 0)));
  customPlot->addGraph(); // orange line
  customPlot->graph(1)->setPen(QPen(QColor(255, 165, 0)));
  customPlot->addGraph(); // yellow line
  customPlot->graph(2)->setPen(QPen(QColor(255, 255, 0)));
  customPlot->addGraph(); // green line
  customPlot->graph(3)->setPen(QPen(QColor(0, 255, 0)));
  customPlot->addGraph(); // cyan line
  customPlot->graph(4)->setPen(QPen(QColor(0, 255, 255)));
  customPlot->addGraph(); // blue line
  customPlot->graph(5)->setPen(QPen(QColor(0, 0, 255)));
  customPlot->addGraph(); // purpel line
  customPlot->graph(6)->setPen(QPen(QColor(128, 0, 128)));
  customPlot->addGraph(); // black line
  customPlot->graph(7)->setPen(QPen(QColor(0, 0, 0)));

  QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
  timeTicker->setTimeFormat("%h:%m:%s");
  customPlot->xAxis->setTicker(timeTicker);
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->yAxis->setRange(-250,250);

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}
void MainWindow::realtimeDataSlot()
{
  static QTime time(QTime::currentTime());

  double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
  static double lastPointKey = 0;
  if (key-lastPointKey > 0.02) // at most add point every 2 ms
  {
    if(!stop_plot_flag_)
    {
        addPoint(plot_y1,0);
        addPoint(plot_y2,1);
        addPoint(plot_y3,2);
        addPoint(plot_y4,3);
        addPoint(plot_y5,4);
        addPoint(plot_y6,5);
        addPoint(plot_y7,6);
        addPoint(plot_y8,7);
    }
    //cv::resize(plot_frame, plot_frame, Size(plot_frame.cols*0.5, plot_frame.rows*0.5));
    Mat src = plot_frame.clone();
    if(!src.empty())
    {
        cv::resize(src, src,Size(400,360));
        cvtColor(src, src, CV_BGR2RGB);//only RGB of Qt
    }

    QImage srcQImage = QImage((uchar*)(src.data), src.cols, src.rows, QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(srcQImage));
    ui->label->resize(srcQImage.size());
    ui->label->show();

    lastPointKey = key;
  }
  plot();
}
