#pragma once
#include <vector>
#include <string>
#include <algorithm>
//底层缓冲区类型
class Buffer
{
public:
static const size_t kCheapPrepend = 8;
static const size_t kInitialSize = 1024;

explicit Buffer(size_t initialSize = kInitialSize)
: buffer_(kCheapPrepend + initialSize)
,readerIndex_(kCheapPrepend)
,writerIndex_(kCheapPrepend)
{}

size_t readableBytes()const
{
    return writerIndex_- readerIndex_;
}
size_t writableBytes()const{
    return buffer_.size() - writerIndex_;
}
size_t prependableBytes()const
{
    return readerIndex_;
}

//返回缓冲区的可读数据起始地址
const char* peek()const
{
    return begin()+readerIndex_;
}
//Buffer->string
void retrieve(size_t len)
{
if(len <readableBytes())
{
    readerIndex_ += len;//只读取的可读缓冲区的一部分（len）
}
else
{
    retrieveAll();
}
}
void retrieveAll()
{
readerIndex_ = writerIndex_ = kCheapPrepend;
}
//数据转成string类型
std::string retrieveAllAsString()
{
    //可读取的数据长度
return retrieveAsString(readableBytes());
}
std::string retrieveAsString(size_t len)
{
    std::string result(peek(),len);
    retrieve(len);//对缓冲区进行复位操作
    return result;
}

void ensureWriteableBytes(size_t len)
{
    if(writableBytes() <len)
    {
makeSpace(len); //扩容
    }
}

//把[data， data+len]数据添加到writeable中
void append(const char* data,size_t len)
{
    ensureWriteableBytes(len);
    std::copy(data,data+len,beginWrite());
    writerIndex_+=len;
}
char* beginWrite()
{
return begin() + writerIndex_;
}
const char* beginWrite() const
{
    return begin() + writerIndex_;
}

//从fd上读取数据
ssize_t readFd(int fd,int* saveErrno);
//通过fd发送数据
ssize_t writeFd(int fd,int* saveErrno);
private:

char* begin()
{
    //调用了*的重载函数+&运算法的重载函数
    return &*buffer_.begin();
}

const char* begin()const
{
    return &*buffer_.begin();
}

void makeSpace(size_t len)
{
   if(writableBytes() + prependableBytes() <len +kCheapPrepend)
   {
    buffer_.resize(writerIndex_ + len);
   }
   else
   {
    size_t readable = readableBytes();
    std::copy(begin()+readerIndex_,
    begin()+writerIndex_,
    begin()+kCheapPrepend);
    readerIndex_ = kCheapPrepend;
    writerIndex_ = readerIndex_ + readable;
   }
}


std::vector<char> buffer_;
size_t readerIndex_;
size_t writerIndex_;
};