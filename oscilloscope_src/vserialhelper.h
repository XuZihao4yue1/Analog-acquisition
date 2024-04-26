#ifndef VSERIALHELPER_H
#define VSERIALHELPER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <vector>
#include <QFile>

struct Vol {
    int a;
    int b;
    int c;
    Vol(int a, int b, int c)
        : a(a), b(b), c(c){};
};

class VSerialHelper : public QObject {
    Q_OBJECT
public:
    explicit VSerialHelper(QObject* parent = nullptr);
    ~VSerialHelper();

    unsigned int data_size;
    unsigned int period_data_size;
    std::vector<Vol> data_vec;
    std::vector<double> data_time; // 时间戳向量

    int open(const QString& portName, unsigned int baudRate);
    void close();

private:
    QByteArray cache_data;
    QSerialPort* serial;
    QFile file_debug;
    bool flag_first;
    std::chrono::time_point<std::chrono::steady_clock> start;

signals:
    void sglVoltageData(QByteArray);
};

#endif  // VSERIALHELPER_H
