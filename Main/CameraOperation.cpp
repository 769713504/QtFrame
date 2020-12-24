#include <thread>
#include "CameraOperation.h"
#include "iostream"


using namespace std;

CMvCamera::CMvCamera(MV_CC_DEVICE_INFO_LIST *device_info_list,
                     int cam_num_,
                     QLabel *label_main,
                     int *label_main_control_array) {
    InitializeCriticalSection(&m_hSaveImageMux);
    //相机句柄
    m_hDevHandle = MV_NULL;
    //
    device_info = device_info_list->pDeviceInfo[cam_num_];
    cam_num = cam_num_;
    this->label_main = label_main;
    this->label_main_control_array = label_main_control_array;
    label_main_width = label_main->width();
    label_main_height = label_main->height();
}

CMvCamera::~CMvCamera() {
    if (m_hDevHandle) {
        MV_CC_DestroyHandle(m_hDevHandle);
        m_hDevHandle = MV_NULL;
    }
    DeleteCriticalSection(&m_hSaveImageMux);

}

// ch:获取SDK版本号
int CMvCamera::GetSDKVersion() {
    return MV_CC_GetSDKVersion();
}

// ch:枚举设备
int CMvCamera::EnumDevices(unsigned int nTLayerType, MV_CC_DEVICE_INFO_LIST *pstDevList) {
    return MV_CC_EnumDevices(nTLayerType, pstDevList);
}

// ch:判断设备是否可达
bool CMvCamera::IsDeviceAccessible(MV_CC_DEVICE_INFO *pstDevInfo, unsigned int nAccessMode) {
    return MV_CC_IsDeviceAccessible(pstDevInfo, nAccessMode);
}

// ch:打开设备
int CMvCamera::Open() {

    if (m_hDevHandle) {
        return MV_E_CALLORDER;
    }

    int nRet = MV_CC_CreateHandle(&m_hDevHandle, device_info);
    if (MV_OK != nRet) {
        return nRet;
    }

    nRet = MV_CC_OpenDevice(m_hDevHandle);
    if (MV_OK != nRet) {
        MV_CC_DestroyHandle(m_hDevHandle);
        m_hDevHandle = MV_NULL;
        is_open = false;
    } else {
        is_open = true;
    }

    return nRet;
}

// ch:关闭设备
int CMvCamera::Close() {
    if (MV_NULL == m_hDevHandle) {
        return MV_E_HANDLE;
    }

    MV_CC_CloseDevice(m_hDevHandle);

    int nRet = MV_CC_DestroyHandle(m_hDevHandle);
    m_hDevHandle = MV_NULL;

    return nRet;
}

// ch:判断相机是否处于连接状态
bool CMvCamera::IsDeviceConnected() {
    return MV_CC_IsDeviceConnected(m_hDevHandle);
}

// ch:注册图像数据回调
int CMvCamera::RegisterImageCallBack(void(__stdcall *cbOutput)(unsigned char *pData,
                                                               MV_FRAME_OUT_INFO_EX *pFrameInfo,
                                                               void *pUser), void *pUser) {
    return MV_CC_RegisterImageCallBackEx(m_hDevHandle, cbOutput, pUser);
}

int aaa() {

    return 0;
}


int CMvCamera::StartGrabbing() {

    ret = MV_CC_StartGrabbing(m_hDevHandle);

    if (ret) {
        cout << "相机" << cam_num << "打开失败! 错误代码: " << ret << endl;
        return ret;
    } else {
        cout << "相机" << cam_num << "打开成功~" << endl;
    }
    thread work_thread(GrabThread, this);
    work_thread.detach();
    // todo &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    //    GrabThread(this);
    return ret;
}


void CMvCamera::GrabThread(CMvCamera *self) {
    self->GrabThreadProcess();
}


