#ifndef _MV_CAMERA_PARAMS_H_
#define _MV_CAMERA_PARAMS_H_

#include "MvInclude/PixelType.h"

#ifndef __cplusplus
typedef char    bool;
#define true    1
#define false   0
#endif

// 设备类型定义    
#define MV_UNKNOW_DEVICE        0x00000000                  // 未知设备类型，保留意义       
#define MV_GIGE_DEVICE          0x00000001                  // GigE设备                     
#define MV_1394_DEVICE          0x00000002                  // 1394-a/b 设备                
#define MV_USB_DEVICE           0x00000004                  // USB 设备                     
#define MV_CAMERALINK_DEVICE    0x00000008                  // CameraLink设备               

// GigE设备信息    
typedef struct _MV_GIGE_DEVICE_INFO_ {
    unsigned int nIpCfgOption;                              // [OUT] IP配置选项
    unsigned int nIpCfgCurrent;                             // [OUT] 当前IP配置
    unsigned int nCurrentIp;                                // [OUT] 当前IP地址
    unsigned int nCurrentSubNetMask;                        // [OUT] 当前子网掩码
    unsigned int nDefultGateWay;                            // [OUT] 当前网关
    unsigned char chManufacturerName[32];                   // [OUT] 制造商名称
    unsigned char chModelName[32];                          // [OUT] 型号名称
    unsigned char chDeviceVersion[32];                      // [OUT] 设备版本
    unsigned char chManufacturerSpecificInfo[48];           // [OUT] 制造商的具体信息
    unsigned char chSerialNumber[16];                       // [OUT] 序列号
    unsigned char chUserDefinedName[16];                    // [OUT] 用户自定义名称
    unsigned int nNetExport;                                // [OUT] 网口IP地址

    unsigned int nReserved[4];                              // 预留
} MV_GIGE_DEVICE_INFO;

#define INFO_MAX_BUFFER_SIZE 64                             // 最大的数据信息大小    

// USB设备信息    
typedef struct _MV_USB3_DEVICE_INFO_ {
    unsigned char CrtlInEndPoint;                            // [OUT] 控制输入端点         
    unsigned char CrtlOutEndPoint;                           // [OUT] 控制输出端点         
    unsigned char StreamEndPoint;                            // [OUT] 流端点               
    unsigned char EventEndPoint;                             // [OUT] 事件端点             
    unsigned short idVendor;                                 // [OUT] 供应商ID号           
    unsigned short idProduct;                                // [OUT] 产品ID号             
    unsigned int nDeviceNumber;                              // [OUT] 设备序列号           
    unsigned char chDeviceGUID[INFO_MAX_BUFFER_SIZE];        // [OUT] 设备GUID号           
    unsigned char chVendorName[INFO_MAX_BUFFER_SIZE];        // [OUT] 供应商名字           
    unsigned char chModelName[INFO_MAX_BUFFER_SIZE];         // [OUT] 型号名字             
    unsigned char chFamilyName[INFO_MAX_BUFFER_SIZE];        // [OUT] 家族名字             
    unsigned char chDeviceVersion[INFO_MAX_BUFFER_SIZE];     // [OUT] 设备版本             
    unsigned char chManufacturerName[INFO_MAX_BUFFER_SIZE];  // [OUT] 制造商名字           
    unsigned char chSerialNumber[INFO_MAX_BUFFER_SIZE];      // [OUT] 序列号               
    unsigned char chUserDefinedName[INFO_MAX_BUFFER_SIZE];   // [OUT] 用户自定义名字       
    unsigned int nbcdUSB;                                    // [OUT] 支持的USB协议        
    unsigned int nDeviceAddress;                             // [OUT] 设备地址             

    unsigned int nReserved[2];                               // 预留                 
} MV_USB3_DEVICE_INFO;

// CameraLink设备信息    
typedef struct _MV_CamL_DEV_INFO_ {
    unsigned char chPortID[INFO_MAX_BUFFER_SIZE];             // [OUT] 端口号                       
    unsigned char chModelName[INFO_MAX_BUFFER_SIZE];          // [OUT] 型号名字                     
    unsigned char chFamilyName[INFO_MAX_BUFFER_SIZE];         // [OUT] 名称                         
    unsigned char chDeviceVersion[INFO_MAX_BUFFER_SIZE];      // [OUT] 设备版本                     
    unsigned char chManufacturerName[INFO_MAX_BUFFER_SIZE];   // [OUT] 制造商名字                   
    unsigned char chSerialNumber[INFO_MAX_BUFFER_SIZE];       // [OUT] 序列号                       

    unsigned int nReserved[38];                               // 预留                         
} MV_CamL_DEV_INFO;

// 设备信息    
typedef struct _MV_CC_DEVICE_INFO_ {
    unsigned short nMajorVer;                                 // [OUT] 主要版本                                 
    unsigned short nMinorVer;                                 // [OUT] 次要版本                                 
    unsigned int nMacAddrHigh;                                // [OUT] 高MAC地址                                
    unsigned int nMacAddrLow;                                 // [OUT] 低MAC地址                                
    unsigned int nTLayerType;                                 // [OUT] 设备传输层协议类型，e.g. MV_GIGE_DEVICE  
    unsigned int nReserved[4];                                // 预留                                     

    union {
        MV_GIGE_DEVICE_INFO stGigEInfo;                       // [OUT] GigE设备信息                             
        MV_USB3_DEVICE_INFO stUsb3VInfo;                      // [OUT] USB设备信息                              
        MV_CamL_DEV_INFO stCamLInfo;                          // [OUT] CameraLink设备信息                          
    } SpecialInfo;

} MV_CC_DEVICE_INFO;

#define MV_MAX_TLS_NUM          8                             // 最多支持的传输层实例个数    
#define MV_MAX_DEVICE_NUM      256                            // 最大支持的设备个数          

