#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;

    void stepAlgo(double xBeg,double yBeg,double xEnd,double yEnd);
    void brezenhemAlgo(double xBeg,double yBeg,double xEnd,double yEnd);
    void DDAalgo(double xBeg,double yBeg,double xEnd,double yEnd);
    void brezenhamAlgoForCircle(double x0,double y0,double radius);
};
#endif // WIDGET_H
