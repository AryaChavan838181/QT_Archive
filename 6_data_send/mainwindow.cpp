#include "mainwindow.h"
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QDebug>
#include <QByteArray>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    // Create scene and view
    auto *view = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    view->setScene(scene);

    // Add background
    backgroundItem = new QGraphicsPixmapItem(QPixmap(":/images/background.png"));
    scene->addItem(backgroundItem);

    // Add needle
    needleItem = new QGraphicsPixmapItem(QPixmap(":/images/needle.png"));
    needleItem->setTransformationMode(Qt::SmoothTransformation);
    needleItem->setPos(400 - needleItem->pixmap().width() / 2, 240); // Center needle base
    needleItem->setTransformOriginPoint(needleItem->pixmap().width() / 2, 200); // Set rotation point
    scene->addItem(needleItem);

    // Set up serial port
    serialPort = new QSerialPort(this);
    serialPort->setPortName("COM10");
    serialPort->setBaudRate(QSerialPort::Baud115200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (serialPort->open(QIODevice::ReadOnly)) {
        qDebug() << "Connected to COM10 successfully!";
    } else {
        qDebug() << "Failed to connect to COM10";
    }

    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::readSerialData);

    // Create labels
    rpmLabel = new QLabel("RPM: 0", this);
    lvLabel = new QLabel("LV: 0.0 V", this);
    hvLabel = new QLabel("HV: 0.0 V", this);
    motorTempLabel = new QLabel("Motor Temp: 0 °C", this);
    accuTempLabel = new QLabel("Accu Temp: 0 °C", this);

    // Layout for labels
    auto *layout = new QVBoxLayout();
    layout->addWidget(rpmLabel);
    layout->addWidget(lvLabel);
    layout->addWidget(hvLabel);
    layout->addWidget(motorTempLabel);
    layout->addWidget(accuTempLabel);

    auto *container = new QWidget(this);
    container->setLayout(layout);
    setCentralWidget(view);
    view->setLayout(layout);
}

MainWindow::~MainWindow() {
    if (serialPort->isOpen()) {
        serialPort->close();
    }
}

void MainWindow::readSerialData() {
    while (serialPort->canReadLine()) {
        QByteArray data = serialPort->readLine().trimmed();
        QList<QByteArray> values = data.split(',');

        // Ensure 6 values are received
        if (values.size() != 6) {
            qDebug() << "Invalid data received: " << data;
            return;
        }

        // Parse values
        bool ok;
        int rpm = values[0].toInt(&ok); if (!ok) return;
        double lv = values[1].toDouble(&ok); if (!ok) return;
        double hv = values[2].toDouble(&ok); if (!ok) return;
        int motorTemp = values[3].toInt(&ok); if (!ok) return;
        int accuTemp = values[4].toInt(&ok); if (!ok) return;
        int speed = values[5].toInt(&ok); if (!ok) return;

        // Update dashboard
        updateNeedle(speed);
        updateDashboard(rpm, lv, hv, motorTemp, accuTemp);
    }
}

void MainWindow::updateDashboard(int rpm, double lv, double hv, int motorTemp, int accuTemp) {
    rpmLabel->setText(QString("RPM: %1").arg(rpm));
    lvLabel->setText(QString("LV: %1 V").arg(lv, 0, 'f', 1));
    hvLabel->setText(QString("HV: %1 V").arg(hv, 0, 'f', 1));
    motorTempLabel->setText(QString("Motor Temp: %1 °C").arg(motorTemp));
    accuTempLabel->setText(QString("Accu Temp: %1 °C").arg(accuTemp));
}

void MainWindow::updateNeedle(int speed) {
    double angle = -90 + (speed / 100.0) * 180;
    needleItem->setRotation(angle);
}
