#include <gtest/gtest.h>

#include <iostream>
#include <fstream>
#include <thread>

#include "tri-ad-challenge/word_input_worker"

#include "synchronization/msg_queue.h"

namespace triad {

using synchronization::MsgQueue;

TEST(WordInputWorker_Test, TrivialCase) {
  std::ofstream out("in.txt");

  EXPECT_TRUE(out);
  out << "jefferson masahiro fujioka end\n";
  out.close();

  std::ifstream in("in.txt", std::ifstream::in);

  // TODO mock MsgQueue class
  auto queue = std::make_shared<MsgQueue<std::string>>();

  std::promise<void> prom;
  auto future = prom.get_future();

  std::thread worker(WordInputWorker(queue, "end", in), std::move(prom));

  future.get();
  worker.join();
  EXPECT_EQ("jefferson", queue->Receive());
  EXPECT_EQ("masahiro", queue->Receive());
  EXPECT_EQ("fujioka", queue->Receive());
  EXPECT_EQ("end", queue->Receive());
}

TEST(WordInputWorker_Test, CheckMultipleLinesAndOrder) {
  std::ofstream out("in.txt");

  EXPECT_TRUE(out);
  out << "jefferson masahiro fujioka\n";
  out << "maria emilia gaicoski fujioka\n";
  out << "lyoto machida\n";
  out << "steve jobs\n";
  out << "end\n";
  out.close();

  std::ifstream in("in.txt", std::ifstream::in);

  // TODO mock MsgQueue class
  auto queue = std::make_shared<MsgQueue<std::string>>();
  WordInputWorker worker(queue, "end", in);

  std::promise<void> prom;
  auto future = prom.get_future();

  worker(std::move(prom));

  future.wait();

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

TEST(WordInputWorker_Test, CheckValidityNullMsgQueue) {
  // TODO mock MsgQueue class
  WordInputWorker worker(nullptr, "end", std::cin);

  std::promise<void> prom;

  EXPECT_THROW(worker(std::move(prom)), std::exception);
}

TEST(WordInputWorker_Test, CheckValidityEmptyStop) {
  // TODO mock MsgQueue class
  WordInputWorker worker(std::make_shared<MsgQueue<std::string>>(), 
                         "", std::cin);

  std::promise<void> prom;

  EXPECT_THROW(worker(std::move(prom)), std::exception);
}

}
