#include <muduo/base/Singleton.h>
#include "codec.h"
void LengthHeaderCodec::onMessage(const muduo::net::TcpConnectionPtr& conn,
		muduo::net::Buffer* buf, muduo::Timestamp receiveTime) {
//	muduo::PrintBuff::printBufferByHex("recv:(onmessage)",
//					buf->toStringPiece().data(), buf->toStringPiece().size());

//		LOG_INFO << "recv: " << buf->toStringPiece().data();

	DP_U32 ipDst = inet_addr(conn->peerAddress().toIp().c_str());

	DP_U32 successOper = 0;
	DP_U32 tftpTimeOut = 1;
	DP_U32 exeFail = 2;
	DP_U32 ack = 3;

	UpdateStatus_S *upInfo = (UpdateStatus_S *) buf->toStringPiece().data();
	if (upInfo->header.u16Headtag == 0xF0F1) {
		if (upInfo->header.stFunctionMsg.u8CommandID == Command_UpdateBatch) {
			if (upInfo->header.u16PackageLen == sizeof(UpdateStatus_S)) {
				if (upInfo->_success == successOper) {
//					LOG_INFO << "8888888888888";

					Screen::persentPrint(ipDst, CompleteName);
					Screen::updateStatusPrint(ipDst, SuccessName);
					MapUpdateStatus status =
							muduo::Singleton<UpdateStatus>::instance().getUpdateStatus();
					status[ipDst] = successOper;

					muduo::Singleton<UpdateStatus>::instance().updateUpdateStatus(
							status);
				} else if (upInfo->_success == tftpTimeOut) {
//					LOG_INFO << "99999999999999";
					Screen::updateStatusPrint(ipDst, ConFailName);
				} else if (upInfo->_success == exeFail) {
//					LOG_INFO << "////////////////////////";
					Screen::updateStatusPrint(ipDst, FailName);
				} else if (upInfo->_success == ack) {

					MapUpdateStatus status =
							muduo::Singleton<UpdateStatus>::instance().getUpdateStatus();
					status.insert(MapUpdateStatus::value_type(ipDst, ack));

					muduo::Singleton<UpdateStatus>::instance().updateUpdateStatus(
							status);
//					LOG_INFO << "666666666666666666666< " << ipDst;
					Screen::updateStatusPrint(ipDst, UpdatingName);
				} else {
//					LOG_INFO << "5555555555555555";
					//						Screen::updateStatusPrint(ipDst, UpdatingName);
				}
				//					LOG_INFO<<"1111111111111111111111";
				//					conn->peerAddress().toIp().c_str();

			} else {
//				LOG_INFO << "1111111111111111111111";
			}
		} else {
//			LOG_INFO << "222222222";
		}
	} else {

	}
	buf->retrieveAll();
	//		while (buf->readableBytes() >= kHeaderLen) // kHeaderLen == 4
	//		{
	//			// FIXME: use Buffer::peekInt32()
	//			const void* data = buf->peek();
	//			int32_t be32 = *static_cast<const int32_t*>(data); // SIGBUS
	//			const int32_t len = muduo::net::sockets::networkToHost32(be32);
	//			if (len > 65536 || len < 0) {
	//				LOG_ERROR<< "Invalid length " << len;
	//				conn->shutdown();  // FIXME: disable reading
	//				break;
	//			}
	//			else if (buf->readableBytes() >= len + kHeaderLen)
	//			{
	//				buf->retrieve(kHeaderLen);
	//				muduo::string message(buf->peek(), len);
	//				messageCallback_(conn, message, receiveTime);
	//				buf->retrieve(len);
	//			}
	//			else
	//			{
	//				break;
	//			}
	//		}

}
#if 0

using namespace muduo;
using namespace muduo::net;
using namespace pubsub;

ParseResult pubsub::parseMessage(Buffer* buf,
		string* cmd,
		string* topic,
		string* content)
{
	ParseResult result = kError;
	const char* crlf = buf->findCRLF();
	if (crlf)
	{
		const char* space = std::find(buf->peek(), crlf, ' ');
		if (space != crlf)
		{
			cmd->assign(buf->peek(), space);
			topic->assign(space+1, crlf);
			if (*cmd == "pub")
			{
				const char* start = crlf + 2;
				crlf = buf->findCRLF(start);
				if (crlf)
				{
					content->assign(start, crlf);
					buf->retrieveUntil(crlf+2);
					result = kSuccess;
				}
				else
				{
					result = kContinue;
				}
			}
			else
			{
				buf->retrieveUntil(crlf+2);
				result = kSuccess;
			}
		}
		else
		{
			result = kError;
		}
	}
	else
	{
		result = kContinue;
	}
	return result;
}

#endif
