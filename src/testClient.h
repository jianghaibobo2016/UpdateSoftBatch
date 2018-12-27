/*
 * testClient.h
 *
 *  Created on: Oct 19, 2018
 *      Author: jhb
 */

#ifndef SRC_TESTCLIENT_H_
#define SRC_TESTCLIENT_H_

#include <muduo/base/Logging.h>
#include <boost/shared_ptr.hpp>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/TcpClient.h>
#include <vector>
#include "interactivepro.h"
#include "dp_sd_type.h"
#include "codec.h"
#include "Screen.h"
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
using namespace muduo;
using namespace muduo::net;



class ChatClient: boost::noncopyable {
public:
	ChatClient(EventLoop* loop, const InetAddress& serverAddr) :
			client_(loop, serverAddr, "ChatClient"), codec_(
					boost::bind(&ChatClient::onStringMessage, this, _1, _2, _3)) {
		client_.setConnectionCallback(
				boost::bind(&ChatClient::onConnection, this, _1));
		client_.setMessageCallback(
				boost::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2,
						_3));
//		client_.enableRetry();
	}

	void connect() {
		client_.connect();
	}

	void disconnect() {
		client_.disconnect();
	}

	void write(const StringPiece& message);
//	void write(const void *data, DP_U32 len);

private:
	void onConnection(const TcpConnectionPtr& conn) {
		LOG_INFO << conn->localAddress().toIpPort() << " -> "
				<< conn->peerAddress().toIpPort() << " is "
				<< (conn->connected() ? "UP" : "DOWN");

		MutexLockGuard lock(mutex_);
		if (conn->connected()) {
//			LOG_ERROR<<"conn->connected())";
			connection_ = conn;
		} else {
			connection_.reset();
		}
	}

	void onStringMessage(const TcpConnectionPtr&, const string& message,
			Timestamp) {
		printf("<<< %s\n", message.c_str());
	}

	TcpClient client_;
	LengthHeaderCodec codec_;
	MutexLock mutex_;
	TcpConnectionPtr connection_ GUARDED_BY(mutex_);
};

#endif /* SRC_TESTCLIENT_H_ */
