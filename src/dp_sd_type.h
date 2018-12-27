// #include "dp_sd_type.h"
#ifndef _DP_SD_TYPE_H
#define _DP_SD_TYPE_H

typedef unsigned char 	DP_U8;
typedef unsigned short 	DP_U16;
typedef unsigned int 	DP_U32;
typedef signed char 	DP_S8;
typedef short 			DP_S16;
typedef int 			DP_S32;

#ifndef _M_IX86
typedef unsigned long long DP_U64;
typedef long long DP_S64;
#else
typedef __int64 DP_U64;
typedef __int64 DP_S64;
#endif
typedef char DP_CHAR;

//#define void DP_VOID

/*----------------------------------------------*
* const defination *
*----------------------------------------------*/
typedef enum {
	
	DP_FALSE = 0,
	DP_TRUE = 1,
} DP_BOOL;

//#ifndef NULL
//#define NULL 0L
//#endif
//
//#define HI_NULL 0L
//#define HI_SUCCESS 0
//#define HI_FAILURE (-1)
//
//
//#define DP_U32 DP_AI_DEV	/* 音频输入设备 */
//#define DP_U32 DP_AO_DEV	/* 音频输出设备 */
//#define DP_U32 DP_AENC_CHN	/* 音频编码通道 */
//#define DP_U32 DP_ADEC_CHN	/* 音频解码通道 */
//
//#define DP_U32 DP_VI_DEV	/* 视频输入设备 */
//#define DP_U32 DP_VO_DEV	/* 视频输出设备 */
//
//#define DP_U32 DP_VENC_CHN	/* 视频编码通道 */
//#define DP_U32 DP_VDEC_CHN	/* 视频解码通道 */
//
//#define DP_U32 DP_AVENC_CHN	/* 音视频编码通道 */
//#define DP_U32 DP_AVDEC_CHN	/* 音视频解码通道 */
//
//#define DP_U32 DP_SWMS_CHN	/* 拼接窗口通道 */

#endif
