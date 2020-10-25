#include <future>
#include <iostream>
#include <thread>

#include "synchronization/msg_queue.h"
#include "synchronization/object_frequency.h"

#include "tri-ad-challenge/word_appender_worker.h"
#include "tri-ad-challenge/word_lookup_worker"
#include "tri-ad-challenge/word_input_worker"

using triad::WordAppenderWorker;
using triad::WordInputWorker;
using triad::WordLookupWorker;
using triad::synchronization::MsgQueue;
using triad::synchronization::ObjectFrequency;

int main ()
{
  try
  {
    auto queue = std::make_shared<MsgQueue<std::string>>();
    auto word_freq = std::make_shared<ObjectFrequency<std::string>>();

    std::promise<void> ready_promise;
    auto ready_future = ready_promise.get_future();
    std::string stop_condition{"end"};

    // 'tlookup' repeatedly ask the user for a word and check whether 
    // it was present in the word list. 
    // Terminate on EOF.
    // It will be waiting for 'ready_future' std::future notification
    // which will be emitted by 'input_thread' as soon as the user enter
    // the 'stop_condition' (string "end")
    // Actually, there is no need to have this thread once it will start 
    // processing only after the completion of tasks 'tappender' and 'tinput'. 
    // I made this way only to "exercise" this threaded-env scenario
    std::thread tlookup(WordLookupWorker(word_freq, std::cin, std::cout),
                        std::move(ready_future));
    // 'tappender' consumes words received from the 'tinput' through 'queue' 
    // (MsgQueue<std::string> - it's a thread-safe queue) and insert them 
    // in the 'word_freq' (ObjectFrequency<std::string> - it's a 
    // thread-safe map), duplicates are discarded.
    // Terminate when the 'stop_condition' (string "end") is encountered.
    std::thread tappender(WordAppenderWorker(queue, word_freq, stop_condition));
    // Read input words from STDIN and pass them to the 'tlookup' through
    // 'queue' for inclusion in the 'word_freq'.
    // Terminate when the 'stop_condition' (string "end") is entered.
    std::thread tinput(WordInputWorker(queue, stop_condition, std::cin),
                       std::move(ready_promise));

    tlookup.join();
    tappender.join();
    tinput.join();

    // std::cout << *word_freq << std::endl;
  }
  catch (std::exception & e)
  {
    std::cout << "error: " << e.what() << "\n";
  } catch (...) {
    std::cout << "Unexpected error has happened!\n";
  }
  
  return 0;
}
