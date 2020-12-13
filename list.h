//
// Created by inejka on 13.12.2020.
//

#ifndef _HOME_INEJKA_CLIONPROJECTS_MY_LIST_AND_QUEUE_LIST_H_
#define _HOME_INEJKA_CLIONPROJECTS_MY_LIST_AND_QUEUE_LIST_H_

#include <memory>
#include <iterator>

namespace my_list {

template<class Type>
class BidirectionalList;

template<class Type>
class ForwardList;

template<class Type>
class List {
 public:
  List() {
    first_ = nullptr;
    last_ = nullptr;
  }

  virtual void PushBack(const Type &to_add) = 0;

 protected:
  struct Node {
    explicit Node(const std::shared_ptr<Node> &prev) : prev(prev) {}
    explicit Node(const Type &key) : key(key) {}
    std::shared_ptr<Node> next = nullptr, prev = nullptr;
    Type key;
  };
  std::shared_ptr<Node> first_, last_;

 public:
  class Iterator : public std::iterator<std::bidirectional_iterator_tag, Type> {
   public:
    explicit Iterator(const std::shared_ptr<Node> &current)
        : current_(current) {}
    virtual Iterator &operator++() = 0;
    virtual Iterator &operator--() = 0;
    Type &operator*() {
      return current_->key;
    }
    bool operator==(const Iterator &rhs) {
      return current_ == rhs.current_;
    }
    bool operator!=(const Iterator &rhs) {
      return current_ != rhs.current_;
    }
   protected:
    friend class BidirectionalList<Type>;
    friend class ForwardList<Type>;
    std::shared_ptr<Node> current_;
  };

  virtual std::shared_ptr<Iterator> Begin() = 0;
  virtual std::shared_ptr<Iterator> End() = 0;
  virtual void Erase(std::shared_ptr<Iterator> &) = 0;
};

template<class Type>
class BidirectionalList : public List<Type> {
 public:
  void PushBack(const Type &to_add) override {
    if (List<Type>::first_ == List<Type>::last_ &&
        List<Type>::first_ == nullptr) {
      List<Type>::first_ = List<Type>::last_ =
          std::make_shared<typename List<Type>::Node>
              (typename List<Type>::Node(to_add));
    } else if (List<Type>::first_ == List<Type>::last_) {
      List<Type>::last_ = std::make_shared<typename List<Type>::Node>
          (typename List<Type>::Node(to_add));
      List<Type>::first_->next = List<Type>::last_;
      List<Type>::last_->prev = List<Type>::first_;
    } else {
      List<Type>::last_->next = std::make_shared<typename List<Type>::Node>
          (typename List<Type>::Node(to_add));
      List<Type>::last_->next->prev = List<Type>::last_;
      List<Type>::last_ = List<Type>::last_->next;
    }
    List<Type>::last_->next = std::make_shared<typename List<Type>::Node>
        (typename List<Type>::Node(List<Type>::last_));
  }

  class Iterator : public List<Type>::Iterator {
   public:
    explicit Iterator(const std::shared_ptr<typename List<Type>::Node> &current)
        : List<Type>::Iterator(current) {}
    Iterator &operator++() override {
      if (List<Type>::Iterator::current_->next)
        List<Type>::Iterator::current_ = List<Type>::Iterator::current_->next;
    }
    Iterator &operator--() override {
      if (List<Type>::Iterator::current_->prev)
        List<Type>::Iterator::current_ = List<Type>::Iterator::current_->prev;
    }
  };

  typename std::shared_ptr<typename List<Type>::Iterator> Begin() override {
    return std::make_shared<Iterator>(Iterator(List<Type>::first_));
  }

  typename std::shared_ptr<typename List<Type>::Iterator> End() override {
    if (List<Type>::last_) {
      return std::make_shared<Iterator>(Iterator(List<Type>::last_->next));
    } else {
      return nullptr;
    }
  }

  void Erase(typename std::shared_ptr<typename List<Type>::Iterator> &to_erase) override {
    if (!End() || (*to_erase) == (*End()))return;
    if ((*to_erase) == (*Begin())) {
      if (List<Type>::first_ == List<Type>::last_) {
        List<Type>::first_ = List<Type>::last_ = nullptr;
        to_erase = nullptr;
        return;
      } else {
        List<Type>::first_ = List<Type>::first_->next;
        List<Type>::first_->prev = nullptr;
        to_erase->current_ = List<Type>::first_;
        return;
      }
    }
    to_erase->current_->prev->next = to_erase->current_->next;
    to_erase->current_->next->prev = to_erase->current_->prev;
    to_erase->current_ = to_erase->current_->prev;
  }
};

template<class Type>
class ForwardList : public BidirectionalList<Type> {
 public:
  void PushBack(const Type &to_add) override {
    if (List<Type>::first_ == nullptr) {
      List<Type>::first_ = std::make_shared<typename List<Type>::Node>
          (typename List<Type>::Node(to_add));
    } else {
      auto tmp = List<Type>::first_;
      while (tmp->next)
        tmp = tmp->next;
      tmp->next = std::make_shared<typename List<Type>::Node>
          (typename List<Type>::Node(to_add));
    }
  }

  class Iterator : public List<Type>::Iterator {
   public:
    explicit Iterator(const std::shared_ptr<typename List<Type>::Node> &current)
        : List<Type>::Iterator(current) {}
    Iterator &operator++() override {
      if (List<Type>::Iterator::current_)
        List<Type>::Iterator::current_ = List<Type>::Iterator::current_->next;
    }
   private:
    Iterator &operator--() override {}  // do nothing,i hoped that using private will hide operator --
  };

  typename std::shared_ptr<typename List<Type>::Iterator> Begin() override {
    return std::make_shared<Iterator>(Iterator(List<Type>::first_));
  }

  typename std::shared_ptr<typename List<Type>::Iterator> End() override {
    return std::make_shared<Iterator>(Iterator(nullptr));
  }

  void Erase(typename std::shared_ptr<typename List<Type>::Iterator> &to_erase) override {
    if (!List<Type>::first_)return;
    if ((to_erase->current_) == List<Type>::first_) {
      List<Type>::first_ = List<Type>::first_->next;
      to_erase->current_ = List<Type>::first_;
      return;
    }
    auto tmp = List<Type>::first_;
    for (; tmp != nullptr; tmp = tmp->next)
      if (tmp->next == to_erase->current_)break;
    if (!tmp)return;
    tmp->next = tmp->next->next;
    to_erase->current_ = tmp;
  }
};

}  // namespace my_list

#endif  // _HOME_INEJKA_CLIONPROJECTS_MY_LIST_AND_QUEUE_LIST_H_
