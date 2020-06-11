#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace QtCharts;

//Todo: Put these as class objects
QLineSeries *series;
QChart *chart;
QChartView *chartView;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    series = new QLineSeries();
    for(int i = 0; i<8; i++)
        series->append(i,i);

    chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Sensor evolution");

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    //ui->graphLayout->addWidget(chartView);
}

MainWindow::~MainWindow()
{
    delete ui;
}


/*void MainWindow::on_pushButton_clicked()
{
    chart->removeSeries(series); // Clear the chart
    QFile file("data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;


}*/
