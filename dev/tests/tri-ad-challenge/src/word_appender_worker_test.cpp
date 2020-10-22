#include <gtest/gtest.h>

#include "tri-ad-challenge/word_appender_worker.h"

#include "synchronization/object_frequency.h"

namespace triad {

using namespace std;

using synchronization::MsgQueue;
using synchronization::ObjectFrequency;

TEST(WordAppenderWorker_Test, BaseCase) {
  MsgQueue<std::string> queue;
  auto word_freq = std::make_shared<ObjectFrequency<string>>();

  WordAppenderWorker worker(queue, word_freq, "end");

  // enqueuing "test1" "test2" "test3" "end"
  queue.Send("test1");
  queue.Send("test2");
  queue.Send("test3");
  queue.Send("end");
    
  worker(); // "executing" worker

  const auto& word_map = worker.words_freq_->get();
  
  EXPECT_EQ(3, word_map.size());
  EXPECT_NO_THROW(word_map.at("test1"));
  EXPECT_NO_THROW(word_map.at("test2"));
  EXPECT_NO_THROW(word_map.at("test3"));

  // "end" shall not be found
  EXPECT_THROW(word_map.at("end"), std::out_of_range);
  #if 0
  EXPECT_EQ(3, worker.words_map_.size());
  EXPECT_NO_THROW(worker.words_freq.at("test1"));
  EXPECT_NO_THROW(worker.words_freq.at("test2"));
  EXPECT_NO_THROW(worker.words_freq->.at("test3"));

  // "end" shall not be found
  EXPECT_THROW(worker.words_map_.at("end"), std::out_of_range);
  #endif
}

} // namespace triad
