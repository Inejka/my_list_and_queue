//
// Created by inejka on 13.12.2020.
//

#ifndef _HOME_INEJKA_CLIONPROJECTS_MY_LIST_AND_QUEUE_QUEUE_H_
#define _HOME_INEJKA_CLIONPROJECTS_MY_LIST_AND_QUEUE_QUEUE_H_

#include <memory>
#include <utility>

#include "List.h"

namespace my_queue {

template<class Type>
class Queue {
 public:
  explicit Queue(std::unique_ptr<my_list::List<Type>> list) {
    list_ = std::move(list);
  }
  void Push(const Type &to_add) {
    list_->PushBack(to_add);
    size_++;
  }
  void Pop() {
    auto tmp = list_->Begin();
    list_->Erase(tmp);
    if (size_ > 0)size_--;
  }
  Type Front() {
    return **list_->Begin();
  }
  int Size() const {
    return size_;
  }
  bool Empty() const {
    return size_ == 0;
  }

 protected:
  int size_ = 0;
  std::unique_ptr<my_list::List<Type>> list_;
};
}  // namespace my_queue
#endif  // _HOME_INEJKA_CLIONPROJECTS_MY_LIST_AND_QUEUE_QUEUE_H_