// 设备信息列表    
typedef struct _MV_CC_DEVICE_INFO_LIST_ {
    unsigned int nDeviceNum;                                  // [OUT] 在线设备数量       
    MV_CC_DEVICE_INFO *pDeviceInfo[MV_MAX_DEVICE_NUM];        // [OUT] 支持最多256个设备  

} MV_CC_DEVICE_INFO_LIST;

// 通过GenTL枚举到的Interface信息    
typedef struct _MV_GENTL_IF_INFO_ {
    unsigned char chInterfaceID[INFO_MAX_BUFFER_SIZE];        // [OUT] GenTL接口ID              
    unsigned char chTLType[INFO_MAX_BUFFER_SIZE];             // [OUT] 传输层类型               
    unsigned char chDisplayName[INFO_MAX_BUFFER_SIZE];        // [OUT] Interface显示名称        
    unsigned int nCtiIndex;                                   // [OUT] GenTL的cti文件索引       

    unsigned int nReserved[8];                                // 预留                     
} MV_GENTL_IF_INFO;

#define MV_MAX_GENTL_IF_NUM      256                           // 最大支持的GenTL接口数量    
// 通过GenTL枚举到的接口信息列表    
typedef struct _MV_GENTL_IF_INFO_LIST_ {
    unsigned int nInterfaceNum;                                // [OUT] 在线接口数量             
    MV_GENTL_IF_INFO *pIFInfo[MV_MAX_GENTL_IF_NUM];            // [OUT] 支持最多256个接口        

} MV_GENTL_IF_INFO_LIST;

// 通过GenTL枚举到的设备信息    
typedef struct _MV_GENTL_DEV_INFO_ {
    unsigned char chInterfaceID[INFO_MAX_BUFFER_SIZE];         // [OUT] GenTL接口ID              
    unsigned char chDeviceID[INFO_MAX_BUFFER_SIZE];            // [OUT] 设备ID                   
    unsigned char chVendorName[INFO_MAX_BUFFER_SIZE];          // [OUT] 供应商名字               
    unsigned char chModelName[INFO_MAX_BUFFER_SIZE];           // [OUT] 型号名字                 
    unsigned char chTLType[INFO_MAX_BUFFER_SIZE];              // [OUT] 传输层类型               
    unsigned char chDisplayName[INFO_MAX_BUFFER_SIZE];         // [OUT] 设备显示名称             
    unsigned char chUserDefinedName[INFO_MAX_BUFFER_SIZE];     // [OUT] 用户自定义名字           
    unsigned char chSerialNumber[INFO_MAX_BUFFER_SIZE];        // [OUT] 序列号                   
    unsigned char chDeviceVersion[INFO_MAX_BUFFER_SIZE];       // [OUT] 设备版本号               
    unsigned int nCtiIndex;                                    // [OUT] GenTL的cti文件索引       

    unsigned int nReserved[8];                                 // 预留                     
} MV_GENTL_DEV_INFO;

#define MV_MAX_GENTL_DEV_NUM      256                          //  最大支持的GenTL设备数量    
// 通过GenTL枚举到的设备信息列表    
typedef struct _MV_GENTL_DEV_INFO_LIST_ {
    unsigned int nDeviceNum;                                   // [OUT] 在线设备数量             
    MV_GENTL_DEV_INFO *pDeviceInfo[MV_MAX_GENTL_DEV_NUM];      // [OUT] 支持最多256个设备        

} MV_GENTL_DEV_INFO_LIST;

// 设备的访问模式    
// 独占权限，其他APP只允许读CCP寄存器                        
#define MV_ACCESS_Exclusive                                    1
// 可以从5模式下抢占权限，然后以独占权限打开                 
#define MV_ACCESS_ExclusiveWithSwitch                          2
// 控制权限，其他APP允许读所有寄存器                         
#define MV_ACCESS_Control                                      3
// 可以从5的模式下抢占权限，然后以控制权限打开               
#define MV_ACCESS_ControlWithSwitch                            4
// 以可被抢占的控制权限打开                                  
#define MV_ACCESS_ControlSwitchEnable                          5
// 可以从5的模式下抢占权限，然后以可被抢占的控制权限打开     
#define MV_ACCESS_ControlSwitchEnableWithKey                   6
// 读模式打开设备，适用于控制权限下                          
#define MV_ACCESS_Monitor                                      7

// Chunk内容    
typedef struct _MV_CHUNK_DATA_CONTENT_ {
    unsigned char *pChunkData;                                 // [OUT] Chunk数据                
    unsigned int nChunkID;                                     // [OUT] Chunk ID                 
    unsigned int nChunkLen;                                    // [OUT] Chunk的长度              

    unsigned int nReserved[8];                                 // 预留                     

} MV_CHUNK_DATA_CONTENT;

