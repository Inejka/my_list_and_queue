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
  }

  virtual void PushBack(const Type &to_add) = 0;

 protected:
  struct Node {
    Node() = default;
    explicit Node(const Type &key) : key(key) {}
    std::shared_ptr<Node> next = nullptr;
    Type key;
  };
  std::shared_ptr<Node> first_;

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
class ForwardList : public List<Type> {
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

  void Erase(typename std::shared_ptr
      <typename List<Type>::Iterator> &to_erase) override {
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

template<class Type>
class BidirectionalList : public ForwardList<Type> {
 protected:
  struct ExtendedNode : public List<Type>::Node {
    explicit ExtendedNode(const std::shared_ptr<ExtendedNode> &prev)
        : prev(prev) {}
    explicit ExtendedNode(const Type &to_add)
        : List<Type>::Node(to_add) {}
    std::shared_ptr<ExtendedNode> prev = nullptr;
  };
  std::shared_ptr<ExtendedNode> last_;

 public:
  void PushBack(const Type &to_add) override {
    auto first_ = std::static_pointer_cast<ExtendedNode>(List<Type>::first_);
    if (List<Type>::first_ == nullptr &&
        last_ == nullptr) {
      List<Type>::first_ = last_ =
          std::make_shared<ExtendedNode>
              (ExtendedNode(to_add));
    } else if (first_ == last_) {
      last_ = std::make_shared<ExtendedNode>
          (ExtendedNode(to_add));
      List<Type>::first_->next = last_;
      last_->prev = first_;
    } else {
      last_->next = std::make_shared<ExtendedNode>
          (ExtendedNode(to_add));
      auto tmp = std::static_pointer_cast<ExtendedNode>(last_->next);
      tmp->prev = last_;
      last_ = tmp;
    }
    last_->next = std::make_shared<ExtendedNode>(ExtendedNode(last_));
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
      auto tmp = std::static_pointer_cast
          <ExtendedNode>(List<Type>::Iterator::current_);
      if (tmp->prev)
        List<Type>::Iterator::current_ = tmp->prev;
    }
  };

  typename std::shared_ptr<typename List<Type>::Iterator> Begin() override {
    return std::make_shared<Iterator>(Iterator(List<Type>::first_));
  }

  typename std::shared_ptr<typename List<Type>::Iterator> End() override {
    if (last_) {
      return std::make_shared<Iterator>(Iterator(last_->next));
    } else {
      return nullptr;
    }
  }

  void Erase(typename std::shared_ptr
      <typename List<Type>::Iterator> &to_erase) override {
    auto first_ = std::static_pointer_cast<ExtendedNode>(List<Type>::first_);
    if (!End() || (*to_erase) == (*End()))return;
    if ((*to_erase) == (*Begin())) {
      if (first_ == last_) {
        List<Type>::first_ = last_ = nullptr;
        to_erase = nullptr;
        return;
      } else {
        List<Type>::first_ = List<Type>::first_->next;
        first_->prev = nullptr;
        to_erase->current_ = List<Type>::first_;
        return;
      }
    }
    auto tmp = std::static_pointer_cast<ExtendedNode>(to_erase->current_);
    tmp->prev->next = tmp->next;
    auto tmp1 = std::static_pointer_cast<ExtendedNode>(tmp->next);
    tmp1->prev = tmp->prev;
    to_erase->current_ = tmp->prev;
  }
};

}  // namespace my_list

#endif  // _HOME_INEJKA_CLIONPROJECTS_MY_LIST_AND_QUEUE_LIST_H_
