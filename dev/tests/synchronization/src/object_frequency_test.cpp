#include <gtest/gtest.h>

#include "synchronization/object_frequency.h"

#include <string>
#include <iostream>

using namespace std;

namespace triad {
namespace synchronization {

TEST(ObjectFrequency_Test, IntFrequency) {
  ObjectFrequency<int> freq;

  EXPECT_EQ(1, freq.InsertOrIncrement(1));
  EXPECT_EQ(1, freq.InsertOrIncrement(2));
  EXPECT_EQ(1, freq.InsertOrIncrement(3));
  EXPECT_EQ(2, freq.InsertOrIncrement(1));
  EXPECT_EQ(3, freq.InsertOrIncrement(1));
  EXPECT_EQ(4, freq.InsertOrIncrement(1));
}

TEST(ObjectFrequency_Test, StringFrequency) {
  ObjectFrequency<string> freq;

  EXPECT_EQ(1, freq.InsertOrIncrement("bbb"));
  EXPECT_EQ(2, freq.InsertOrIncrement("bbb"));
  EXPECT_EQ(1, freq.InsertOrIncrement("aaa"));
  EXPECT_EQ(2, freq.InsertOrIncrement("aaa"));
  EXPECT_EQ(3, freq.InsertOrIncrement("aaa"));
  EXPECT_EQ(4, freq.InsertOrIncrement("aaa"));
}

TEST(ObjectFrequency_Test, CheckOrder) {
  ObjectFrequency<string> freq;

  freq.InsertOrIncrement("bbb");
  freq.InsertOrIncrement("aaa");
  freq.InsertOrIncrement("a");
  freq.InsertOrIncrement("aa");
  freq.InsertOrIncrement("123");
  freq.InsertOrIncrement("abc");
  freq.InsertOrIncrement("ab");
  
  const auto& objmap = freq.obj_freq_map_;
  auto it = objmap.begin();

  EXPECT_NE(it, objmap.end());
  
  EXPECT_EQ("123", it->first);
  ++it;
  EXPECT_EQ("a", it->first);
  ++it;
  EXPECT_EQ("aa", it->first);
  ++it;
  EXPECT_EQ("aaa", it->first);
  ++it;
  EXPECT_EQ("ab", it->first);
  ++it;
  EXPECT_EQ("abc", it->first);
  ++it;
  EXPECT_EQ("bbb", it->first);
}

}
}