int CMvCamera::GrabThreadProcess() {
    cout << "相机" << cam_num << "进入线程~" << endl;


    //从相机中获取一帧图像大小
    MVCC_INTVALUE_EX stIntValue = {0};
    //获取Int值
    int nRet = GetIntValue(IN "PayloadSize", OUT &stIntValue);
    if (nRet != MV_OK) {
        cout << "获取Payload大小失败" << endl;
        return nRet;
    }
    unsigned int nDataSize = (unsigned int) stIntValue.nCurValue;

    if (NULL == m_pGrabBuf || nDataSize > m_nGrabBufSize) {
        if (m_pGrabBuf) {
            free(m_pGrabBuf);
            m_pGrabBuf = NULL;
        }

        m_pGrabBuf = (unsigned char *) malloc(sizeof(unsigned char) * nDataSize);
        if (m_pGrabBuf == NULL) {

            cout << "m_pGrabBuf == NULL" << endl;

            return 0;
        }
        m_nGrabBufSize = nDataSize;
    }

    MV_FRAME_OUT_INFO_EX stImageInfo = {0};
    MV_DISPLAY_FRAME_INFO stDisplayInfo = {0};

    int n = 0;
    while (is_open) {
        n += 1;
        cout << m_hDevHandle << "相机" << cam_num << "输出-->第" << n << "帧" << endl;

        //写图 加锁
        EnterCriticalSection(&m_hSaveImageMux);
        nRet = GetOneFrameTimeout(m_pGrabBuf, m_nGrabBufSize, &stImageInfo, 1000);
        if (nRet == 0) { memcpy(&m_stImageInfo, &stImageInfo, sizeof(MV_FRAME_OUT_INFO_EX)); }
        LeaveCriticalSection(&m_hSaveImageMux);

        //未获取到
        if (nRet) {
            cout << "等待超时: 图像为获取到!" << endl;
            continue;
        }

        //获取大小
        bgr_data = (unsigned char *) malloc(stImageInfo.nWidth * stImageInfo.nHeight * 3);
        bgr_data_size = stImageInfo.nWidth * stImageInfo.nHeight * 3;

        //结构体赋值
        bgr_image_param.nWidth = stImageInfo.nWidth;                        // [IN]  图像宽
        bgr_image_param.nHeight = stImageInfo.nHeight;                      // [IN]  图像高
        bgr_image_param.enSrcPixelType = stImageInfo.enPixelType;           // [IN]  源像素格式
        bgr_image_param.pSrcData = m_pGrabBuf;                              // [IN]  输入数据缓存
        bgr_image_param.nSrcDataLen = stImageInfo.nFrameLen;                // [IN]  输入数据大小
        bgr_image_param.enDstPixelType = PixelType_Gvsp_BGR8_Packed;        // [IN]  目标像素格式
        bgr_image_param.pDstBuffer = bgr_data;                              // [OUT] 输出数据缓存
        bgr_image_param.nDstBufferSize = bgr_data_size;                     // [IN]  提供的输出缓冲区大小

        //数据转换: BayerRG8-->BGR888
        if (ConvertPixelType(&bgr_image_param)) {
            cout << "BayerRG8-->BGR888 转换失败!";
            continue;
        }

        //图像处理流程
        ProcessFlow();


        //释放malloc
        free(bgr_data);
    }
    return 0;
}

//图像处理流程
void CMvCamera::ProcessFlow() {
    //读取 bgr_image_param 结构体

    //向标签显示
    if (label_main_control_array[cam_num]) {
        ShowImageToMainLabel();
    }
}

void CMvCamera::ShowImageToMainLabel() {
    q_rgb_image = QImage(bgr_image_param.pDstBuffer, bgr_image_param.nWidth, bgr_image_param.nHeight,
                         bgr_image_param.nWidth * 3, QImage::Format_RGB888).rgbSwapped();
    label_main->setPixmap(QPixmap::fromImage(q_rgb_image).scaled(label_main_width, label_main_height));
}

// ch:停止抓图 
int CMvCamera::StopGrabbing() {
    return MV_CC_StopGrabbing(m_hDevHandle);
}

// ch:主动获取一帧图像数据 
int CMvCamera::GetImageBuffer(MV_FRAME_OUT *pFrame, int nMsec) {
    return MV_CC_GetImageBuffer(m_hDevHandle, pFrame, nMsec);
}

// ch:释放图像缓存 
int CMvCamera::FreeImageBuffer(MV_FRAME_OUT *pFrame) {
    return MV_CC_FreeImageBuffer(m_hDevHandle, pFrame);
}

// ch:主动获取一帧图像数据 
int CMvCamera::GetOneFrameTimeout(unsigned char *pData,
                                  unsigned int nDataSize,
                                  MV_FRAME_OUT_INFO_EX *pFrameInfo,
                                  int nMsec) {
    return MV_CC_GetOneFrameTimeout(m_hDevHandle, pData, nDataSize, pFrameInfo, nMsec);
}

