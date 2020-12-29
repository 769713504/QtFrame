#include "SetCameraMap.h"
#include "iostream"
#include "default.h"

void SetCameraMap::set_camera_dict() {
    camera_dict[camera0] = new DetectionBase;
    camera_dict[camera1] = new CameraDefault;
    // camera_dict[camera2] = new CameraDefault;
    // camera_dict[camera3] = new CameraDefault;
    // camera_dict[camera4] = new CameraDefault;
    // camera_dict[camera5] = new CameraDefault;
}

SetCameraMap::~SetCameraMap() {
}

SetCameraMap::SetCameraMap() {
    set_camera_dict();
}
