#include "tri-ad-challenge/word_appender_worker.h"

#include "synchronization/object_frequency.h"

namespace triad {

using synchronization::MsgQueue;

WordAppenderWorker::WordAppenderWorker(MsgQueue<std::string>& queue, 
  WordFreqPtr words_freq, const std::string& stop) : sync_queue_(queue),
  words_freq_(words_freq), stop_(stop) {
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
      // stop condition not found... so let's insert/increment a "new" word
      words_freq_->InsertOrIncrement(word); // it's thread safe
    }
  }
}

} // namespace triad
