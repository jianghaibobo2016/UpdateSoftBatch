/*
 * Screen.h
 *
 *  Created on: Dec 25, 2018
 *      Author: jhb
 */

#ifndef SRC_SCREEN_H_
#define SRC_SCREEN_H_
#include <ncurses.h>
#include <locale.h>
#include <map>
#include <vector>
#include <string>
#include <algorithm>//注意要包含该头文件
#include <muduo/base/Mutex.h>
#include <muduo/base/CountDownLatch.h>
#include "dp_sd_type.h"
#include "interactivepro.h"

#define SEQOFFSET  6
#define PROCESSOFFSET  25
#define STATUSOFFSET  17

#define TOTALTIME 30

typedef std::map<DP_U32, DP_U32> MapUpdateStatus;
typedef std::vector<std::string> VectWrongIP;

static const DP_CHAR *SeqName = "序号";
static const DP_CHAR *ListName = "更新列表";
static const DP_CHAR *ProcessName = "更新进度";
static const DP_CHAR *StatusName = "更新状态";

static const DP_CHAR *SuccessName = "更新完成";
static const DP_CHAR *UpdatingName = "更新中..";
static const DP_CHAR *FailName = "更新失败";
static const DP_CHAR *ConnectName = "连接中";
static const DP_CHAR *ConFailName = "连接失败";
static const DP_CHAR *IPWrongName = "IP地址错误";
static const DP_CHAR *ZeorName = "0%";
static const DP_CHAR *FiftyName = "50%";
static const DP_CHAR *CompleteName = "100%";

static const DP_CHAR *Symbol = "#";

#pragma pack(push)
#pragma pack(1)
typedef struct _UpdateInfo_S {
	_UpdateInfo_S(DP_U32 srcIP) :
			u32SrcIP(srcIP) {
		header.stFunctionMsg.u8CommandID = Command_UpdateBatch;
	}
	_sRemote_Header header;
	DP_U32 u32SrcIP;  //for tftp
} UpdateInfo_S;

typedef struct _UpdateStatus_S {
	_UpdateStatus_S(DP_U32 sucess) :
			_success(sucess) {
		header.stFunctionMsg.u8CommandID = Command_UpdateBatch;
	}
	_UpdateStatus_S() :
			_success() {
		header.stFunctionMsg.u8CommandID = Command_UpdateBatch;
	}
	_sRemote_Header header;
	DP_U32 _success;  //for tftp
} UpdateStatus_S;

#pragma pack(pop)

class Screen {
public:
	Screen() {
	}
	~Screen() {
	}

public:

//	static int TileColour[] = {
//	COLOR_RED,
//	COLOR_GREEN,
//	COLOR_YELLOW,
//	COLOR_BLUE,
//	COLOR_MAGENTA,
//	COLOR_CYAN, };

	static void initial(); /* 自定开启 curses 函式 */

	static void headPrint();

	static void seqPrint();

	static void ipListPrint();

	static void updateStatusPrint(DP_U32 dstIP, const DP_CHAR *status);

	static void wrongIPPrint();

	static void symbolPrint();

	static void persentPrint(DP_U32 dstIP, const DP_CHAR *status);

	static void connectFailPrint();

	static DP_BOOL checkIP(const DP_CHAR* ipaddr);

private:
	static DP_BOOL _inited;
//	muduo::CountDownLatch _latch;

};

class UpdateStatus {
public:

	inline MapUpdateStatus getUpdateStatus() const {
		muduo::MutexLockGuard lock(_mutex);
		return _mDstIPUpdateStauts;
	}

	inline VectWrongIP getWrongIP() const {
		muduo::MutexLockGuard lock(_mutex);
		return _vWrongIP;
	}

	inline void updateUpdateStatus(MapUpdateStatus newData) {
		muduo::MutexLockGuard lock(_mutex);
		_mDstIPUpdateStauts.swap(newData);
	}

	inline void updateWrongIP(VectWrongIP newData) {
		muduo::MutexLockGuard lock(_mutex);
		_vWrongIP.swap(newData);
	}

private:
	mutable muduo::MutexLock _mutex;
	//ip status
	MapUpdateStatus _mDstIPUpdateStauts;
	VectWrongIP _vWrongIP;
};

#endif /* SRC_SCREEN_H_ */