// 输出帧的信息    
typedef struct _MV_FRAME_OUT_INFO_EX_ {
    unsigned short nWidth;                                     // [OUT] 图像宽                   
    unsigned short nHeight;                                    // [OUT] 图像高                   
    enum MvGvspPixelType enPixelType;                          // [OUT] 像素格式                 

    unsigned int nFrameNum;                                    // [OUT] 帧号                     
    unsigned int nDevTimeStampHigh;                            // [OUT] 时间戳高32位             
    unsigned int nDevTimeStampLow;                             // [OUT] 时间戳低32位             
    unsigned int nReserved0;                                   // [OUT] 保留，8字节对齐          
    int64_t nHostTimeStamp;                                    // [OUT] 主机生成的时间戳         

    unsigned int nFrameLen;                                    // [OUT] 帧的长度                 

    // 设备水印时标    
    unsigned int nSecondCount;                                 // [OUT] 秒数                     
    unsigned int nCycleCount;                                  // [OUT] 周期数                   
    unsigned int nCycleOffset;                                 // [OUT] 周期偏移量               

    float fGain;                                               // [OUT] 增益                     
    float fExposureTime;                                       // [OUT] 曝光时间                 
    unsigned int nAverageBrightness;                           // [OUT] 平均亮度                 

    // 白平衡相关    
    unsigned int nRed;                                         // [OUT] 红色                     
    unsigned int nGreen;                                       // [OUT] 绿色                     
    unsigned int nBlue;                                        // [OUT] 蓝色                     

    unsigned int nFrameCounter;                                // [OUT] 总帧数                   
    unsigned int nTriggerIndex;                                // [OUT] 触发计数                 

    unsigned int nInput;                                       // [OUT] 输入                     
    unsigned int nOutput;                                      // [OUT] 输出                     

    // ROI区域    
    unsigned short nOffsetX;                                   // [OUT] 水平偏移量               
    unsigned short nOffsetY;                                   // [OUT] 垂直偏移量               
    unsigned short nChunkWidth;                                // [OUT] Chunk宽                  
    unsigned short nChunkHeight;                               // [OUT] Chunk高                  

    unsigned int nLostPacket;                                  // [OUT] 本帧丢包数               

    unsigned int nUnparsedChunkNum;                            // [OUT] 未解析的Chunkdata个数    
    union {
        MV_CHUNK_DATA_CONTENT *pUnparsedChunkContent;          // [OUT] 未解析的Chunk   
        int64_t nAligning;                                     // [OUT] 校准                     
    } UnparsedChunkList;

    unsigned int nReserved[36];                                // 预留                     
} MV_FRAME_OUT_INFO_EX;

// 图像结构体，输出图像指针地址及图像信息    
typedef struct _MV_FRAME_OUT_ {
    unsigned char *pBufAddr;                                   // [OUT] 图像指针地址             
    MV_FRAME_OUT_INFO_EX stFrameInfo;                          // [OUT] 图像信息                 

    unsigned int nRes[16];                                     // 预留                     
} MV_FRAME_OUT;

// 取流策略    
typedef enum _MV_GRAB_STRATEGY_ {
    MV_GrabStrategy_OneByOne = 0,                              // 从旧到新一帧一帧的获取图像（默认为该策略）           
    MV_GrabStrategy_LatestImagesOnly = 1,                      // 获取列表中最新的一帧图像（同时清除列表中的其余图像） 
    MV_GrabStrategy_LatestImages = 2,                          // 获取列表中最新的图像，个数由OutputQueueSize决定，范围为1-ImageNodeNum，设置成1等同于LatestImagesOnly,设置成ImageNodeNum等同于OneByOne 
    MV_GrabStrategy_UpcomingImage = 3,                         // 等待下一帧图像                                       

} MV_GRAB_STRATEGY;

// 网络传输的相关信息    
typedef struct _MV_NETTRANS_INFO_ {
    int64_t nReceiveDataSize;                                  // [OUT] 已接收数据大小 [统计StartGrabbing和StopGrabbing之间的数据量] 
    int nThrowFrameCount;                                      // [OUT] 丢帧数量                                                     
    unsigned int nNetRecvFrameCount;                           // [OUT] 已接收的帧数                                                 
    int64_t nRequestResendPacketCount;                         // [OUT] 请求重发包数                                                 
    int64_t nResendPacketCount;                                // [OUT] 重发包数                                                     

} MV_NETTRANS_INFO;

// 信息类型    
#define MV_MATCH_TYPE_NET_DETECT             0x00000001        // 网络流量和丢包信息              
#define MV_MATCH_TYPE_USB_DETECT             0x00000002        // host接收到来自U3V设备的字节总数 

// 全匹配的一种信息结构体    
typedef struct _MV_ALL_MATCH_INFO_ {
    unsigned int nType;                                        // [IN]  需要输出的信息类型，e.g. MV_MATCH_TYPE_NET_DETECT 
    void *pInfo;                                               // [OUT] 输出的信息缓存，由调用者分配                      
    unsigned int nInfoSize;                                    // [IN]  信息缓存的大小                                   

} MV_ALL_MATCH_INFO;

// 网络流量和丢包信息反馈结构体，对应类型为 MV_MATCH_TYPE_NET_DETECT    
typedef struct _MV_MATCH_INFO_NET_DETECT_ {
    int64_t nReceiveDataSize;                                  // [OUT] 已接收数据大小 [统计StartGrabbing和StopGrabbing之间的数据量] 
    int64_t nLostPacketCount;                                  // [OUT] 丢失的包数量                                                 
    unsigned int nLostFrameCount;                              // [OUT] 丢帧数量                                                     
    unsigned int nNetRecvFrameCount;                           // [OUT] 保留                                                         
    int64_t nRequestResendPacketCount;                         // [OUT] 请求重发包数                                                 
    int64_t nResendPacketCount;                                // [OUT] 重发包数                                                     
} MV_MATCH_INFO_NET_DETECT;

// host收到从u3v设备端的总字节数，对应类型为 MV_MATCH_TYPE_USB_DETECT    
typedef struct _MV_MATCH_INFO_USB_DETECT_ {
    int64_t nReceiveDataSize;                                  // [OUT] 已接收数据大小 [统计OpenDevicce和CloseDevice之间的数据量] 
    unsigned int nReceivedFrameCount;                          // [OUT] 已收到的帧数                                              
    unsigned int nErrorFrameCount;                             // [OUT] 错误帧数                                                  
    unsigned int nReserved[2];                                 // 保留                                                      
} MV_MATCH_INFO_USB_DETECT;

