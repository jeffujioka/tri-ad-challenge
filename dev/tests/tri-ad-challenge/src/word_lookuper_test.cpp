#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <thread>

#include "tri-ad-challenge/object_frequency"
#include "tri-ad-challenge/word_lookuper"

namespace triad {

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

TEST_F(WordLookupWorker_Test, TrivialCase) {
  WordLookuper lookup(objfreq_, "found test");

 lookup.LookupRepeatedly(
      [](const std::string& msg) { 
        EXPECT_EQ("found test", msg);
        static auto counter = 0u;
        if (counter++ == 0) {
          return "jefferson"; 
        }
        return ""; // returns "EOF" to exit from LookupRepeatedly
      },
      [this](const std::string& w, size_t f) {
        EXPECT_EQ("jefferson", w);
        EXPECT_EQ(3, f);
      });

  // TODO replace EXPECT_EQ by EXPECT_CALL after mocking ObjectFrequency class
  EXPECT_EQ(3, objfreq_->Frequency("jefferson"));
}

TEST_F(WordLookupWorker_Test, NotFoundCase) {
  WordLookuper lookup(objfreq_, "not found test");

  lookup.LookupRepeatedly(
      [](const std::string& msg) { 
        EXPECT_EQ("not found test", msg);
        static auto counter = 0u;
        if (counter++ == 0) {
          return "notfound"; 
        }
        return ""; // returns "EOF" from LookupRepeatedly
      },
      [](const std::string& w, size_t f) {
        EXPECT_EQ("notfound", w);
        EXPECT_EQ(0, f);
      });

  // TODO replace EXPECT_EQ by EXPECT_CALL after mocking ObjectFrequency class
  EXPECT_EQ(3, objfreq_->Frequency("jefferson"));
}

TEST_F(WordLookupWorker_Test, NullWordFreque) {
  WordLookuper lookup(nullptr);
  EXPECT_THROW(lookup.LookupRepeatedly(), std::runtime_error);
}

}
