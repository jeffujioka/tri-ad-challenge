#pragma once

#include <gtest/gtest.h>

#include <map>
#include <memory>
#include <string>

#include "synchronization/msg_queue.h"

namespace triad {

namespace synchronization {
template<typename T>
class ObjectFrequency;
}

class WordAppenderWorker {
  using WordFreq = synchronization::ObjectFrequency<std::string>;
  using WordFreqPtr = std::shared_ptr<WordFreq>;

  synchronization::MsgQueue<std::string>& sync_queue_;
  WordFreqPtr words_freq_;
  std::string stop_;

public:
  WordAppenderWorker(synchronization::MsgQueue<std::string>& queue,
                     WordFreqPtr words_freq,
                     const std::string& stop = "end");

  WordAppenderWorker() = delete;
  WordAppenderWorker(const WordAppenderWorker&) = delete;
  WordAppenderWorker& operator=(const WordAppenderWorker&) = delete;

  void operator()();

  FRIEND_TEST(WordAppenderWorker_Test, BaseCase);
};

} // namespace triad namespace synchronization
