// VibrationAnalyzer.h

#ifndef VIBRATIONANALYZER_H
#define VIBRATIONANALYZER_H

#include <string>
#include <vector>

struct DataPoint {
    double timestamp;
    double displacement;
};

class VibrationAnalyzer {
private:
    std::vector<DataPoint> data;
    double threshold; // 用于筛选有效振动的位移量变化阈值

public:
    VibrationAnalyzer(double thresh = 0.01);
    void loadData(const std::string& filePath);
    std::pair<double, double> analyzeVibration() const;
};

#endif // VIBRATIONANALYZER_H
