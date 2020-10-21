#include "tri-ad-challenge/word_appender_worker.h"

namespace triad {

using synchronization::MsgQueue;

WordAppenderWorker::WordAppenderWorker(
  synchronization::MsgQueue<std::string>& queue,
  std::map<std::string, size_t>& words_map, const std::string& stop) 
  : sync_queue_(queue),  words_map_(words_map), stop_(stop) {
}

void WordAppenderWorker::operator()() {
  bool end_found = false;
  
  while (!end_found) // keep processing until not find "end"
  {
    // blocking function, it will wait until a new message is available
    auto word = sync_queue_.Receive();
    // from this point producer is free to produce more data

    end_found = word == stop_; // shall we stop processing? was "end" found?
    
    if (!end_found)
    {
      auto it = words_map_.find(word);

      if (it != words_map_.end()) {
        // 'word' is already present in the map
        // let's increment the word counter 
        ++(it->second);
      } else {
        // 'word' is not present in the map
        // let's add 'word' with counter equal to zero
        // insert 'word' ordered (case sensitive)
        words_map_.emplace(word, 1);
      }
    }
  }
}

} // namespace triad
