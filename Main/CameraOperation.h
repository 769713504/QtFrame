#ifndef QTFRAME_CAMERAOPERATION_H
#define QTFRAME_CAMERAOPERATION_H


#include "MvInclude/MvCameraControl.h"
#include <cstring>
#include <windows.h>
#include <QLabel>
#include <QPixmap>
#include "synchapi.h"

#ifndef MV_NULL
#define MV_NULL    0
#endif


struct CAMERA_PARAMS_LIST {
    int cam_num;                                    //相机编号
    MV_CC_DEVICE_INFO_LIST *device_info_list;       //设备信息列表
    QLabel *label_main;                             //主标签
    int *label_main_control_array;                  //主标签控制数组

};


//相机控制类
class CMvCamera {
public:

    // Todo/////////////////////////////////////////////声明变量/////////////////////////////////////////////////////////

    int *label_main_control_array;
    MV_CC_PIXEL_CONVERT_PARAM bgr_image_param{};
    unsigned char *bgr_data;
    unsigned int nDstLen{};
    unsigned int bgr_data_size;
    int *show_image_control_array[6];
    QImage q_rgb_image;


    MV_CC_DEVICE_INFO *device_info;
    unsigned char *m_pGrabBuf{};
    unsigned int m_nGrabBufSize{0};
    bool is_open{false};
    bool is_grabbing{false};
    //主窗口宽高
    int label_main_width;
    int label_main_height;
    int cam_num, ret;
    QLabel *label_main;
    //保存图像的互斥锁
    CRITICAL_SECTION m_hSaveImageMux{};

    //输出帧的信息
    MV_FRAME_OUT_INFO_EX m_stImageInfo;
    //触发模式
    bool m_nTriggerMode{false};
    //显示句柄
    HWND m_hwndDisplay;

    // Todo//////////////////////////////////////////声明函数/////////////////////////////////////////////////////////////

    //构造函数
    CMvCamera(CAMERA_PARAMS_LIST *camera_params);

    //析构
    ~CMvCamera();

    //图像处理流程
    void ProcessFlow();

    //向标签中显示彩图
    void ShowImageToMainLabel();

    //去除自定义的像素格式
    bool RemoveCustomPixelFormats(enum MvGvspPixelType enPixelFormat);

    //获取SDK版本号 
    static int GetSDKVersion();

    //枚举设备 
    static int EnumDevices(unsigned int nTLayerType, MV_CC_DEVICE_INFO_LIST *pstDevList);

    //判断设备是否可达 
    static bool IsDeviceAccessible(MV_CC_DEVICE_INFO *pstDevInfo, unsigned int nAccessMode);

    //取流线程
    static void GrabThread(CMvCamera *self);

    //处理
    int GrabThreadProcess();

    //打开设备 
    int Open();

    //关闭设备 
    int Close();

    //判断相机是否处于连接状态 
    bool IsDeviceConnected();

    //注册图像数据回调 
    int RegisterImageCallBack(void(__stdcall *cbOutput)(unsigned char *pData,
                                                        MV_FRAME_OUT_INFO_EX *pFrameInfo,
                                                        void *pUser), void *pUser);

    //开启抓图
    int StartGrabbing();

    //停止抓图 
    int StopGrabbing();

    //主动获取一帧图像数据 
    int GetImageBuffer(MV_FRAME_OUT *pFrame, int nMsec);

    //释放图像缓存 
    int FreeImageBuffer(MV_FRAME_OUT *pFrame);

    //主动获取一帧图像数据 
    int GetOneFrameTimeout(unsigned char *pData, unsigned int nDataSize, MV_FRAME_OUT_INFO_EX *pFrameInfo, int nMsec);

    //显示一帧图像 
    int DisplayOneFrame(MV_DISPLAY_FRAME_INFO *pDisplayInfo);

    //设置SDK内部图像缓存节点个数 
    int SetImageNodeNum(unsigned int nNum);

    //获取设备信息 
    int GetDeviceInfo(MV_CC_DEVICE_INFO *pstDevInfo);

    //获取GEV相机的统计信息 
    int GetGevAllMatchInfo(MV_MATCH_INFO_NET_DETECT *pMatchInfoNetDetect);

    //获取U3V相机的统计信息 
    int GetU3VAllMatchInfo(MV_MATCH_INFO_USB_DETECT *pMatchInfoUSBDetect);

    //获取和设置Int型参数，如 Width和Height，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
    int GetIntValue(IN const char *strKey, OUT MVCC_INTVALUE_EX *pIntValue);

    int SetIntValue(IN const char *strKey, IN int64_t nValue);

    //获取和设置Enum型参数，如 PixelFormat，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
    int GetEnumValue(IN const char *strKey, OUT MVCC_ENUMVALUE *pEnumValue);

    int SetEnumValue(IN const char *strKey, IN unsigned int nValue);

    int SetEnumValueByString(IN const char *strKey, IN const char *sValue);

    //获取和设置Float型参数，如 ExposureTime和Gain，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
    int GetFloatValue(IN const char *strKey, OUT MVCC_FLOATVALUE *pFloatValue);

    int SetFloatValue(IN const char *strKey, IN float fValue);

    //获取和设置Bool型参数，如 ReverseX，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
    int GetBoolValue(IN const char *strKey, OUT bool *pbValue);

    int SetBoolValue(IN const char *strKey, IN bool bValue);

    //获取和设置String型参数，如 DeviceUserID，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件UserSetSave
    int GetStringValue(IN const char *strKey, MVCC_STRINGVALUE *pStringValue);

    int SetStringValue(IN const char *strKey, IN const char *strValue);

    //执行一次Command型命令，如 UserSetSave，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
    int CommandExecute(IN const char *strKey);

    //探测网络最佳包大小(只对GigE相机有效) 
    int GetOptimalPacketSize(unsigned int *pOptimalPacketSize);

    //注册消息异常回调 
    int RegisterExceptionCallBack(void(__stdcall *cbException)(unsigned int nMsgType, void *pUser), void *pUser);

    //注册单个事件回调 
    int RegisterEventCallBack(const char *pEventName,
                              void(__stdcall *cbEvent)(MV_EVENT_OUT_INFO *pEventInfo, void *pUser),
                              void *pUser);

    //强制IP 
    int ForceIp(unsigned int nIP, unsigned int nSubNetMask, unsigned int nDefaultGateWay);

    //配置IP方式 
    int SetIpConfig(unsigned int nType);

    //设置网络传输模式 
    int SetNetTransMode(unsigned int nType);

    //像素格式转换 
    int ConvertPixelType(MV_CC_PIXEL_CONVERT_PARAM *pstCvtParam);

    //保存图片 
    int SaveImage(MV_SAVE_IMAGE_PARAM_EX *pstParam);

    //保存图片为文件 
    int SaveImageToFile(MV_SAVE_IMG_TO_FILE_PARAM *pstParam);

private:

    void *m_hDevHandle;

};

#endif//QTFRAME_CAMERAOPERATION_H