// ch:设置显示窗口句柄 
int CMvCamera::DisplayOneFrame(MV_DISPLAY_FRAME_INFO *pDisplayInfo) {
    return MV_CC_DisplayOneFrame(m_hDevHandle, pDisplayInfo);
}

// ch:设置SDK内部图像缓存节点个数 
int CMvCamera::SetImageNodeNum(unsigned int nNum) {
    return MV_CC_SetImageNodeNum(m_hDevHandle, nNum);
}

// ch:获取设备信息 
int CMvCamera::GetDeviceInfo(MV_CC_DEVICE_INFO *pstDevInfo) {
    return MV_CC_GetDeviceInfo(m_hDevHandle, pstDevInfo);
}

// ch:获取GEV相机的统计信息 
int CMvCamera::GetGevAllMatchInfo(MV_MATCH_INFO_NET_DETECT *pMatchInfoNetDetect) {
    if (MV_NULL == pMatchInfoNetDetect) {
        return MV_E_PARAMETER;
    }

    MV_CC_DEVICE_INFO stDevInfo = {0};
    GetDeviceInfo(&stDevInfo);
    if (stDevInfo.nTLayerType != MV_GIGE_DEVICE) {
        return MV_E_SUPPORT;
    }

    MV_ALL_MATCH_INFO struMatchInfo = {0};

    struMatchInfo.nType = MV_MATCH_TYPE_NET_DETECT;
    struMatchInfo.pInfo = pMatchInfoNetDetect;
    struMatchInfo.nInfoSize = sizeof(MV_MATCH_INFO_NET_DETECT);
    memset(struMatchInfo.pInfo, 0, sizeof(MV_MATCH_INFO_NET_DETECT));

    return MV_CC_GetAllMatchInfo(m_hDevHandle, &struMatchInfo);
}

// ch:获取U3V相机的统计信息 
int CMvCamera::GetU3VAllMatchInfo(MV_MATCH_INFO_USB_DETECT *pMatchInfoUSBDetect) {
    if (MV_NULL == pMatchInfoUSBDetect) {
        return MV_E_PARAMETER;
    }

    MV_CC_DEVICE_INFO stDevInfo = {0};
    GetDeviceInfo(&stDevInfo);
    if (stDevInfo.nTLayerType != MV_USB_DEVICE) {
        return MV_E_SUPPORT;
    }

    MV_ALL_MATCH_INFO struMatchInfo = {0};

    struMatchInfo.nType = MV_MATCH_TYPE_USB_DETECT;
    struMatchInfo.pInfo = pMatchInfoUSBDetect;
    struMatchInfo.nInfoSize = sizeof(MV_MATCH_INFO_USB_DETECT);
    memset(struMatchInfo.pInfo, 0, sizeof(MV_MATCH_INFO_USB_DETECT));

    return MV_CC_GetAllMatchInfo(m_hDevHandle, &struMatchInfo);
}

// ch:获取和设置Int型参数，如 Width和Height，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
// en:Get Int type parameters, such as Width and Height, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int CMvCamera::GetIntValue(IN const char *strKey, OUT MVCC_INTVALUE_EX *pIntValue) {
    return MV_CC_GetIntValueEx(m_hDevHandle, strKey, pIntValue);
}

int CMvCamera::SetIntValue(IN const char *strKey, IN int64_t nValue) {
    return MV_CC_SetIntValueEx(m_hDevHandle, strKey, nValue);
}

// ch:获取和设置Enum型参数，如 PixelFormat，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
// en:Get Enum type parameters, such as PixelFormat, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int CMvCamera::GetEnumValue(IN const char *strKey, OUT MVCC_ENUMVALUE *pEnumValue) {
    return MV_CC_GetEnumValue(m_hDevHandle, strKey, pEnumValue);
}

int CMvCamera::SetEnumValue(IN const char *strKey, IN unsigned int nValue) {
    return MV_CC_SetEnumValue(m_hDevHandle, strKey, nValue);
}

int CMvCamera::SetEnumValueByString(IN const char *strKey, IN const char *sValue) {
    return MV_CC_SetEnumValueByString(m_hDevHandle, strKey, sValue);
}

// ch:获取和设置Float型参数，如 ExposureTime和Gain，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
// en:Get Float type parameters, such as ExposureTime and Gain, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int CMvCamera::GetFloatValue(IN const char *strKey, OUT MVCC_FLOATVALUE *pFloatValue) {
    return MV_CC_GetFloatValue(m_hDevHandle, strKey, pFloatValue);
}

