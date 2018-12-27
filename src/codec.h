#ifndef MUDUO_EXAMPLES_ASIO_CHAT_CODEC_H
#define MUDUO_EXAMPLES_ASIO_CHAT_CODEC_H

#include <muduo/base/Logging.h>
#include <muduo/net/Buffer.h>
#include <muduo/net/Endian.h>
#include <muduo/net/TcpConnection.h>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <arpa/inet.h>
#include "Screen.h"
#include "PrintBuff.h"
extern std::vector<DP_U32> g_DstIPAddr;
class LengthHeaderCodec: boost::noncopyable {
public:
	typedef boost::function<
			void(const muduo::net::TcpConnectionPtr&,
					const muduo::string& message, muduo::Timestamp)> StringMessageCallback;

	explicit LengthHeaderCodec(const StringMessageCallback& cb) :
			messageCallback_(cb) {
	}

	void onMessage(const muduo::net::TcpConnectionPtr& conn,
			muduo::net::Buffer* buf, muduo::Timestamp receiveTime);

// FIXME: TcpConnectionPtr
	void send(muduo::net::TcpConnection* conn,
			const muduo::StringPiece& message) {
		muduo::net::Buffer buf;
		buf.append(message.data(), message.size());
		int32_t len = static_cast<int32_t>(message.size());
//		int32_t be32 = muduo::net::sockets::hostToNetwork32(len);
//		buf.prepend(&be32, sizeof be32);
		muduo::PrintBuff::printBufferByHex("send: ", buf.toStringPiece().data(),
				buf.toStringPiece().size());
		conn->send(&buf);
	}

private:
	StringMessageCallback messageCallback_;
	const static size_t kHeaderLen = sizeof(int32_t);
};

#endif  // MUDUO_EXAMPLES_ASIO_CHAT_CODEC_H
