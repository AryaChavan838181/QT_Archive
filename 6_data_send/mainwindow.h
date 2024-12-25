#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QSerialPort>
#include <QLabel>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void readSerialData();
    void updateDashboard(int rpm, double lv, double hv, int motorTemp, int accuTemp);
    void updateNeedle(int speed);

private:
    QGraphicsScene *scene;
    QGraphicsPixmapItem *backgroundItem;
    QGraphicsPixmapItem *needleItem;
    QSerialPort *serialPort;

    QLabel *rpmLabel;
    QLabel *lvLabel;
    QLabel *hvLabel;
    QLabel *motorTempLabel;
    QLabel *accuTempLabel;
};

#endif // MAINWINDOW_H
