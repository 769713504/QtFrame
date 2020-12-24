#include <thread>
#include "CameraOperation.h"
#include "iostream"


using namespace std;

CMvCamera::CMvCamera(MV_CC_DEVICE_INFO_LIST *device_info_list,
                     int cam_num_,
                     QLabel *label_main,
                     int *label_main_control_array) {
    InitializeCriticalSection(&m_hSaveImageMux);
    //������
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

// ch:��ȡSDK�汾��
int CMvCamera::GetSDKVersion() {
    return MV_CC_GetSDKVersion();
}

// ch:ö���豸
int CMvCamera::EnumDevices(unsigned int nTLayerType, MV_CC_DEVICE_INFO_LIST *pstDevList) {
    return MV_CC_EnumDevices(nTLayerType, pstDevList);
}

// ch:�ж��豸�Ƿ�ɴ�
bool CMvCamera::IsDeviceAccessible(MV_CC_DEVICE_INFO *pstDevInfo, unsigned int nAccessMode) {
    return MV_CC_IsDeviceAccessible(pstDevInfo, nAccessMode);
}

// ch:���豸
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

// ch:�ر��豸
int CMvCamera::Close() {
    if (MV_NULL == m_hDevHandle) {
        return MV_E_HANDLE;
    }

    MV_CC_CloseDevice(m_hDevHandle);

    int nRet = MV_CC_DestroyHandle(m_hDevHandle);
    m_hDevHandle = MV_NULL;

    return nRet;
}

// ch:�ж�����Ƿ�������״̬
bool CMvCamera::IsDeviceConnected() {
    return MV_CC_IsDeviceConnected(m_hDevHandle);
}

// ch:ע��ͼ�����ݻص�
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
        cout << "���" << cam_num << "��ʧ��! �������: " << ret << endl;
        return ret;
    } else {
        cout << "���" << cam_num << "�򿪳ɹ�~" << endl;
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
    cout << "���" << cam_num << "�����߳�~" << endl;


    //������л�ȡһ֡ͼ���С
    MVCC_INTVALUE_EX stIntValue = {0};
    //��ȡIntֵ
    int nRet = GetIntValue(IN "PayloadSize", OUT &stIntValue);
    if (nRet != MV_OK) {
        cout << "��ȡPayload��Сʧ��" << endl;
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
        cout << m_hDevHandle << "���" << cam_num << "���-->��" << n << "֡" << endl;

        //дͼ ����
        EnterCriticalSection(&m_hSaveImageMux);
        nRet = GetOneFrameTimeout(m_pGrabBuf, m_nGrabBufSize, &stImageInfo, 1000);
        if (nRet == 0) { memcpy(&m_stImageInfo, &stImageInfo, sizeof(MV_FRAME_OUT_INFO_EX)); }
        LeaveCriticalSection(&m_hSaveImageMux);

        //δ��ȡ��
        if (nRet) {
            cout << "�ȴ���ʱ: ͼ��Ϊ��ȡ��!" << endl;
            continue;
        }

        //��ȡ��С
        bgr_data = (unsigned char *) malloc(stImageInfo.nWidth * stImageInfo.nHeight * 3);
        bgr_data_size = stImageInfo.nWidth * stImageInfo.nHeight * 3;

        //�ṹ�帳ֵ
        bgr_image_param.nWidth = stImageInfo.nWidth;                        // [IN]  ͼ���
        bgr_image_param.nHeight = stImageInfo.nHeight;                      // [IN]  ͼ���
        bgr_image_param.enSrcPixelType = stImageInfo.enPixelType;           // [IN]  Դ���ظ�ʽ
        bgr_image_param.pSrcData = m_pGrabBuf;                              // [IN]  �������ݻ���
        bgr_image_param.nSrcDataLen = stImageInfo.nFrameLen;                // [IN]  �������ݴ�С
        bgr_image_param.enDstPixelType = PixelType_Gvsp_BGR8_Packed;        // [IN]  Ŀ�����ظ�ʽ
        bgr_image_param.pDstBuffer = bgr_data;                              // [OUT] ������ݻ���
        bgr_image_param.nDstBufferSize = bgr_data_size;                     // [IN]  �ṩ�������������С

        //����ת��: BayerRG8-->BGR888
        if (ConvertPixelType(&bgr_image_param)) {
            cout << "BayerRG8-->BGR888 ת��ʧ��!";
            continue;
        }

        //ͼ��������
        ProcessFlow();


        //�ͷ�malloc
        free(bgr_data);
    }
    return 0;
}

