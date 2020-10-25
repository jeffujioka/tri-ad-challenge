#pragma once

#include <gtest/gtest.h>

#include <map>
#include <memory>
#include <string>

namespace triad {

template<typename T>
class ObjectFrequency;

namespace synchronization {
template<typename T>
class MsgQueue;
}

class WordAppenderWorker {
  using MsgQueueStr = synchronization::MsgQueue<std::string>;
  using MsgQueueStrPtr = std::shared_ptr<MsgQueueStr>;
  using WordFreq = ObjectFrequency<std::string>;
  using WordFreqPtr = std::shared_ptr<WordFreq>;

  MsgQueueStrPtr sync_queue_;
  WordFreqPtr words_freq_;
  std::string stop_;

public:
  WordAppenderWorker(MsgQueueStrPtr queue,
                     WordFreqPtr words_freq,
                     const std::string& stop = "end");

  WordAppenderWorker() = delete;
  WordAppenderWorker(const WordAppenderWorker&) = delete;
  WordAppenderWorker& operator=(const WordAppenderWorker&) = delete;
  WordAppenderWorker(WordAppenderWorker&&) = default;
  WordAppenderWorker& operator=(WordAppenderWorker&&) = default;

  void operator()();

  FRIEND_TEST(WordAppenderWorker_Test, BaseCase);
};

} // namespace triad namespace synchronization
