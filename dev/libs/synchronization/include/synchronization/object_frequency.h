#pragma once

#include <cstddef>
#include <map>
#include <mutex>
#include <ostream>

#include <gtest/gtest_prod.h>

namespace triad {
namespace synchronization {

template<typename T>
class ObjectFrequency {
  mutable std::mutex mtx_;
  // it obj_freq_map_ could be templatized as well
  // perhaps is not relevant so std::unordered_map could be used 
  // to improve perfomance on insertion/finding
  std::map<T, size_t> obj_freq_map_;

public:
  ObjectFrequency() = default;

  /// non-copyable
  ObjectFrequency(const ObjectFrequency&) = delete;
  ObjectFrequency& operator=(const ObjectFrequency&) = delete;
  /// non-movable
  ObjectFrequency(const ObjectFrequency&&) = delete;
  ObjectFrequency&& operator=(const ObjectFrequency&&) = delete;

  /// \brief     Inserts \a obj with counter equal to one if \a obj is not yet
  ///            inserted, otherwise increment the frequency counter by 1.
  /// \param[in] obj The object to be inserted or incremented.
  /// \return    Returns the frequency counter for the given object \a obj.
  size_t InsertOrIncrement(const T& obj) {
    size_t frequency = 0;
    {
      std::lock_guard<std::mutex> lck(mtx_);
      auto it = obj_freq_map_.find(obj);

      if (it != obj_freq_map_.end()) {
        // 'obj' is already present in the map
        // let's increment the frequency counter 
        ++(it->second);
        frequency = it->second;
      } else {
        // 'obj' is not present in the map
        // let's add 'obj' with counter equal to one
        // insert 'obj' ordered
        frequency = 1;
        obj_freq_map_.emplace(obj, frequency);
      }
    }
    return frequency;
  }

  size_t Frequency(const T &obj) {
    size_t frequency = 0;
    {
      std::lock_guard<std::mutex> lck(mtx_);
      frequency = obj_freq_map_[obj];
    }
    return frequency;
  }

  /// \brief   Returns a const reference of \a obj_freq_map_ inder to give
  ///          a read-only access to it.
  /// \return  Returns a const reference of \a obj_freq_map_
  const auto& get() const { return obj_freq_map_; }

  const auto& operator&() const { return obj_freq_map_; }

  template<typename Obj>
  friend std::ostream& operator<<(std::ostream& oss, 
                                  const ObjectFrequency<Obj>& rhs);

private:
  void Print(std::ostream& oss) const {
    std::lock_guard<std::mutex> lck(mtx_);

    for (const auto& [key, value] : obj_freq_map_) {
      oss << key << " " << value << "\n";
    }
    oss << "=== Total objects found: " << obj_freq_map_.size();
  }

  FRIEND_TEST(ObjectFrequency_Test, CheckOrder);
};

template<typename Obj>
std::ostream& operator<<(std::ostream& oss, 
                         const ObjectFrequency<Obj>& rhs) {
  // print is thread safe
  rhs.Print(oss);
  return oss;  
}

} // namespace synchronization
} // namespace triad
