#include "widget.h"
#include "ui_widget.h"
#include<QLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    QVBoxLayout* vLayoutForBoxes = new QVBoxLayout(this);
    QHBoxLayout* hLayout = new QHBoxLayout(this);

    vLayoutForBoxes->addWidget(ui->beginCoordBox,2);
    vLayoutForBoxes->addWidget(ui->endCoordBox,2);
    vLayoutForBoxes->addWidget(ui->radiusBox,1);
    vLayoutForBoxes->addWidget(ui->pushButton,4);
    vLayoutForBoxes->addWidget(ui->label_4,1);

    hLayout->addWidget(ui->plotArea,6);
    hLayout->addLayout(vLayoutForBoxes,4),
    vLayout->addLayout(hLayout,6);
    vLayout->addWidget(ui->boxWithAlgos,1);

    QCPAxis *xAxis = ui->plotArea->xAxis;
    QCPAxis *yAxis = ui->plotArea->yAxis;

    xAxis->setRange(-10, 10);
    yAxis->setRange(-10, 10);

    xAxis->setLabel("Ось X");
    yAxis->setLabel("Ось Y");

    ui->plotArea->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    ui->plotArea->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    ui->plotArea->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);

    ui->stepAlgo->setChecked(true);

    QRegularExpression regex("-?\\d{1,7}(\\.\\d{1,2})?");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, this);

    ui->line->setValidator(validator);
    ui->line_2->setValidator(validator);
    ui->lineFor->setValidator(validator);
    ui->lineFor_2->setValidator(validator);
    ui->lineEdit->setValidator(validator);
    ui->lineEdit_2->setValidator(validator);
    ui->radEdit->setValidator(validator);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    qDebug()<<ui->xBeg->text();
    double xBegin = ui->line->text().toDouble();
    double yBegin = ui->line_2->text().toDouble();
    double xEnd = ui->lineFor_2->text().toDouble();
    double yEnd = ui->lineFor->text().toDouble();

    if(ui->Brezenhem_circle->isChecked()){
        double radius = ui->radEdit->text().toDouble();
        double x0 = ui->lineEdit->text().toDouble();
        double y0 = ui->lineEdit_2->text().toDouble();
        brezenhamAlgoForCircle(x0,y0,radius);
    }else if(ui->stepAlgo->isChecked()){
        stepAlgo(xBegin,yBegin,xEnd,yEnd);
    }else if(ui->Brezenhem->isChecked()){
        brezenhemAlgo(xBegin,yBegin,xEnd,yEnd);
    }else if(ui->DDA->isChecked()){
        DDAalgo(xBegin,yBegin,xEnd,yEnd);
    }
}

void Widget::stepAlgo(double x1, double y1, double x2, double y2)
{
    ui->plotArea->clearPlottables();
    QCPGraph *graph = ui->plotArea->addGraph();
    graph->setLineStyle(QCPGraph::lsNone);
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 10));
    graph->addData(round(x1),round(y1));
    graph->addData(round(x2),round(y2));

    QElapsedTimer timer;
    timer.start();

    double dx = x2 - x1;
    double dy = y2 - y1;
    double k = qAbs(dy / dx);
    double x = x1;
    double y = y1;

    if (qAbs(dx) >= qAbs(dy))
    {
        while (qAbs(x - x2) >= 1)
        {
            graph->addData(qRound(x), qRound(y));
            dx < 0 ? x-- : x++;
            dy < 0 ? y -= k : y += k;
        }
    }
    else
    {
        k = 1 / k;
        while (qAbs(y - y2) >= 1)
        {
            graph->addData(qRound(x), qRound(y));
            dy < 0 ? y-- : y++;
            dx < 0 ? x -= qAbs(k) : x += qAbs(k);
        }
    }

    qint64 elapsed = timer.elapsed();
    QString timeString = QString("Время работы алгоритма: %1 миллисекунд").arg(elapsed);
    ui->label_4->setText(timeString);
    ui->label_4->show();


    ui->plotArea->replot();
}

