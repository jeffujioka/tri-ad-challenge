#include <gtest/gtest.h>

#include <string>

#include "synchronization/msg_queue.h"

namespace triad {
namespace synchronization {

using std::string;

TEST(MessageQueue_Test, BaseCase) {
  MsgQueue<string> queue;

  string msg{"test"};

  queue.Send(std::move(msg));
  auto rcvd = queue.Receive();

  EXPECT_EQ("test", rcvd);
}

TEST(MessageQueue_Test, Send) {
  MsgQueue<string> queue;

  string msg{"test"};

  EXPECT_EQ(0, queue.message_queue_.size());
  queue.Send(std::move(msg));
  EXPECT_EQ(1, queue.message_queue_.size());
}

}
}