// 显示帧信息   
typedef struct _MV_DISPLAY_FRAME_INFO_ {
    void *hWnd;                                                 // [IN]  窗口句柄       
    unsigned char *pData;                                       // [IN]  显示的数据     
    unsigned int nDataLen;                                      // [IN]  数据长度       
    unsigned short nWidth;                                      // [IN]  图像宽         
    unsigned short nHeight;                                     // [IN]  图像高         
    enum MvGvspPixelType enPixelType;                           // [IN]  像素格式       
    unsigned int nRes[4];                                       // 保留           

} MV_DISPLAY_FRAME_INFO;

// 保存的3D数据格式    
enum MV_SAVE_POINT_CLOUD_FILE_TYPE {
    MV_PointCloudFile_Undefined = 0,  // 未定义的点云格式    
    MV_PointCloudFile_PLY = 1,  // PLY点云格式         
    MV_PointCloudFile_CSV = 2,  // CSV点云格式         
    MV_PointCloudFile_OBJ = 3,  // OBJ点云格式         
};

// 保存3D数据到缓存    
typedef struct _MV_SAVE_POINT_CLOUD_PARAM_ {
    unsigned int nLinePntNum;                                   // [IN] 每一行点的数量，即图像宽     
    unsigned int nLineNum;                                      // [IN] 行数，即图像高               

    MvGvspPixelType enSrcPixelType;                             // [IN] 输入数据的像素格式           
    unsigned char *pSrcData;                                    // [IN] 输入数据缓存                 
    unsigned int nSrcDataLen;                                   // [IN] 输入数据大小                 

    unsigned char *pDstBuf;                                     // [OUT]输出像素数据缓存             
    unsigned int nDstBufSize;                                   // [IN] 提供的输出缓冲区大小(nLinePntNum * nLineNum * (16*3 + 4) + 2048) 
    unsigned int nDstBufLen;                                    // [OUT]输出像素数据缓存长度         
    MV_SAVE_POINT_CLOUD_FILE_TYPE enPointCloudFileType;         // [IN] 提供输出的点云文件类型       

    unsigned int nReserved[8];                                  // 保留                    

} MV_SAVE_POINT_CLOUD_PARAM;

// 保存图片格式    
enum MV_SAVE_IAMGE_TYPE {
    MV_Image_Undefined = 0,                                     // 未定义的图像格式      
    MV_Image_Bmp = 1,                                           // BMP图像格式           
    MV_Image_Jpeg = 2,                                          // JPEG图像格式          
    MV_Image_Png = 3,                                           // PNG图像格式           
    MV_Image_Tif = 4,                                           // TIFF图像格式          
};

// 图片保存参数    
typedef struct _MV_SAVE_IMAGE_PARAM_T_EX_ {
    unsigned char *pData;                                       // [IN]  输入数据缓存                     
    unsigned int nDataLen;                                      // [IN]  输入数据大小                     
    enum MvGvspPixelType enPixelType;                           // [IN]  输入数据的像素格式               
    unsigned short nWidth;                                      // [IN]  图像宽                           
    unsigned short nHeight;                                     // [IN]  图像高                           

    unsigned char *pImageBuffer;                                // [OUT] 输出图片缓存                     
    unsigned int nImageLen;                                     // [OUT] 输出图片大小                     
    unsigned int nBufferSize;                                   // [IN]  提供的输出缓冲区大小             
    enum MV_SAVE_IAMGE_TYPE enImageType;                        // [IN]  输出图片格式                     
    unsigned int nJpgQuality;                                   // [IN]  JPG编码质量(50-99]，其它格式无效 

    unsigned int iMethodValue;                                  // [IN]  插值方法                         
    unsigned int nReserved[3];                                  // 预留                             

} MV_SAVE_IMAGE_PARAM_EX;

// 保存BMP、JPEG、PNG、TIFF图片文件的参数    
typedef struct _MV_SAVE_IMG_TO_FILE_PARAM_ {
    enum MvGvspPixelType enPixelType;                           // [IN]  输入数据的像素格式               
    unsigned char *pData;                                       // [IN]  输入数据缓存                    
    unsigned int nDataLen;                                      // [IN]  输入数据大小                    
    unsigned short nWidth;                                      // [IN]  图像宽                          
    unsigned short nHeight;                                     // [IN]  图像高                          
    enum MV_SAVE_IAMGE_TYPE enImageType;                        // [IN]  输入图片格式                    
    unsigned int nQuality;                                      // [IN]  JPG编码质量(50-99]，PNG编码质量[0-9]，其它格式无效 
    char pImagePath[256];                                       // [IN]  输入文件路径                    

    int iMethodValue;                                           // [IN]  插值方法                        
    unsigned int nReserved[8];                                  // 预留                            

} MV_SAVE_IMG_TO_FILE_PARAM;

// 旋转角度    
typedef enum _MV_IMG_ROTATION_ANGLE_ {
    MV_IMAGE_ROTATE_90 = 1,
    MV_IMAGE_ROTATE_180 = 2,
    MV_IMAGE_ROTATE_270 = 3,

} MV_IMG_ROTATION_ANGLE;

// 图像旋转结构体    
typedef struct _MV_CC_ROTATE_IMAGE_PARAM_T_ {
    enum MvGvspPixelType enPixelType;                           // [IN]  像素格式(仅支持Mono8/RGB24/BGR24)  
    unsigned int nWidth;                                        // [IN][OUT]  \~chinese 图像宽                    
    unsigned int nHeight;                                       // [IN][OUT]  \~chinese 图像高                    

    unsigned char *pSrcData;                                    // [IN]  输入数据缓存                   
    unsigned int nSrcDataLen;                                   // [IN]  输入数据大小                   

    unsigned char *pDstBuf;                                     // [OUT] 输出数据缓存                   
    unsigned int nDstBufLen;                                    // [OUT] 输出数据大小                   
    unsigned int nDstBufSize;                                   // [IN]  提供的输出缓冲区大小           

    MV_IMG_ROTATION_ANGLE enRotationAngle;                      // [IN]  旋转角度                       

    unsigned int nRes[8];                                       // 预留                           
} MV_CC_ROTATE_IMAGE_PARAM;

