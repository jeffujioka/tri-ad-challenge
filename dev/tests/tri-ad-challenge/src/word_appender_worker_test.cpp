#include <gtest/gtest.h>

#include "tri-ad-challenge/word_appender_worker.h"

namespace triad {

using synchronization::MsgQueue;

TEST(WordAppenderWorker_Test, BaseCase) {
  MsgQueue<std::string> queue;
  std::map<std::string, size_t> map;

  WordAppenderWorker worker(queue, map, "end");

  // enqueuing "test1" "test2" "test3" "end"
  queue.Send("test1");
  queue.Send("test2");
  queue.Send("test3");
  queue.Send("end");
    
  worker(); // "executing" worker

  EXPECT_EQ(3, worker.words_map_.size());
  EXPECT_NO_THROW(worker.words_map_.at("test1"));
  EXPECT_NO_THROW(worker.words_map_.at("test2"));
  EXPECT_NO_THROW(worker.words_map_.at("test3"));

  // "end" shall not be found
  EXPECT_THROW(worker.words_map_.at("end"), std::out_of_range);
}

} // namespace triad
