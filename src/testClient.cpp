/*
 * testClient.cpp
 *
 *  Created on: Oct 19, 2018
 *      Author: jhb
 */
#include "testClient.h"

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <boost/bind.hpp>
#include <muduo/net/Buffer.h>
#include <muduo/base/Singleton.h>
#include <muduo/base/ThreadPool.h>
#include <muduo/net/EventLoopThreadPool.h>
#include <EventLoop.h>
#include <vector>
#include <arpa/inet.h>
#include <ncurses.h>
#include <locale.h>

using namespace std;

extern std::vector<DP_U32> g_DstIPAddr;

static struct option long_options[] = { { "src", required_argument, 0, 's' }, {
		"dst", required_argument, 0, 'd' },
		{ "help", required_argument, 0, 'h' }, { 0, 0, 0, 0 } };
static char simple_options[] = "s:d:h";

void usage() {
	printf(
			"Usage: ./exe --src [srcIPAddr] [port] --dst [dstIPAddr] [dstIPAddr_2] [dstIPAddr_3] ... \n");
}

void screenPresent();
int main(int argc, char* argv[]) {

	int c;
	//need tcp ip addr and port.
	if (argc <= 4) {
		usage();
		return 0;
	}
//	EventLoopThread loopThread;
	EventLoop loop;
	EventLoopThreadPool loopThread(&loop, "UpdateBatch");
	loopThread.setThreadNum(2);
	loopThread.start();
	uint16_t port = 0;
//	uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
//	InetAddress serverAddr(argv[1], port);
//	cout << "argc: " << argc << " argv: " << argv[0] << endl;
//	argc -= 2;
//	argv += 2;
//	cout << "argc: " << argc << " argv: " << argv[0] << endl;
//#if 1
//	ChatClient client(loopThread.startLoop(), serverAddr);
//	boost::shared_ptr<ChatClient> client;
	boost::ptr_vector<ChatClient> clients;
	VectWrongIP wrongIP =
			muduo::Singleton<UpdateStatus>::instance().getWrongIP();

	DP_BOOL isInit = DP_FALSE;

	DP_U32 tftpSrcIp = 0;
//	client.connect();
//	CurrentThread::sleepUsec(300 * 1000);
//#endif

//	client.write("厉害厉害");
//	CurrentThread::sleepUsec(300 * 1000);
//	client.disconnect();
//	CurrentThread::sleepUsec(300 * 1000);
//	return 0;
	while (1) {
		int this_option_optind = optind ? optind : 1;
		int longindex = -1;
		c = getopt_long(argc, argv, simple_options, long_options, &longindex);
		if (c == -1) {
			break;
		}
		switch (c) {
		case 's': {
			tftpSrcIp = inet_addr(optarg);
			//check ip
			optarg += strlen(optarg) + 1;
			port = static_cast<uint16_t>(atoi(optarg));
//			InetAddress serverAddr(ipSrc, port);
//			client.reset(new ChatClient(loopThread.startLoop(), serverAddr));
			isInit = DP_TRUE;
		}

			break;
		case 'd': {
			if (isInit) {
				g_DstIPAddr.clear();
				for (DP_S32 count = 0; count < argc - 5; count++) {

					if (Screen::checkIP(optarg) == DP_TRUE) {
						g_DstIPAddr.push_back(inet_addr(optarg));
						optarg += strlen(optarg) + 1;
					} else {
						wrongIP.push_back(optarg);
						optarg += strlen(optarg) + 1;
						continue;
					}

				}
				struct in_addr addr;
				for (std::vector<DP_U32>::iterator it = g_DstIPAddr.begin();
						it != g_DstIPAddr.end(); it++) {
//					cout << "isdst: " << *it << " port: " << port << endl;
					addr.s_addr = *it;
					InetAddress serverAddr((DP_CHAR *) inet_ntoa(addr), port);
					clients.push_back(
							new ChatClient(loopThread.getNextLoop(),
									serverAddr));
					UpdateInfo_S info(tftpSrcIp);
//					cout << "srcIP: " << tftpSrcIp << "info:  " << info.u32SrcIP
//							<< endl;
//					cout << "sizeof(UpdateInfo_S)<< " << sizeof(UpdateInfo_S)
//							<< endl;
					StringPiece str((DP_CHAR *) &info, sizeof(UpdateInfo_S));
					clients.back().connect();
					CurrentThread::sleepUsec(20 * 1000);
					clients.back().write(str);
//					CurrentThread::sleepUsec(50 * 1000);
				}
			} else {

			}
			muduo::Singleton<UpdateStatus>::instance().updateWrongIP(wrongIP);
		}
			break;
		case 'h': {
			usage();
		}
			break;
		default:
			break;
//		std::cout << "?? getopt returned character code 0%o ??" << c
//		<< std::endl;
		} // switch
	}	 // while

//	muduo::ThreadPool threadPool("UpdateBatch");
//	threadPool.setMaxQueueSize(3);
//	threadPool.start(3);
	CurrentThread::sleepUsec(100 * 1000);
	Screen::initial();
	Screen::headPrint();
//	std::vector<DP_U32>::iterator it;
//	it = find(g_DstIPAddr.begin(), g_DstIPAddr.end(), 1550061740);
//	for (DP_U32 i = 0; i < g_DstIPAddr.size(); i++) {
//		cout << "ip: " << g_DstIPAddr[i] << " " << endl;
//
//	}
//	if (it == g_DstIPAddr.end()) {
//		cout << "ffffffffffffffffffffF" << endl;
//	}
//	cout << "dis : " << std::distance(g_DstIPAddr.begin(), it) << endl;

	Screen::ipListPrint();
//	loopThread.getNextLoop()->runInLoop(boost::bind(Screen::symbolPrint));
	Screen::symbolPrint();
	Screen::seqPrint();
	MapUpdateStatus status =
			muduo::Singleton<UpdateStatus>::instance().getUpdateStatus();
//	cout << " statussize: " << status.size();

//	for (MapUpdateStatus::iterator it = status.begin(); it != status.end();
//			it++) {
//		cout << " first: " << it->first << " second: " << it->second << endl;
//	}
	DP_U32 ack = 3;
	for (DP_U32 i = 0; i < g_DstIPAddr.size(); i++) {
		if (status.find(g_DstIPAddr[i]) != status.end()) {
			if (status[g_DstIPAddr[i]] == ack) {
				Screen::persentPrint(g_DstIPAddr[i], FiftyName);
				Screen::updateStatusPrint(g_DstIPAddr[i], UpdatingName);
			}
		} else {
			Screen::persentPrint(g_DstIPAddr[i], ZeorName);
			Screen::updateStatusPrint(g_DstIPAddr[i], ConnectName);
		}
	}
	Screen::wrongIPPrint();
	Screen::connectFailPrint();
//	screenPresent();
//	client->disconnect();
//	cout<<"sizeof(UpdfffffffffffffffffffffateInfo_S)<< "<<sizeof(UpdateInfo_S)<<endl;
	loop.loop();
	return 0;

#if 0
	if (argc > 2) {
		EventLoopThread loopThread;
		uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
		InetAddress serverAddr(argv[1], port);

		ChatClient client(loopThread.startLoop(), serverAddr);
		client.connect();

		std::string line;

		while (std::getline(std::cin, line)) {

			client.write(line);
		}
		client.disconnect();
		CurrentThread::sleepUsec(1000 * 1000); // wait for disconnect, see ace/logging/client.cc
	} else {
		printf("Usage: %s host_ip port\n", argv[0]);
	}
#endif
}

void ChatClient::write(const StringPiece& message) {
	MutexLockGuard lock(mutex_);
//	LOG_INFO << "message: " << message.size();
	if (connection_) {
		codec_.send(get_pointer(connection_), message);
	} else {
		if (connection_)
			codec_.send(get_pointer(connection_), message);
		else
			LOG_ERROR<<"send msg error.-->"<<message.data();
		}
	}

void print_in_middle(WINDOW *win, int starty, int startx, int width,
		char *string) {
	int length, x, y;
	float temp;

	if (win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if (startx != 0)
		x = startx;
	if (starty != 0)
		y = starty;
	if (width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length) / 2;
	x = startx + (int) temp;
	mvwprintw(win, y, x, "%s", string);
	refresh();
}

void screenPresent() {
	setlocale(LC_ALL, "");
	initscr(); /* Start curses mode 		*/
	if (has_colors() == FALSE) {
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	start_color(); /* Start color 			*/
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	attron(COLOR_PAIR(1));

	box(stdscr, '|', '-');
	print_in_middle(stdscr, LINES / 2, 0, 0, "完成");
	mvaddstr(LINES / 3, COLS / 2, "移 动 无 效 !!");
	getch();
	endwin();
}