// 翻转类型    
typedef enum _MV_IMG_FLIP_TYPE_ {
    MV_FLIP_VERTICAL = 1,
    MV_FLIP_HORIZONTAL = 2,

} MV_IMG_FLIP_TYPE;

// 图像翻转结构体    
typedef struct _MV_CC_FLIP_IMAGE_PARAM_T_ {
    enum MvGvspPixelType enPixelType;                           // [IN]  像素格式(仅支持Mono8/RGB24/BGR24)  
    unsigned int nWidth;                                        // [IN]  图像宽                         
    unsigned int nHeight;                                       // [IN]  图像高                         

    unsigned char *pSrcData;                                    // [IN]  输入数据缓存                   
    unsigned int nSrcDataLen;                                   // [IN]  输入数据大小                   

    unsigned char *pDstBuf;                                     // [OUT] 输出数据缓存                   
    unsigned int nDstBufLen;                                    // [OUT] 输出数据大小                   
    unsigned int nDstBufSize;                                   // [IN]  提供的输出缓冲区大小           

    MV_IMG_FLIP_TYPE enFlipType;                                // [IN]  翻转类型                       

    unsigned int nRes[8];                                       // 预留                           
} MV_CC_FLIP_IMAGE_PARAM;

// 图像转换结构体    
typedef struct _MV_PIXEL_CONVERT_PARAM_T_ {
    unsigned short nWidth;                                      // [IN]  图像宽                          
    unsigned short nHeight;                                     // [IN]  图像高                          

    enum MvGvspPixelType enSrcPixelType;                        // [IN]  源像素格式                      
    unsigned char *pSrcData;                                    // [IN]  输入数据缓存                    
    unsigned int nSrcDataLen;                                   // [IN]  输入数据大小                    

    enum MvGvspPixelType enDstPixelType;                        // [IN]  目标像素格式                    
    unsigned char *pDstBuffer;                                  // [OUT] 输出数据缓存                    
    unsigned int nDstLen;                                       // [OUT] 输出数据大小                    
    unsigned int nDstBufferSize;                                // [IN]  提供的输出缓冲区大小            

    unsigned int nRes[4];                                       // 预留                            
} MV_CC_PIXEL_CONVERT_PARAM;

// 色彩矫正参数    
typedef struct _MV_CC_CCM_PARAM_T_ {
    bool bCCMEnable;                                            // [IN]  色彩校正使能                    
    int nCCMat[9];                                              // [IN]  色彩矫正矩阵(-8192~8192)        

    unsigned int nRes[8];                                       // 预留                            
} MV_CC_CCM_PARAM;

// 噪声特性类型    
typedef enum _MV_CC_BAYER_NOISE_FEATURE_TYPE {
    MV_CC_BAYER_NOISE_FEATURE_TYPE_INVALID = 0,                 // 无效值
    MV_CC_BAYER_NOISE_FEATURE_TYPE_PROFILE = 1,                 // 噪声曲线
    MV_CC_BAYER_NOISE_FEATURE_TYPE_LEVEL = 2,                   // 噪声水平
    MV_CC_BAYER_NOISE_FEATURE_TYPE_DEFAULT = 1,                 // 默认值

} MV_CC_BAYER_NOISE_FEATURE_TYPE;

// Bayer格式降噪特性信息    
typedef struct _MV_CC_BAYER_NOISE_PROFILE_INFO_T_ {
    unsigned int nVersion;                                      // 版本
    MV_CC_BAYER_NOISE_FEATURE_TYPE enNoiseFeatureType;          // 噪声特性类型
    enum MvGvspPixelType enPixelType;                           // 图像格式
    int nNoiseLevel;                                            // 平均噪声水平
    unsigned int nCurvePointNum;                                // 曲线点数
    int *nNoiseCurve;                                           // 噪声曲线
    int *nLumCurve;                                             // 亮度曲线

    unsigned int nRes[8];                                       // 预留
} MV_CC_BAYER_NOISE_PROFILE_INFO;

// Bayer格式噪声估计参数    
typedef struct _MV_CC_BAYER_NOISE_ESTIMATE_PARAM_T_ {
    unsigned int nWidth;                                        //  图像宽(大于等于8)
    unsigned int nHeight;                                       //  图像高(大于等于8)
    enum MvGvspPixelType enPixelType;                           //  像素格式
    unsigned char *pSrcData;                                    //  输入数据缓存
    unsigned int nSrcDataLen;                                   //  输入数据大小
    unsigned int nNoiseThreshold;                               //  噪声阈值(0-4095)
    unsigned char *pCurveBuf;                                   //  用于存储噪声曲线和亮度曲线（需要外部分配，缓存大小：4096 * sizeof(int) * 2）
    MV_CC_BAYER_NOISE_PROFILE_INFO stNoiseProfile;              //  降噪特性信息
    unsigned int nThreadNum;                                    //  线程数量，0表示算法库根据硬件自适应；1表示单线程（默认）；大于1表示线程数目

    unsigned int nRes[8];                                       // 预留
} MV_CC_BAYER_NOISE_ESTIMATE_PARAM;

