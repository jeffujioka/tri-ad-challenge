#include <gtest/gtest.h>

#include "tri-ad-challenge/word_appender_worker.h"

#include "synchronization/msg_queue.h"
#include "synchronization/object_frequency.h"

namespace triad {

using namespace std;

using synchronization::MsgQueue;
using synchronization::ObjectFrequency;

TEST(WordAppenderWorker_Test, BaseCase) {
  auto queue = std::make_shared<MsgQueue<std::string>>();
  auto word_freq = std::make_shared<ObjectFrequency<string>>();
  std::string stop_cond{"end"};

  WordAppenderWorker worker(queue, word_freq, stop_cond);

  // enqueuing "test1" "test2" "test3" "end"
  queue->Send("test1");
  queue->Send("test2");
  queue->Send("test3");
  queue->Send("end");
    
  worker(); // "executing" worker

  const auto& word_map = worker.words_freq_->get();
  
  EXPECT_EQ(3, word_map.size());
  EXPECT_NO_THROW(word_map.at("test1"));
  EXPECT_NO_THROW(word_map.at("test2"));
  EXPECT_NO_THROW(word_map.at("test3"));

  // "end" shall not be found
  EXPECT_THROW(word_map.at(stop_cond), std::out_of_range);
}

} // namespace triad
