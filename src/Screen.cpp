/*
 * Screen.cpp
 *
 *  Created on: Dec 25, 2018
 *      Author: jhb
 */
#include "Screen.h"
#include <stdlib.h>
#include <vector>
#include <algorithm>//注意要包含该头文件
#include <arpa/inet.h>
#include <muduo/base/Singleton.h>
#include <muduo/base/Logging.h>
#include <muduo/base/CurrentThread.h>
std::vector<DP_U32> g_DstIPAddr;
DP_BOOL Screen::_inited = DP_FALSE;

void Screen::initial() /* 自定开启 curses 函式 */
{
	setlocale(LC_ALL, "");
	initscr();
	cbreak();
	nonl();
	noecho();
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);
	if (has_colors() == FALSE) {
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	start_color(); /* Start color 			*/
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_CYAN, COLOR_BLACK);
	init_pair(6, COLOR_WHITE, COLOR_BLACK);
	init_pair(7, COLOR_RED, COLOR_YELLOW);

	attron(COLOR_PAIR(4));
	box(stdscr, '|', '-');
	refresh();
	_inited = DP_TRUE;
}

void Screen::headPrint() {
	if (!_inited) {
		return;
	}
	/*LINES和COLS是Curses内部变量,用于存储当前终端的行数和列数*/
	if (LINES < 20 || COLS < 75) /*标准的终端为24行80列*/
	{
		endwin();
		fprintf(stderr, "当前小于 20x75 \n");
		return;
	}
	int posSeq[] = { 1, 1 };
	int posList[] = { 1, SEQOFFSET };
	int posProcess[] = { 1, PROCESSOFFSET };
	int posStatus[] = { 1, COLS / 3 };
	mvaddstr(posSeq[0], posSeq[1], SeqName);
	mvaddstr(posList[0], posList[1], ListName);
	mvaddstr(posProcess[0], posProcess[1], ProcessName);
	mvaddstr(posStatus[0], posStatus[1], StatusName);
	refresh();
}

void Screen::seqPrint() {
	if (!_inited) {
		return;
	}
	attron(COLOR_PAIR(5));
	for (DP_U32 i = 0; i < g_DstIPAddr.size(); i++) {
		mvwprintw(stdscr, i + 2, 1, "%d", i + 1);
	}
	refresh();
}

void Screen::ipListPrint() {
	if (!_inited) {
		return;
	}
	attron(COLOR_PAIR(5));
	struct in_addr addr;
	for (DP_U32 i = 0; i < g_DstIPAddr.size(); i++) {
		addr.s_addr = g_DstIPAddr[i];
		mvaddstr(i + 2, SEQOFFSET, inet_ntoa(addr));
	}
	refresh();
}

void Screen::updateStatusPrint(DP_U32 dstIP, const DP_CHAR *status) {
	if (!_inited) {
		return;
	}
//	std::cout << "1111111111111<" << std::endl;
	std::vector<DP_U32>::iterator it;
	attron(COLOR_PAIR(6));
	if ((it = find(g_DstIPAddr.begin(), g_DstIPAddr.end(), dstIP))
			!= g_DstIPAddr.end()) {
		DP_S32 offset = std::distance(g_DstIPAddr.begin(), it);
//		std::cout << "fffff: " << offset << " " << COLS - STATUSOFFSET
//				<< " "<<status<<std::endl;
		mvaddstr(offset + 2, COLS / 3, status);
	}
	refresh();
}

void Screen::wrongIPPrint() {
	if (!_inited) {
		return;
	}
	VectWrongIP wrongIP =
			muduo::Singleton<UpdateStatus>::instance().getWrongIP();
	DP_U32 ipCount = g_DstIPAddr.size();
	attron(COLOR_PAIR(7));
	for (DP_U32 offset = 0; offset < wrongIP.size(); offset++) {
		mvwprintw(stdscr, ipCount + offset + 2, 1, "%d", ipCount + offset + 1);
		mvaddstr(ipCount + offset + 2, SEQOFFSET, wrongIP[offset].c_str());
		mvaddstr(ipCount + offset + 2, COLS / 3, IPWrongName);
	}
	refresh();
}

void Screen::symbolPrint() {
	std::vector<DP_U32>::iterator it;
	DP_S32 offset = 0;
	attron(COLOR_PAIR(1));
	for (it = g_DstIPAddr.begin(); it != g_DstIPAddr.end(); it++) {
		DP_S32 offset = std::distance(g_DstIPAddr.begin(), it);
		for (DP_S32 j = PROCESSOFFSET;
				j < ((COLS / 3 - 3 - PROCESSOFFSET) / 3 + PROCESSOFFSET); j++) {
			mvaddstr(offset + 2, j, Symbol);
		}
		for (DP_S32 j = (((COLS / 3 - 3 - PROCESSOFFSET) / 3) * 2
				+ PROCESSOFFSET); j < COLS / 3 - 3; j++) {
			attron(COLOR_PAIR(1));
			mvaddstr(offset + 2, j, Symbol);
		}
	}
	refresh();
}