// Bayer格式空域降噪参数    
typedef struct _MV_CC_BAYER_SPATIAL_DENOISE_PARAM_T_ {
    unsigned int nWidth;                                        // [IN]  图像宽(大于等于8)
    unsigned int nHeight;                                       // [IN]  图像高(大于等于8)
    enum MvGvspPixelType enPixelType;                           // [IN]  像素格式

    unsigned char *pSrcData;                                    // [IN]  输入数据缓存
    unsigned int nSrcDataLen;                                   // [IN]  输入数据大小

    unsigned char *pDstBuf;                                     // [OUT] 输出降噪后的数据
    unsigned int nDstBufSize;                                   // [IN]  提供的输出缓冲区大小
    unsigned int nDstBufLen;                                    // [OUT] 输出降噪后的数据长度

    MV_CC_BAYER_NOISE_PROFILE_INFO stNoiseProfile;              // [IN]  降噪特性信息(来源于噪声估计)
    unsigned int nDenoiseStrength;                              // [IN]  降噪强度(0-100)
    unsigned int nSharpenStrength;                              // [IN]  锐化强度(0-32)
    unsigned int nNoiseCorrect;                                 // [IN]  噪声校正系数(0-1280)
    unsigned int nThreadNum;                                    // [IN]  线程数量，0表示算法库根据硬件自适应；1表示单线程（默认）；大于1表示线程数目

    unsigned int nRes[8];                                       // 预留

} MV_CC_BAYER_SPATIAL_DENOISE_PARAM;

// 水印信息      
typedef struct _MV_CC_FRAME_SPEC_INFO_ {
    // 设备水印时标    
    unsigned int nSecondCount;                                  // [OUT] 秒数
    unsigned int nCycleCount;                                   // [OUT] 周期数
    unsigned int nCycleOffset;                                  // [OUT] 周期偏移量

    float fGain;                                                // [OUT] 增益
    float fExposureTime;                                        // [OUT] 曝光时间
    unsigned int nAverageBrightness;                            // [OUT] 平均亮度

    // 白平衡相关    
    unsigned int nRed;                                          // [OUT] 红色
    unsigned int nGreen;                                        // [OUT] 绿色
    unsigned int nBlue;                                         // [OUT] 蓝色

    unsigned int nFrameCounter;                                 // [OUT] 总帧数
    unsigned int nTriggerIndex;                                 // [OUT] 触发计数

    unsigned int nInput;                                        // [OUT] 输入
    unsigned int nOutput;                                       // [OUT] 输出

    // ROI区域    
    unsigned short nOffsetX;                                    // [OUT] 水平偏移量
    unsigned short nOffsetY;                                    // [OUT] 垂直偏移量
    unsigned short nFrameWidth;                                 // [OUT] 水印宽
    unsigned short nFrameHeight;                                // [OUT] 水印高

    unsigned int nReserved[16];                                 // 预留
} MV_CC_FRAME_SPEC_INFO;

// 无损解码参数    
typedef struct _MV_CC_HB_DECODE_PARAM_T_ {
    unsigned char *pSrcBuf;                                     // [IN]  输入数据缓存
    unsigned int nSrcLen;                                       // [IN]  输入数据大小

    unsigned int nWidth;                                        // [OUT] 图像宽
    unsigned int nHeight;                                       // [OUT] 图像高
    unsigned char *pDstBuf;                                     // [OUT] 输出数据缓存
    unsigned int nDstBufSize;                                   // [IN]  提供的输出缓冲区大小
    unsigned int nDstBufLen;                                    // [OUT] 输出数据大小
    MvGvspPixelType enDstPixelType;                             // [OUT] 输出的像素格式
    MV_CC_FRAME_SPEC_INFO stFrameSpecInfo;                      // [OUT] 水印信息

    unsigned int nRes[8];                                       // 预留

} MV_CC_HB_DECODE_PARAM;

// 录像格式定义    
typedef enum _MV_RECORD_FORMAT_TYPE_ {
    MV_FormatType_Undefined = 0,                                // 未定义的录像格式
    MV_FormatType_AVI = 1,                                      // AVI录像格式

} MV_RECORD_FORMAT_TYPE;

// 录像参数    
typedef struct _MV_CC_RECORD_PARAM_T_ {
    enum MvGvspPixelType enPixelType;                           // [IN]  输入数据的像素格式

    unsigned short nWidth;                                      // [IN]  图像宽(指定目标参数时需为2的倍数)
    unsigned short nHeight;                                     // [IN]  图像高(指定目标参数时需为2的倍数)

    float fFrameRate;                                           // [IN]  帧率fps(大于1/16)
    unsigned int nBitRate;                                      // [IN]  码率kbps(128kbps-16Mbps)

    MV_RECORD_FORMAT_TYPE enRecordFmtType;                      // [IN]  录像格式

    char *strFilePath;                                          // [IN]  录像文件存放路径(如果路径中存在中文，需转成utf-8)

    unsigned int nRes[8];                                       // 预留

} MV_CC_RECORD_PARAM;

// 传入的图像数据    
typedef struct _MV_CC_INPUT_FRAME_INFO_T_ {
    unsigned char *pData;                                       // [IN]  图像数据指针
    unsigned int nDataLen;                                      // [IN]  图像大小

    unsigned int nRes[8];                                       // 预留

} MV_CC_INPUT_FRAME_INFO;

// 采集模式    
typedef enum _MV_CAM_ACQUISITION_MODE_ {
    MV_ACQ_MODE_SINGLE = 0,                                     // 单帧模式
    MV_ACQ_MODE_MUTLI = 1,                                      // 多帧模式
    MV_ACQ_MODE_CONTINUOUS = 2,                                 // 持续采集模式

} MV_CAM_ACQUISITION_MODE;

// 增益模式    
typedef enum _MV_CAM_GAIN_MODE_ {
    MV_GAIN_MODE_OFF = 0,                                       // 关闭
    MV_GAIN_MODE_ONCE = 1,                                      // 一次
    MV_GAIN_MODE_CONTINUOUS = 2,                                // 连续

} MV_CAM_GAIN_MODE;

// 曝光模式    
typedef enum _MV_CAM_EXPOSURE_MODE_ {
    MV_EXPOSURE_MODE_TIMED = 0,                                 // 时间
    MV_EXPOSURE_MODE_TRIGGER_WIDTH = 1,                         // 触发脉冲宽度
} MV_CAM_EXPOSURE_MODE;