//ͼ��������
void CMvCamera::ProcessFlow() {
    //��ȡ bgr_image_param �ṹ��

    //���ǩ��ʾ
    if (label_main_control_array[cam_num]) {
        ShowImageToMainLabel();
    }
}

void CMvCamera::ShowImageToMainLabel() {
    q_rgb_image = QImage(bgr_image_param.pDstBuffer, bgr_image_param.nWidth, bgr_image_param.nHeight,
                         bgr_image_param.nWidth * 3, QImage::Format_RGB888).rgbSwapped();
    label_main->setPixmap(QPixmap::fromImage(q_rgb_image).scaled(label_main_width, label_main_height));
}

// ch:ֹͣץͼ 
int CMvCamera::StopGrabbing() {
    return MV_CC_StopGrabbing(m_hDevHandle);
}

// ch:������ȡһ֡ͼ������ 
int CMvCamera::GetImageBuffer(MV_FRAME_OUT *pFrame, int nMsec) {
    return MV_CC_GetImageBuffer(m_hDevHandle, pFrame, nMsec);
}

// ch:�ͷ�ͼ�񻺴� 
int CMvCamera::FreeImageBuffer(MV_FRAME_OUT *pFrame) {
    return MV_CC_FreeImageBuffer(m_hDevHandle, pFrame);
}

// ch:������ȡһ֡ͼ������ 
int CMvCamera::GetOneFrameTimeout(unsigned char *pData,
                                  unsigned int nDataSize,
                                  MV_FRAME_OUT_INFO_EX *pFrameInfo,
                                  int nMsec) {
    return MV_CC_GetOneFrameTimeout(m_hDevHandle, pData, nDataSize, pFrameInfo, nMsec);
}

// ch:������ʾ���ھ�� 
int CMvCamera::DisplayOneFrame(MV_DISPLAY_FRAME_INFO *pDisplayInfo) {
    return MV_CC_DisplayOneFrame(m_hDevHandle, pDisplayInfo);
}

// ch:����SDK�ڲ�ͼ�񻺴�ڵ���� 
int CMvCamera::SetImageNodeNum(unsigned int nNum) {
    return MV_CC_SetImageNodeNum(m_hDevHandle, nNum);
}

// ch:��ȡ�豸��Ϣ 
int CMvCamera::GetDeviceInfo(MV_CC_DEVICE_INFO *pstDevInfo) {
    return MV_CC_GetDeviceInfo(m_hDevHandle, pstDevInfo);
}

// ch:��ȡGEV�����ͳ����Ϣ 
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

// ch:��ȡU3V�����ͳ����Ϣ 
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

// ch:��ȡ������Int�Ͳ������� Width��Height����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
// en:Get Int type parameters, such as Width and Height, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int CMvCamera::GetIntValue(IN const char *strKey, OUT MVCC_INTVALUE_EX *pIntValue) {
    return MV_CC_GetIntValueEx(m_hDevHandle, strKey, pIntValue);
}

int CMvCamera::SetIntValue(IN const char *strKey, IN int64_t nValue) {
    return MV_CC_SetIntValueEx(m_hDevHandle, strKey, nValue);
}

// ch:��ȡ������Enum�Ͳ������� PixelFormat����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
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

