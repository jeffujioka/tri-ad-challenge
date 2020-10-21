#pragma once

#include <gtest/gtest.h>

#include <map>
#include <string>

#include "synchronization/msg_queue.h"

namespace triad {

class WordAppenderWorker {
  synchronization::MsgQueue<std::string>& sync_queue_;
  std::map<std::string, size_t>& words_map_;
  std::string stop_;

public:
  WordAppenderWorker(synchronization::MsgQueue<std::string>& queue,
                     std::map<std::string, size_t>& words_map,
                     const std::string& stop = "end");

  WordAppenderWorker() = delete;
  WordAppenderWorker(const WordAppenderWorker&) = delete;
  WordAppenderWorker& operator=(const WordAppenderWorker&) = delete;

  void operator()();

  FRIEND_TEST(WordAppenderWorker_Test, BaseCase);
};

} // namespace triad namespace synchronization