int CMvCamera::SetFloatValue(IN const char *strKey, IN float fValue) {
    return MV_CC_SetFloatValue(m_hDevHandle, strKey, fValue);
}

// ch:获取和设置Bool型参数，如 ReverseX，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
// en:Get Bool type parameters, such as ReverseX, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int CMvCamera::GetBoolValue(IN const char *strKey, OUT bool *pbValue) {
    return MV_CC_GetBoolValue(m_hDevHandle, strKey, pbValue);
}

int CMvCamera::SetBoolValue(IN const char *strKey, IN bool bValue) {
    return MV_CC_SetBoolValue(m_hDevHandle, strKey, bValue);
}

// ch:获取和设置String型参数，如 DeviceUserID，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件UserSetSave
// en:Get String type parameters, such as DeviceUserID, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int CMvCamera::GetStringValue(IN const char *strKey, MVCC_STRINGVALUE *pStringValue) {
    return MV_CC_GetStringValue(m_hDevHandle, strKey, pStringValue);
}

int CMvCamera::SetStringValue(IN const char *strKey, IN const char *strValue) {
    return MV_CC_SetStringValue(m_hDevHandle, strKey, strValue);
}

// ch:执行一次Command型命令，如 UserSetSave，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
// en:Execute Command once, such as UserSetSave, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int CMvCamera::CommandExecute(IN const char *strKey) {
    return MV_CC_SetCommandValue(m_hDevHandle, strKey);
}

// ch:探测网络最佳包大小(只对GigE相机有效) 
int CMvCamera::GetOptimalPacketSize(unsigned int *pOptimalPacketSize) {
    if (MV_NULL == pOptimalPacketSize) {
        return MV_E_PARAMETER;
    }

    int nRet = MV_CC_GetOptimalPacketSize(m_hDevHandle);
    if (nRet < MV_OK) {
        return nRet;
    }

    *pOptimalPacketSize = (unsigned int) nRet;

    return MV_OK;
}

// ch:注册消息异常回调 
int CMvCamera::RegisterExceptionCallBack(void(__stdcall *cbException)(unsigned int nMsgType, void *pUser),
                                         void *pUser) {
    return MV_CC_RegisterExceptionCallBack(m_hDevHandle, cbException, pUser);
}

// ch:注册单个事件回调 
int CMvCamera::RegisterEventCallBack(const char *pEventName,
                                     void(__stdcall *cbEvent)(MV_EVENT_OUT_INFO *pEventInfo, void *pUser),
                                     void *pUser) {
    return MV_CC_RegisterEventCallBackEx(m_hDevHandle, pEventName, cbEvent, pUser);
}

// ch:强制IP 
int CMvCamera::ForceIp(unsigned int nIP, unsigned int nSubNetMask, unsigned int nDefaultGateWay) {
    return MV_GIGE_ForceIpEx(m_hDevHandle, nIP, nSubNetMask, nDefaultGateWay);
}

// ch:配置IP方式 
int CMvCamera::SetIpConfig(unsigned int nType) {
    return MV_GIGE_SetIpConfig(m_hDevHandle, nType);
}

// ch:设置网络传输模式 
int CMvCamera::SetNetTransMode(unsigned int nType) {
    return MV_GIGE_SetNetTransMode(m_hDevHandle, nType);
}

// ch:像素格式转换 
int CMvCamera::ConvertPixelType(MV_CC_PIXEL_CONVERT_PARAM *pstCvtParam) {
    return MV_CC_ConvertPixelType(m_hDevHandle, pstCvtParam);
}

// ch:保存图片 
int CMvCamera::SaveImage(MV_SAVE_IMAGE_PARAM_EX *pstParam) {
    return MV_CC_SaveImageEx2(m_hDevHandle, pstParam);
}

// ch:保存图片为文件 
int CMvCamera::SaveImageToFile(MV_SAVE_IMG_TO_FILE_PARAM *pstSaveFileParam) {
    return MV_CC_SaveImageToFile(m_hDevHandle, pstSaveFileParam);
}


bool CMvCamera::RemoveCustomPixelFormats(enum MvGvspPixelType enPixelFormat) {
    int nResult = enPixelFormat & MV_GVSP_PIX_CUSTOM;
    if (MV_GVSP_PIX_CUSTOM == nResult) {
        return true;
    } else {
        return false;
    }
}