// 自动曝光模式    
typedef enum _MV_CAM_EXPOSURE_AUTO_MODE_ {
    MV_EXPOSURE_AUTO_MODE_OFF = 0,                              // 关闭
    MV_EXPOSURE_AUTO_MODE_ONCE = 1,                             // 一次
    MV_EXPOSURE_AUTO_MODE_CONTINUOUS = 2,                       // 连续

} MV_CAM_EXPOSURE_AUTO_MODE;

// 触发模式    
typedef enum _MV_CAM_TRIGGER_MODE_ {
    MV_TRIGGER_MODE_OFF = 0,                                    // 关闭
    MV_TRIGGER_MODE_ON = 1,                                     // 打开

} MV_CAM_TRIGGER_MODE;

// Gamma选择器    
typedef enum _MV_CAM_GAMMA_SELECTOR_ {
    MV_GAMMA_SELECTOR_USER = 1,                                 // 用户
    MV_GAMMA_SELECTOR_SRGB = 2,                                 // sRGB

} MV_CAM_GAMMA_SELECTOR;

// 白平衡    
typedef enum _MV_CAM_BALANCEWHITE_AUTO_ {
    MV_BALANCEWHITE_AUTO_OFF = 0,                               // 关闭
    MV_BALANCEWHITE_AUTO_ONCE = 2,                              // 一次
    MV_BALANCEWHITE_AUTO_CONTINUOUS = 1,                        // 连续

} MV_CAM_BALANCEWHITE_AUTO;

// 触发源    
typedef enum _MV_CAM_TRIGGER_SOURCE_ {
    MV_TRIGGER_SOURCE_LINE0 = 0,                                // Line0
    MV_TRIGGER_SOURCE_LINE1 = 1,                                // Line1
    MV_TRIGGER_SOURCE_LINE2 = 2,                                // Line2
    MV_TRIGGER_SOURCE_LINE3 = 3,                                // Line3
    MV_TRIGGER_SOURCE_COUNTER0 = 4,                             // Conuter0

    MV_TRIGGER_SOURCE_SOFTWARE = 7,                             // 软触发
    MV_TRIGGER_SOURCE_FrequencyConverter = 8,                   // 变频器

} MV_CAM_TRIGGER_SOURCE;

// GigEVision IP配置    
#define MV_IP_CFG_STATIC        0x05000000                      // 静态
#define MV_IP_CFG_DHCP          0x06000000                      // DHCP
#define MV_IP_CFG_LLA           0x04000000                      // LLA

// GigEVision网络传输模式    
#define MV_NET_TRANS_DRIVER     0x00000001                      // 驱动
#define MV_NET_TRANS_SOCKET     0x00000002                      // Socket

// CameraLink波特率    
#define MV_CAML_BAUDRATE_9600            0x00000001             // 9600
#define MV_CAML_BAUDRATE_19200           0x00000002             // 19200
#define MV_CAML_BAUDRATE_38400           0x00000004             // 38400
#define MV_CAML_BAUDRATE_57600           0x00000008             // 57600
#define MV_CAML_BAUDRATE_115200          0x00000010             // 115200
#define MV_CAML_BAUDRATE_230400          0x00000020             // 230400
#define MV_CAML_BAUDRATE_460800          0x00000040             // 460800
#define MV_CAML_BAUDRATE_921600          0x00000080             // 921600
#define MV_CAML_BAUDRATE_AUTOMAX         0x40000000             // 最大值

// 异常消息类型    
#define MV_EXCEPTION_DEV_DISCONNECT     0x00008001              // 设备断开连接
#define MV_EXCEPTION_VERSION_CHECK      0x00008002              // SDK与驱动版本不匹配

// Event事件回调信息
#define MAX_EVENT_NAME_SIZE     128                             // 设备Event事件名称最大长度
typedef struct _MV_EVENT_OUT_INFO_ {
    char EventName[MAX_EVENT_NAME_SIZE];                    // [OUT] Event名称

    unsigned short nEventID;                                // [OUT] Event号
    unsigned short nStreamChannel;                          // [OUT] 流通道序号

    unsigned int nBlockIdHigh;                              // [OUT] 帧号高位
    unsigned int nBlockIdLow;                               // [OUT] 帧号低位

    unsigned int nTimestampHigh;                            // [OUT] 时间戳高位
    unsigned int nTimestampLow;                             // [OUT] 时间戳低位

    void *pEventData;                                       // [OUT] Event数据
    unsigned int nEventDataSize;                            // [OUT] Event数据长度

    unsigned int nReserved[16];                             // 预留
} MV_EVENT_OUT_INFO;

// 文件存取    
typedef struct _MV_CC_FILE_ACCESS_T {
    const char *pUserFileName;                              // [IN]  用户文件名
    const char *pDevFileName;                               // [IN]  设备文件名

    unsigned int nReserved[32];                             // 预留
} MV_CC_FILE_ACCESS;

// 文件存取进度    
typedef struct _MV_CC_FILE_ACCESS_PROGRESS_T {
    int64_t nCompleted;                                     // [OUT] 已完成的长度
    int64_t nTotal;                                         // [OUT] 总长度

    unsigned int nReserved[8];                              // 预留
} MV_CC_FILE_ACCESS_PROGRESS;

// Gige的传输类型    
typedef enum _MV_GIGE_TRANSMISSION_TYPE_ {
    MV_GIGE_TRANSTYPE_UNICAST = 0x0,                        // 表示单播(默认)
    MV_GIGE_TRANSTYPE_MULTICAST = 0x1,                      // 表示组播
    MV_GIGE_TRANSTYPE_LIMITEDBROADCAST = 0x2,               // 表示局域网内广播，暂不支持
    MV_GIGE_TRANSTYPE_SUBNETBROADCAST = 0x3,                // 表示子网内广播，暂不支持
    MV_GIGE_TRANSTYPE_CAMERADEFINED = 0x4,                  // 表示从设备获取，暂不支持
    MV_GIGE_TRANSTYPE_UNICAST_DEFINED_PORT = 0x5,           // 表示用户自定义应用端接收图像数据Port号
    MV_GIGE_TRANSTYPE_UNICAST_WITHOUT_RECV = 0x00010000,    // 表示设置了单播，但本实例不接收图像数据
    MV_GIGE_TRANSTYPE_MULTICAST_WITHOUT_RECV = 0x00010001,  // 表示组播模式，但本实例不接收图像数据
} MV_GIGE_TRANSMISSION_TYPE;

