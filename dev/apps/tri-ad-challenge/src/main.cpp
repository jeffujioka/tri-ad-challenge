#include <future>
#include <iostream>
#include <thread>

#include "synchronization/msg_queue.h"

#include "tri-ad-challenge/object_frequency"
#include "tri-ad-challenge/word_appender_worker.h"
#include "tri-ad-challenge/word_lookuper"
#include "tri-ad-challenge/word_input_reader"

using triad::ObjectFrequency;
using triad::WordAppenderWorker;
using triad::WordInputReader;
using triad::WordLookuper;
using triad::synchronization::MsgQueue;

int main ()
{
  try
  {
    auto queue = std::make_shared<MsgQueue<std::string>>();
    auto word_freq = std::make_shared<ObjectFrequency<std::string>>();

    std::promise<void> ready_promise;
    std::string stop_condition{"end"};

    // 'tappender' consumes words received from the 'tinput' through 'queue' 
    // (MsgQueue<std::string> - it's a thread-safe queue) and insert them 
    // in the 'word_freq', duplicated values increase by one the frequency 
    // counter in the 'word_freq'.
    // Terminate when the 'stop_condition' (string "end") is encountered.
    std::thread tappender(WordAppenderWorker(queue, word_freq, stop_condition));

    WordInputReader input_reader(queue, stop_condition);
    // Read input words from STDIN and pass them to the 'lookup' through
    // 'queue' for inclusion in the 'word_freq'.
    // Terminate when the 'stop_condition' (string "end") is entered.
    input_reader.AskWords();

    // it'll terminate when 'input_reader' send the 'stop_condition' ("end")
    tappender.join();

    WordLookuper lookup(word_freq);
    // 'WordLookuper' repeatedly ask the user for a word and check whether 
    // it is present in the 'word_freq'.
    // Terminate on EOF.
    lookup.LookupRepeatedly();

    //std::cout << *word_freq << std::endl;
    std::cout << std::endl << "Take care!!!" << std::endl;
  }
  catch (std::exception & e)
  {
    std::cout << "error: " << e.what() << "\n";
  } catch (...) {
    std::cout << "Unexpected error has happened!\n";
  }
  
  return 0;
}
