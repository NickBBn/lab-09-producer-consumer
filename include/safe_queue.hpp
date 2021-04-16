//
// Created by niickson on 4/16/21.
//

#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <iostream>
#include <queue>
#include <mutex>

template <typename T>
class safe_queue {
  T front();
  void pop();
  void push(const T& obj);
  void push(T&& obj);
  bool is_empty();

 private:
  std::queue<T> _queue;
  std::mutex _mutex;
};

template <typename T>
T safe_queue<T>::front() {
  std::lock_guard<std::mutex> lock(_mutex);
  return _queue.front();
}

template <typename T>
void safe_queue<T>::pop() {
  std::lock_guard<std::mutex> lock(_mutex);
  if (!_queue.empty()) return _queue.pop();
}

template <typename T>
void safe_queue<T>::push(const T& obj) {
  std::lock_guard<std::mutex> lock(_mutex);
  return _queue.push(obj);
}

template <typename T>
void safe_queue<T>::push(T&& obj) {
  std::lock_guard<std::mutex> lock(_mutex);
  return _queue.push(std::forward<>(obj));
}

template <typename T>
bool safe_queue<T>::is_empty() {
  return _queue.empty();
}

#endif  // QUEUE_HPP
