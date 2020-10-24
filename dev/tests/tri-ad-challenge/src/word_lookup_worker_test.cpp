#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <thread>

#include "tri-ad-challenge/word_lookup_worker"

#include "synchronization/object_frequency.h"

namespace triad {

using synchronization::ObjectFrequency;

class WordLookupWorker_Test : public ::testing::Test {
 public:
  void SetUp() {
    // TODO mock ObjectFrequency class
    objfreq_ = std::make_shared<ObjectFrequency<std::string>>();

    objfreq_->InsertOrIncrement("jefferson");
    objfreq_->InsertOrIncrement("jefferson");
    objfreq_->InsertOrIncrement("jefferson");
    objfreq_->InsertOrIncrement("masahiro");
    objfreq_->InsertOrIncrement("fujioka");
    objfreq_->InsertOrIncrement("steve");
    objfreq_->InsertOrIncrement("jobs");
    objfreq_->InsertOrIncrement("toyota");
    objfreq_->InsertOrIncrement("toyota");
    objfreq_->InsertOrIncrement("toyota");
    objfreq_->InsertOrIncrement("toyota");
    objfreq_->InsertOrIncrement("toyota");
  }

  void TearDown() {}

  std::shared_ptr<ObjectFrequency<std::string>> objfreq_;
};

void WriteToFile(const std::string& filename, const std::string& text) {
  std::ofstream out(filename);

  EXPECT_TRUE(out);
  out << text;
  out.close();
}

auto GetOutputLineFromFile(const std::string& filename) {
  std::ifstream in(filename, std::ifstream::in);
  EXPECT_TRUE(in.is_open());

  std::string line;
  std::getline(in, line);

  in.close();

  return line;
}

TEST_F(WordLookupWorker_Test, TrivialCase) {
  WriteToFile("in.txt", "jefferson\n");

  // worker will read the input from this file instead of reading from std::cin
  std::ifstream in("in.txt", std::ifstream::in);

  // worker's output will be written to 'out.tx'
  // trunc is required to ensure reading "new/right" output data
  std::ofstream out("out.txt", std::ofstream::trunc);

  EXPECT_TRUE(out.is_open());

  std::promise<void> prom;

  // worker will read from 'in' and write the result to 'out'
  EXPECT_NO_THROW(
    std::thread t(WordLookupWorker(objfreq_, in, out), prom.get_future());

    prom.set_value(); // notify worker
    t.join();
  );

  in.close();
  out.close();

  // TODO replace EXPECT_EQ by EXPECT_CALL after mocking ObjectFrequency class
  EXPECT_EQ(3, objfreq_->Frequency("jefferson"));

  auto result = GetOutputLineFromFile("out.txt");
  EXPECT_EQ("Enter a word for lookup: "
            "SUCCESS: 'jefferson' was present 3 times in the initial word list",
            result);

}

TEST_F(WordLookupWorker_Test, NotFoundCase) {
  WriteToFile("in.txt", "NOTFOUND\n");

  // worker will read the input from this file instead of reading from std::cin
  std::ifstream in("in.txt", std::ifstream::in);

  // worker's output will be written to 'out.tx'
  // trunc is required to ensure reading "new/right" output data
  std::ofstream out("out.txt", std::ofstream::trunc);

  EXPECT_TRUE(out.is_open());

  std::promise<void> prom;

  // worker will read from 'in' and write the result to 'out'
  EXPECT_NO_THROW(
    using std::thread; // just to the below line fit in 80 columns large
    thread t (WordLookupWorker(objfreq_, in, out), prom.get_future());

    prom.set_value(); // notify WordLookupWorker
    t.join();
  );

  in.close();
  out.close();

  // TODO replace EXPECT_EQ by EXPECT_CALL after mocking ObjectFrequency class
  EXPECT_EQ(3, objfreq_->Frequency("jefferson"));
  // read worker's output written to 'out.txt'
  auto result = GetOutputLineFromFile("out.txt");

  EXPECT_EQ("Enter a word for lookup: "
            "'NOTFOUND' was NOT found in the initial word list",
            result);
}

TEST_F(WordLookupWorker_Test, NullWordFreque) {
  std::promise<void> prom;

  WordLookupWorker worker(nullptr, std::cin, std::cout);
  EXPECT_THROW(worker(prom.get_future()),
               std::runtime_error);
}

}
