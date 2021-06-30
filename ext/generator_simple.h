#pragma once
// based on: https://github.com/roger-dv/cpp20-coro-generator

#if __has_include(<coroutine>)
#  include <coroutine>
#elif __has_include(<experimental/coroutine>)
//#  warning "Using experimental coroutine implementation!"
#  include <experimental/coroutine>
namespace std {
  using namespace experimental;
}
#else
#  error "Coroutine implementation not found."
#endif
#include <memory>     
#include <iostream>
     
namespace coroexp {
  template<typename T>
  class generator {
  public:
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
  private:
    handle_type coro;
  public:
    explicit generator(handle_type h): coro(h) {}
    generator(const generator &) = delete;
    generator(generator &&oth) noexcept: coro(oth.coro) {
      oth.coro = nullptr;
    }
    generator &operator=(const generator &) = delete;
    generator &operator=(generator &&other) noexcept {
      coro = other.coro;
      other.coro = nullptr;
      return *this;
    }
    ~generator() {
      if (coro) {
        coro.destroy();
      }
    }

    bool next() {
      coro.resume();
      return not coro.done();
    }

    T getValue() {
      return coro.promise().current_value;
    }

    void destroy() {
      if (coro) {
        coro.destroy();
        coro = nullptr;
      }
    }

    struct promise_type {
    private:
      T current_value{};
      friend class generator;
    public:
      promise_type() = default;
      ~promise_type() = default;
      promise_type(const promise_type &) = delete;
      promise_type(promise_type &&) = delete;
      promise_type &operator=(const promise_type &) = delete;
      promise_type &operator=(promise_type &&) = delete;

      auto initial_suspend() {
        return std::suspend_always{};
      }

      auto final_suspend() noexcept{
        return std::suspend_always{};
      }

      auto get_return_object() {
        return generator{handle_type::from_promise(*this)};
      }

      void return_void() {}

      auto yield_value(T some_value) {
        current_value = some_value;
        return std::suspend_always{};
      }

      void unhandled_exception() {
        exit(1);
      }
    };
  };
} 