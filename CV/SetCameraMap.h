// 设置相机映射

#ifndef QTFRAME_SETCAMERAMAP_H
#define QTFRAME_SETCAMERAMAP_H

#include "Main/DetectionBase.h"
#include "map"

class SetCameraMap {
public:
    SetCameraMap();
    ~SetCameraMap();

    enum Camear_num {
        camera0, camera1, camera2, camera3, camera4, camera5
    };

    std::map<int, DetectionBase *> camera_dict;

    void set_camera_dict();

};

#endif //QTFRAME_SETCAMERAMAP_H
