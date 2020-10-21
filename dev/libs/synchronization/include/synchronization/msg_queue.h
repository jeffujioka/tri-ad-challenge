#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>

#include <gtest/gtest_prod.h>

namespace triad
{
namespace synchronization
{

template<typename T>
class MsgQueue {
  std::mutex mtx_;
  std::condition_variable cond_var_;
  std::queue<T> message_queue_;

public:
  MsgQueue() = default;

  MsgQueue(const MsgQueue&) = delete;
  MsgQueue& operator=(const MsgQueue&) = delete;

  MsgQueue(const MsgQueue&& msg) = delete;
  MsgQueue&& operator=(const MsgQueue&& msg) = delete;

  void Send(T&& msg) {
    {
      std::lock_guard<std::mutex> lck(mtx_);
      message_queue_.push(std::move(msg));
    }
    cond_var_.notify_one();
  }

  T Receive() {
    std::unique_lock<std::mutex> lck(mtx_);
    // if message queue is empty, let's wait until someone post a new message
    cond_var_.wait(lck, [this]() { return !message_queue_.empty(); });
    // get (moving) the first message in the list
    auto msg = std::move(message_queue_.front());
    message_queue_.pop();
    return msg;
  }

  FRIEND_TEST(MessageQueue_Test, Send);
};

} // namespace synchronization
} // namespace triad
