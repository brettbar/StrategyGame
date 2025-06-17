#pragma once


template<typename T>
class Singleton {
  public:
  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;

  static T *Get() {
    static T instance;
    return &instance;
  }

  protected:
  // Protected constructor to prevent direct instantiation
  Singleton() = default;
};
