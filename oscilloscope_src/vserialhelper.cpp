#include "vserialhelper.h"

#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QDateTime>

#pragma execution_character_set("utf-8")

VSerialHelper::VSerialHelper(QObject* parent)
    : QObject{parent}, period_data_size(0), data_size(0), serial(nullptr) {
    cache_data.clear();
    flag_first = true;
    file_debug.setFileName("/home/zd/桌面/oscilloscope_src/points.txt");
}

int VSerialHelper::open(const QString& portName, unsigned int baudRate) {
    close();
    file_debug.open(QIODevice::WriteOnly);
    qDebug() << "串口打开" << portName << baudRate;
    serial = new QSerialPort(this);

    // 设置串口参数
    serial->setPortName(portName);
    serial->setBaudRate(baudRate);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->clear(QSerialPort::AllDirections);

    // 打开串口
    if (serial->open(QIODevice::ReadOnly)) {
        qDebug() << "串口已打开" << serial->portName();
        serial->clear(QSerialPort::AllDirections);

        // 读取串口数据
        QObject::connect(serial, &QSerialPort::readyRead, [this]() {
            QByteArray data = serial->readAll();
                        // qDebug() << "收到数据：" << data ;
            int r_len = data.size();
//            file_debug.write(data);
            data_size += r_len;
            period_data_size += r_len;
            QByteArray debug_data;

            for (size_t i = 0; i < r_len; i++) {
                unsigned char ch = (unsigned char)data.at(i);
                if (ch == 0xa0) {
                    if (cache_data.isEmpty())
                        continue;
                    if (cache_data.size() != 6) {
//                        qDebug() << "errrrrrrrrrrrrrrrrrrr " << cache_data.size();
//                        cache_data.clear();
                        cache_data.append(ch);
                        continue;
                    }


                    int a = (int)(((unsigned char)cache_data.at(0) << 8) + (unsigned char)cache_data.at(1));
                    int b = (int)(((unsigned char)cache_data.at(2) << 8) + (unsigned char)cache_data.at(3));
                    int c = (int)(((unsigned char)cache_data.at(4) << 8) + (unsigned char)cache_data.at(5));
                    //                                qDebug() << a;
                    if(flag_first){
                        start = std::chrono::steady_clock::now(); // 记录开始时间
                        flag_first = false;
                    }
                    auto now = std::chrono::steady_clock::now(); // 获取当前时间
                    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - start).count();
                    double time_recorded = elapsed;
                    debug_data.append(QString::number(time_recorded, 'f', 0));
                    debug_data.append(',');
                    debug_data.append(QString::number(a));
                    debug_data.append(',');
                    debug_data.append(QString::number(b));
                    debug_data.append(',');
                    debug_data.append(QString::number(c));
                    debug_data.append('\n');

                    file_debug.write(debug_data);


                    data_time.emplace_back(time_recorded);

                    data_vec.emplace_back(a, b, c);
                    cache_data.clear();
                } else {
                    cache_data.append(ch);
                }
            }
        });
    } else {
        qDebug() << "无法打开串口";
        return -1;
    }
    return 0;
}

void VSerialHelper::close() {
    if (serial) {
        serial->close();
        serial->deleteLater();
        serial = nullptr;
    }
    data_size = 0;
    period_data_size = 0;
    file_debug.close();
}

VSerialHelper::~VSerialHelper() {
    close();
}
