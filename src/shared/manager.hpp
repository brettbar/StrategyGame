#pragma once


template<typename T>
class Manager {
  public:
  Manager(const Manager &) = delete;
  Manager &operator=(const Manager &) = delete;

  static T *Get() {
    static T instance;
    return &instance;
  }

  protected:
  // Protected constructor to prevent direct instantiation
  Manager() = default;
};
