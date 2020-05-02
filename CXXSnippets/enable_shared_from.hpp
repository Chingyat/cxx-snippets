#pragma once
#include <memory>
#include <type_traits>

namespace cs {
class enable_shared_from
    : private std::enable_shared_from_this<enable_shared_from> {
public:
  virtual ~enable_shared_from() = default;

  template <typename T,
            std::enable_if_t<std::is_base_of<enable_shared_from, T>{}, int> = 0>
  static auto shared_from(T *this_) -> std::shared_ptr<T> {
    return std::dynamic_pointer_cast<T>(this_->shared_from_this());
  }

  template <typename T,
            std::enable_if_t<std::is_base_of<enable_shared_from, T>{}, int> = 0>
  static auto weak_from(T *this_) -> std::weak_ptr<T> {
    return shared_from(this_);
  }
};
} // namespace cs