// ch:��ȡ������Float�Ͳ������� ExposureTime��Gain����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
// en:Get Float type parameters, such as ExposureTime and Gain, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int CMvCamera::GetFloatValue(IN const char *strKey, OUT MVCC_FLOATVALUE *pFloatValue) {
    return MV_CC_GetFloatValue(m_hDevHandle, strKey, pFloatValue);
}

int CMvCamera::SetFloatValue(IN const char *strKey, IN float fValue) {
    return MV_CC_SetFloatValue(m_hDevHandle, strKey, fValue);
}

// ch:��ȡ������Bool�Ͳ������� ReverseX����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
// en:Get Bool type parameters, such as ReverseX, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int CMvCamera::GetBoolValue(IN const char *strKey, OUT bool *pbValue) {
    return MV_CC_GetBoolValue(m_hDevHandle, strKey, pbValue);
}

int CMvCamera::SetBoolValue(IN const char *strKey, IN bool bValue) {
    return MV_CC_SetBoolValue(m_hDevHandle, strKey, bValue);
}

// ch:��ȡ������String�Ͳ������� DeviceUserID����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�UserSetSave
// en:Get String type parameters, such as DeviceUserID, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int CMvCamera::GetStringValue(IN const char *strKey, MVCC_STRINGVALUE *pStringValue) {
    return MV_CC_GetStringValue(m_hDevHandle, strKey, pStringValue);
}

int CMvCamera::SetStringValue(IN const char *strKey, IN const char *strValue) {
    return MV_CC_SetStringValue(m_hDevHandle, strKey, strValue);
}

// ch:ִ��һ��Command������� UserSetSave����ϸ���ݲο�SDK��װĿ¼�µ� MvCameraNode.xlsx �ļ�
// en:Execute Command once, such as UserSetSave, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int CMvCamera::CommandExecute(IN const char *strKey) {
    return MV_CC_SetCommandValue(m_hDevHandle, strKey);
}

// ch:̽��������Ѱ���С(ֻ��GigE�����Ч) 
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

// ch:ע����Ϣ�쳣�ص� 
int CMvCamera::RegisterExceptionCallBack(void(__stdcall *cbException)(unsigned int nMsgType, void *pUser),
                                         void *pUser) {
    return MV_CC_RegisterExceptionCallBack(m_hDevHandle, cbException, pUser);
}

// ch:ע�ᵥ���¼��ص� 
int CMvCamera::RegisterEventCallBack(const char *pEventName,
                                     void(__stdcall *cbEvent)(MV_EVENT_OUT_INFO *pEventInfo, void *pUser),
                                     void *pUser) {
    return MV_CC_RegisterEventCallBackEx(m_hDevHandle, pEventName, cbEvent, pUser);
}

// ch:ǿ��IP 
int CMvCamera::ForceIp(unsigned int nIP, unsigned int nSubNetMask, unsigned int nDefaultGateWay) {
    return MV_GIGE_ForceIpEx(m_hDevHandle, nIP, nSubNetMask, nDefaultGateWay);
}

// ch:����IP��ʽ 
int CMvCamera::SetIpConfig(unsigned int nType) {
    return MV_GIGE_SetIpConfig(m_hDevHandle, nType);
}

// ch:�������紫��ģʽ 
int CMvCamera::SetNetTransMode(unsigned int nType) {
    return MV_GIGE_SetNetTransMode(m_hDevHandle, nType);
}

// ch:���ظ�ʽת�� 
int CMvCamera::ConvertPixelType(MV_CC_PIXEL_CONVERT_PARAM *pstCvtParam) {
    return MV_CC_ConvertPixelType(m_hDevHandle, pstCvtParam);
}

// ch:����ͼƬ 
int CMvCamera::SaveImage(MV_SAVE_IMAGE_PARAM_EX *pstParam) {
    return MV_CC_SaveImageEx2(m_hDevHandle, pstParam);
}

// ch:����ͼƬΪ�ļ� 
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