void Widget::brezenhemAlgo(double x1, double y1, double x2, double y2)
{
    ui->plotArea->clearPlottables();
    QCPGraph *graph = ui->plotArea->addGraph();
    graph->setLineStyle(QCPGraph::lsNone);
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 10));

    graph->addData(round(x1),round(y1));
    graph->addData(round(x2),round(y2));

    QElapsedTimer timer;
    timer.start();

    bool steep = qAbs(y2 - y1) > qAbs(x2 - x1);

    if (steep)
    {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if (x1 > x2)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    double dx = x2 - x1;
    double dy = qAbs(y2 - y1);
    double error = dx / 2;
    double yStep = (y1 < y2) ? 1 : -1;
    double y = y1;
    for (double x = x1; x <= x2; x++)
    {
        graph->addData(qRound(steep ? y : x), qRound(steep ? x : y));
        error -= dy;
        if (error <= 0)
        {
            y += yStep;
            error += dx;
        }
    }

    qint64 elapsed = timer.elapsed();
    QString timeString = QString("Время работы алгоритма: %1 миллисекунд").arg(elapsed);
    ui->label_4->setText(timeString);
    ui->label_4->show();


    ui->plotArea->replot();
}

void Widget::DDAalgo(double x1, double y1, double x2, double y2)
{
    ui->plotArea ->clearPlottables();

    ui->plotArea->clearPlottables();
    QCPGraph *graph = ui->plotArea->addGraph();
    graph->setLineStyle(QCPGraph::lsNone);
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 10));

    QElapsedTimer timer;
    timer.start();
    int xStart = round(x1),yStart = round(y1),xEnd = round(x2),yEnd = round(y2);
    int L = std::max(abs(xStart - xEnd),abs(yStart - yEnd));

    if(L == 0){
        graph->addData(xStart,yStart);
    }else{
        double x = x1,y = y1;
        graph->addData(x,y);
        for(int i = 2;i<=L+1;i++){
            double temp1 = x + (x2 - x1)/L;
            double temp2 = y + (y2 - y1)/L;
            graph->addData(round(temp1),round(temp2));
            x = temp1;
            y = temp2;
        }
    }

    qint64 elapsed = timer.elapsed();
    QString timeString = QString("Время работы алгоритма: %1 миллисекунд").arg(elapsed);
    ui->label_4->setText(timeString);
    ui->label_4->show();

    ui->plotArea->replot();
}

void Widget::brezenhamAlgoForCircle(double x0, double y0, double radius)
{
    ui->plotArea->clearPlottables();
    QCPGraph *graph = ui->plotArea->addGraph();
    graph->setLineStyle(QCPGraph::lsNone);
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 10));

    QElapsedTimer timer;
    timer.start();

    int x  = 0;
    int y = radius;
    int delta = 1 - 2 * radius;
    int error = 0;
    while (y >= x){
        graph->addData(round(x0 + x),round(y0+y));
        graph->addData(round(x0 + x),round(y0-y));
        graph->addData(round(x0 - x),round(y0+y));
        graph->addData(round(x0 - x),round(y0-y));
        graph->addData(round(x0 + y),round(y0+x));
        graph->addData(round(x0 + y),round(y0-x));
        graph->addData(round(x0 - y),round(y0+x));
        graph->addData(round(x0 - y),round(y0-x));
        error = 2 * (delta + y) - 1;
        if ((delta < 0) && (error <= 0)){
            delta += 2 * ++x + 1;
            continue;
        }
        if ((delta > 0) && (error > 0)){
            delta -= 2 * --y + 1;
            continue;
        }
        delta += 2 * (++x - --y);
    }
    qint64 elapsed = timer.elapsed();
    QString timeString = QString("Время работы алгоритма: %1 миллисекунд").arg(elapsed);
    ui->label_4->setText(timeString);
    ui->label_4->show();

    ui->plotArea->replot();
}
