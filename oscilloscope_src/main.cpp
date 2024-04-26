#include "mainwindow.h"
#include "vibrationanalyzer.h"
#include <QApplication>
#include <iostream>
using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();




//    VibrationAnalyzer analyzer(0.01);
//    analyzer.loadData("/home/zd/桌面/oscilloscope_src/points_displacement.txt"); // 替换为你的文件路径

//    auto [amplitude, frequency] = analyzer.analyzeVibration();

//    std::cout << "Amplitude: " << amplitude << " mm" << std::endl;
//    std::cout << "Frequency: " << frequency << " Hz" << std::endl;

//    return 0;

}
