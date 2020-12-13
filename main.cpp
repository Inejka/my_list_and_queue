#include <iostream>
#include <ctime>

#include "List.h"
#include "Queue.h"

void ForwardListTest();
void BidirectionalListTest();
void QueueTest();
int main() {
  std::srand(std::time(0));
  ForwardListTest();
  BidirectionalListTest();
  QueueTest();
}

void ForwardListTest() {
  std::cout << "ForwardListTest\n";
  std::unique_ptr<my_list::List<int>> test =
      std::make_unique<my_list::ForwardList<int>>();
  for (int i = 0; i < 20; i++)
    test->PushBack(rand() % 10);
  for (auto i = test->Begin(); (*i) != (*test->End()); ++(*i))
    std::cout << **i << ' ';
  for (auto i = test->Begin(); (*i) != (*test->End()); ++(*i))
    if ((**i) % 2 == 1)test->Erase(i);
  std::cout << "\nAfter deleting odd numbers:\n";
  for (auto i = test->Begin(); (*i) != (*test->End()); ++(*i))
    std::cout << **i << ' ';
}

void BidirectionalListTest() {
  std::cout << "\nBidirectionalListTest\n";
  std::unique_ptr<my_list::List<int>> test =
      std::make_unique<my_list::BidirectionalList<int>>();
  for (int i = 0; i < 20; ++i)
    test->PushBack(rand() % 10);
  for (auto i = test->Begin(); (*i) != (*test->End()); ++(*i))
    std::cout << **i << ' ';
  for (auto i = test->Begin(); (*i) != (*test->End()); ++(*i))
    if ((**i) % 2 == 1)test->Erase(i);
  std::cout << "\nAfter deleting odd numbers:\n";
  for (auto i = test->Begin(); (*i) != (*test->End()); ++(*i))
    std::cout << **i << ' ';
}

void QueueTest() {
  std::cout << "\nQueueTest\n";
  std::unique_ptr<my_queue::Queue<int>> test =
      std::make_unique<my_queue::Queue<int>>
          (my_queue::Queue<int>(std::make_unique<my_list::BidirectionalList<int>>()));  // Тут видимо я неправильно понял задание,я могу создать очередь на основе любого списка
  for (int i = 0; i < 20; ++i) {
    int i1 = rand() % 10;
    std::cout << "Now adding " << i1 << " to queue\n";
    test->Push(i1);
  }
  std::cout << "Using queue\n";
  while (!test->Empty()) {
    std::cout << test->Front() << ' ';
    test->Pop();
  }
}
