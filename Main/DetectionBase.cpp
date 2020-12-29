#include "DetectionBase.h"

DetectionBase::~DetectionBase() {
}


DetectionBase::DetectionBase(int cam_num) {
    defErrorType();
    defineParams();
}

//定义错误类型
void DetectionBase::defErrorType() {
    // Todo 一、定义错误类型


    error_type_dict[-1] = "错误1";
    error_type_dict[-2] = "错误2";
    error_type_dict[-3] = "错误3";
}

//定义参数: 参数名:[最大值,最小值,当前值]
void DetectionBase::defineParams() {
    // Todo 二、定义界面参数: ('参数名' :[最小值, 最大值, 当前值])
    params_vector.push_back({"默认 1", 0, 2, 100});
    params_vector.push_back({"哈哈哈", 0, 18, 100});
    params_vector.push_back({"param", 0, 2, 100});
}

// Todo 四、根据界面参数 声明变量.
int val_1;
int val_2;
int val_3;
int val_4;
int val_5;
int val_6;


void DetectionBase::updateArgs(int *value_p) {
    // Todo 五、映射变量.

    val_1 = value_p[0];
    val_2 = value_p[1];
    val_3 = value_p[2];
}

// int processing(Mat &img) {
//     // Todo 六、此处写图像算法.  提示:训练区域坐标为 Point类型的 Point_A(左上) 和 Point_B(右下)
//
//     cvtColor(img, img, cv::COLOR_BGR2GRAY);
//
//     return 0;
// }