void Screen::persentPrint(DP_U32 dstIP, const DP_CHAR *status) {
	attron(COLOR_PAIR(1));
	std::vector<DP_U32>::iterator it;
	DP_U32 end = (((COLS / 3 - 3 - PROCESSOFFSET) / 3) * 2 + PROCESSOFFSET);
	DP_U32 start = ((COLS / 3 - 3 - PROCESSOFFSET) / 3 + PROCESSOFFSET);
	DP_U32 newStart = (end - start) / 2 + start - 1;
	if ((it = find(g_DstIPAddr.begin(), g_DstIPAddr.end(), dstIP))
			!= g_DstIPAddr.end()) {
		DP_S32 offset = std::distance(g_DstIPAddr.begin(), it);
		mvaddstr(offset + 2, newStart, status);
	}
	refresh();
}

void Screen::connectFailPrint() {
	muduo::CurrentThread::sleepUsec(30000 * 1000);
//	std::vector<DP_U32>::iterator itIP;
	MapUpdateStatus::iterator itM;
	DP_S32 offset = 0;
	attron(COLOR_PAIR(2));
	MapUpdateStatus status =
			muduo::Singleton<UpdateStatus>::instance().getUpdateStatus();

	std::vector<DP_U32>::iterator it;
	for (it = g_DstIPAddr.begin(); it != g_DstIPAddr.end(); it++) {
		if ((itM = status.find(*it)) != status.end()) {
			if (itM->second != 0) {
				offset = std::distance(g_DstIPAddr.begin(), it);
				mvaddstr(offset + 2, COLS / 3, FailName);
				refresh();
			}
		} else {
			offset = std::distance(g_DstIPAddr.begin(), it);
			mvaddstr(offset + 2, COLS / 3, ConFailName);
			refresh();
		}
	}

//	for (MapUpdateStatus::iterator it = status.begin(); it != status.end();
//			it++) {
//		if (it->second != 0) {
//			if ((itIP = find(g_DstIPAddr.begin(), g_DstIPAddr.end(), it->first))
//					!= g_DstIPAddr.end()) {
//				DP_S32 offset = std::distance(g_DstIPAddr.begin(), itIP);
//				mvaddstr(offset + 2, COLS / 3, FailName);
//				refresh();
//			}
//		}
//	}
}

DP_BOOL Screen::checkIP(const DP_CHAR* ipaddr) {
	const DP_CHAR* ptrIP = ipaddr;
//	cout << "ptrIP: " << ptrIP << endl;
	const DP_CHAR* dot = ".";
	DP_CHAR** section;
	section = new DP_CHAR *[4];
	for (DP_S32 i = 0; i < 4; i++) {
		section[i] = new DP_CHAR[3];
		memset(section[i], 0, 3);
	}

// INT8*
	DP_S32 dotIndex = 0, chIndex = 0, ipLen = strlen(ptrIP);
	if (NULL == ipaddr) {
		LOG_ERROR<<"IP is wrong: "<<ipaddr;
		delete[] section;
		return DP_FALSE;
	}

	/* segmentation  and check number*/
	do {

		if (ptrIP[0] == ' ') {
			LOG_ERROR<<"IP is wrong: "<<ipaddr;
			delete[] section;
			return DP_FALSE;
		}

		if (strncmp(ptrIP, dot, 1) != 0) {

			if (isdigit(ptrIP[0])) {
				if (dotIndex > 3 || chIndex > 2) {
					LOG_ERROR<<"IP is wrong: "<<ipaddr;
					delete[] section;
					return DP_FALSE;
				}
				memset(&section[dotIndex][chIndex], ptrIP[0], 1);

				ptrIP += 1;
				chIndex++;
				ipLen--;
			} else if (ptrIP[0] == '\0')
			break;
			else {
				LOG_ERROR<<"IP is wrong: "<<ipaddr;
				delete[] section;
				return DP_FALSE;
			}
		} else {
			ptrIP += 1;
			dotIndex++;
			chIndex = 0;
		}
	}while (ipLen > 0);

	if (dotIndex != 3) {
		LOG_ERROR<<"IP is wrong: "<<ipaddr;
		delete[] section;
		return DP_FALSE;
	}

	/* check number */
	while (dotIndex >= 0) {
		if (atoi(section[dotIndex]) >= 255 || atoi(section[dotIndex]) < 0
				|| strlen(section[dotIndex]) == 0) {
			LOG_ERROR<<"IP is wrong: "<<ipaddr;
			delete[] section;
			return DP_FALSE;
		}
		dotIndex--;
	}
	if (atoi(section[0]) == 0 && atoi(section[1]) == 0 && atoi(section[2]) == 0
			&& atoi(section[3]) == 0) {
		LOG_ERROR<<"IP is wrong: "<<ipaddr;
		delete[] section;
		return DP_FALSE;
	}

	if (INADDR_NONE == inet_addr(ipaddr)) {
		LOG_ERROR<<"IP is wrong: "<<ipaddr;
		delete[] section;
		return DP_FALSE;
	}
	delete[] section;
	return DP_TRUE;
}