// 传输模式，可以为单播模式、组播模式等    
typedef struct _MV_TRANSMISSION_TYPE_T {
    MV_GIGE_TRANSMISSION_TYPE enTransmissionType; // [IN]  传输模式
    unsigned int nDestIp;                         // [IN]  目标IP，组播模式下有意义
    unsigned short nDestPort;                     // [IN]  目标Port，组播模式下有意义

    unsigned int nReserved[32];                   // 预留
} MV_TRANSMISSION_TYPE;

// 动作命令信息    
typedef struct _MV_ACTION_CMD_INFO_T {
    unsigned int nDeviceKey;                      // [IN]  设备密钥
    unsigned int nGroupKey;                       // [IN]  组键
    unsigned int nGroupMask;                      // [IN]  组掩码

    unsigned int bActionTimeEnable;                                       // [IN]  只有设置成1时Action Time才有效，非1时无效
    int64_t nActionTime;                                                  // [IN]  预定的时间，和主频有关

    const char *pBroadcastAddress;                                        // [IN]  广播包地址
    unsigned int nTimeOut;                                                // [IN]  等待ACK的超时时间，如果为0表示不需要ACK

    unsigned int bSpecialNetEnable;                                       // [IN]  只有设置成1时指定的网卡IP才有效，非1时无效
    unsigned int nSpecialNetIP;                                           // [IN]  指定的网卡IP

    unsigned int nReserved[14];                                           // 预留

} MV_ACTION_CMD_INFO;

// 动作命令返回信息    
typedef struct _MV_ACTION_CMD_RESULT_T {
    unsigned char strDeviceAddress[12 + 3 + 1];                           // [OUT] 设备IP
    int nStatus;                                                          // [OUT] 状态码

    unsigned int nReserved[4];                                            // 预留

} MV_ACTION_CMD_RESULT;

// 动作命令返回信息列表    
typedef struct _MV_ACTION_CMD_RESULT_LIST_T {
    unsigned int nNumResults;                                             // [OUT] 返回值个数
    MV_ACTION_CMD_RESULT *pResults;                                       // [OUT] 动作命令结果

} MV_ACTION_CMD_RESULT_LIST;

// 每个节点对应的接口类型    
enum MV_XML_InterfaceType {
    IFT_IValue,                                                 // Value
    IFT_IBase,                                                  // Base
    IFT_IInteger,                                               // Integer
    IFT_IBoolean,                                               // Boolean
    IFT_ICommand,                                               // Command
    IFT_IFloat,                                                 // Float
    IFT_IString,                                                // String
    IFT_IRegister,                                              // Register
    IFT_ICategory,                                              // Category
    IFT_IEnumeration,                                           // Enumeration
    IFT_IEnumEntry,                                             // EnumEntry
    IFT_IPort,                                                  // Port
};

// 节点的访问模式    
enum MV_XML_AccessMode {
    AM_NI,                                                      // 不可实现
    AM_NA,                                                      // 不可用
    AM_WO,                                                      // 只写
    AM_RO,                                                      // 只读
    AM_RW,                                                      // 读写
    AM_Undefined,                                               // 未定义
    AM_CycleDetect,                                             // 内部用于AccessMode循环检测
};

#define MV_MAX_XML_SYMBOLIC_NUM      64                         // 最大XML符号数
// 枚举类型值    
typedef struct _MVCC_ENUMVALUE_T {
    unsigned int nCurValue;                                     // [OUT] 当前值
    unsigned int nSupportedNum;                                 // [OUT] 数据的有效数据个数
    unsigned int nSupportValue[MV_MAX_XML_SYMBOLIC_NUM];        // [OUT] 支持的枚举值

    unsigned int nReserved[4];                                  // 预留
} MVCC_ENUMVALUE;

// Int类型值    
typedef struct _MVCC_INTVALUE_T {
    unsigned int nCurValue;                                     // [OUT] 当前值
    unsigned int nMax;                                          // [OUT] 最大值
    unsigned int nMin;                                          // [OUT] 最小值
    unsigned int nInc;                                          // [OUT]

    unsigned int nReserved[4];                                  // 预留
} MVCC_INTVALUE;

// Int类型值Ex    
typedef struct _MVCC_INTVALUE_EX_T {
    int64_t nCurValue;                                          // [OUT] 当前值
    int64_t nMax;                                               // [OUT] 最大值
    int64_t nMin;                                               // [OUT] 最小值
    int64_t nInc;                                               // [OUT] Inc

    unsigned int nReserved[16];                                 // 预留
} MVCC_INTVALUE_EX;

// Float类型值    
typedef struct _MVCC_FLOATVALUE_T {
    float fCurValue;                                            // [OUT] 当前值
    float fMax;                                                 // [OUT] 最大值
    float fMin;                                                 // [OUT] 最小值

    unsigned int nReserved[4];                                  // 预留
} MVCC_FLOATVALUE;

// String类型值    
typedef struct _MVCC_STRINGVALUE_T {
    char chCurValue[256];                                       // [OUT] 当前值

    int64_t nMaxLength;                                         // [OUT] 最大长度
    unsigned int nReserved[2];                                  // 预留
} MVCC_STRINGVALUE;

#endif /* _MV_CAMERA_PARAMS_H_ */
