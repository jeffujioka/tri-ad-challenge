#include <gtest/gtest.h>

#include <iostream>
#include <fstream>
#include <thread>

#include "tri-ad-challenge/word_input_reader"

#include "synchronization/msg_queue.h"

namespace triad {

using synchronization::MsgQueue;

TEST(WordInputReader_Test, TrivialCase) {
  // TODO mock MsgQueue class
  auto queue = std::make_shared<MsgQueue<std::string>>();

  WordInputReader input(queue, "end", "test msg$");

  input.AskWords([](const std::string& msg) {
        EXPECT_EQ("test msg$", msg);
        return "jefferson masahiro fujioka end"; 
      });

  EXPECT_EQ("jefferson", queue->Receive());
  EXPECT_EQ("masahiro", queue->Receive());
  EXPECT_EQ("fujioka", queue->Receive());
  EXPECT_EQ("end", queue->Receive());
}

TEST(WordInputReader_Test, CheckMultipleLinesAndOrder) {
  // TODO mock MsgQueue class
  auto queue = std::make_shared<MsgQueue<std::string>>();
  WordInputReader input(queue, "end", "test msg$");

  input.AskWords([](const std::string& msg) {
        EXPECT_EQ("test msg$", msg);
        static auto counter = 0u;
        switch (counter++) {
        case 0:
          return "jefferson masahiro fujioka"; 
        case 1:
          return "maria emilia gaicoski fujioka"; 
        case 2:
          return "lyoto machida";
        case 3:
          return "steve jobs"; 
        default:
          return "end";
        }
      });

  // TODO replace EXPECT_EQ by EXPECT_CALL after mocking MsgQueue class
  EXPECT_EQ("jefferson", queue->Receive());
  EXPECT_EQ("masahiro", queue->Receive());
  EXPECT_EQ("fujioka", queue->Receive());
  EXPECT_EQ("maria", queue->Receive());
  EXPECT_EQ("emilia", queue->Receive());
  EXPECT_EQ("gaicoski", queue->Receive());
  EXPECT_EQ("fujioka", queue->Receive());
  EXPECT_EQ("lyoto", queue->Receive());
  EXPECT_EQ("machida", queue->Receive());
  EXPECT_EQ("steve", queue->Receive());
  EXPECT_EQ("jobs", queue->Receive());
  EXPECT_EQ("end", queue->Receive());
}

TEST(WordInputReader_Test, CheckValidityNullMsgQueue) {
  // TODO mock MsgQueue class
  WordInputReader input(nullptr, "end");

  std::promise<void> prom;

  EXPECT_THROW(input.AskWords(), std::exception);
}

TEST(WordInputReader_Test, CheckValidityEmptyStop) {
  // TODO mock MsgQueue class
  WordInputReader input(std::make_shared<MsgQueue<std::string>>(), 
                         "");

  EXPECT_THROW(input.AskWords(), std::exception);
}

}
