#include "tri-ad-challenge/word_appender_worker.h"

#include "synchronization/msg_queue.h"
#include "synchronization/object_frequency.h"

namespace triad {

using synchronization::MsgQueue;

WordAppenderWorker::WordAppenderWorker(MsgQueueStrPtr queue, 
  WordFreqPtr words_freq, const std::string& stop) : sync_queue_(queue),
  words_freq_(words_freq), stop_(stop) {
}

void WordAppenderWorker::operator()() {
  bool shall_stop = false;
  
  while (!shall_stop) // keep processing until not find "end"
  {
    // blocking function, it will wait until a new message is available
    auto word = sync_queue_->Receive();
    // from this point producer is free to produce more data
    
    // the following code can be used to extract non-alphabetic characteres
    // word.erase(remove_if(word.begin(), word.end(), [](unsigned char c) { return std::isalpha(c); }), word.end());
    shall_stop = word == stop_; // shall we stop processing? was "end" found?
    
    if (!shall_stop)
    {
      // stop condition not found... so let's insert/increment a "new" word
      words_freq_->InsertOrIncrement(word); // it's thread safe
    }
  }
}

} // namespace triad
