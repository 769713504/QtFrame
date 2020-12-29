#ifndef QTFRAME_DETECTIONBASE_H
#define QTFRAME_DETECTIONBASE_H

#include <map>
#include <QString>

struct ParamsStruct {
    std::string name;
    int min;
    int value;
    int max;

};


// 图像检测基类
class DetectionBase {
public:

    DetectionBase(int cam_num = -1);

    ~DetectionBase();


    int x1, y1, x2, y2;

    std::map<int, QString> error_type_dict;
    std::vector<ParamsStruct> params_vector;

    // 定义错误类型
    void defErrorType();

    //定义参数字典
    void defineParams();

    //更新参数
    void updateArgs(int value_p[]);
};

#endif //QTFRAME_DETECTIONBASE_H
