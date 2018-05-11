/*
  ezpp -- Easy performance profiler for C++.

  Copyright (c) 2010-2017 <http://ez8.co> <orca.zhang@yahoo.com>

  This library is released under the MIT License.
  Please see LICENSE file or visit https://github.com/ez8-co/ezpp for details.
 */
#pragma once

#include <typeinfo>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <ctime>

#include <stdexcept>
#include <memory>
#include <cstring>
#include <cassert>
#include <cstdio>

#if  ( defined (__LP64__) || defined (__64BIT__) || defined (_LP64) \
    || ((defined(__WORDSIZE)) && (__WORDSIZE == 64)) || defined(WIN64))
  #define EZPP_X64 1
#endif

#ifdef _MSC_VER
  #include <intrin.h>
  #define LIKELY(x)                   (x)
  #define UNLIKELY(x)                 (x)
#else
  #define LIKELY(x)                   (__builtin_expect((x), 1))
  #define UNLIKELY(x)                 (__builtin_expect((x), 0))
#endif

#define EZPP_NODE_MAX                 512

#define EZPP_ADD_OPTION(option)       ::ezpp::inst().addOption(option)
#define EZPP_REMOVE_OPTION(option)    ::ezpp::inst().removeOption(option)
#define EZPP_SET_OUTPUT(file)         ::ezpp::inst().setOutputFileName(file)
#define EZPP_PRINT()                  ::ezpp::inst().print()
#define EZPP_SAVE(file)               ::ezpp::inst().save(file)
#define EZPP_CLEAR()                  ::ezpp::inst().clear()
#define EZPP_ENABLED()                ::ezpp::inst().enabled()

#ifdef _WIN32
  #define int64_t __int64
  #define PRId64 "I64d"
  #include <windows.h>
  #define EZPP_THREAD_ID              (size_t)GetCurrentThreadId()
#else
  #include <inttypes.h>
  #include <unistd.h>
  #include <sys/syscall.h>
  #define EZPP_THREAD_ID              (size_t)syscall(SYS_gettid)
#endif

//////////////////////////////////////////////////////////////////////////

#define EZPP_OPT_SAVE_IN_DTOR         0x80

#define EZPP_OPT_SORT_BY_NAME         0x40
#define EZPP_OPT_SORT_BY_CALL         0x20
#define EZPP_OPT_SORT_BY_COST         0x10

#define EZPP_OPT_FORCE_ENABLE         0x02
#define EZPP_OPT_FORCE_DISABLE        0x01

#define EZPP_OPT_SORT                 (EZPP_OPT_SORT_BY_NAME | EZPP_OPT_SORT_BY_CALL | EZPP_OPT_SORT_BY_COST)
#define EZPP_OPT_SWITCH               (EZPP_OPT_FORCE_ENABLE | EZPP_OPT_FORCE_DISABLE)

//////////////////////////////////////////////////////////////////////////

#define EZPP()                        _EZPP_AUX_BASE(, 0, "")
#define EZPP_IN_LOOP()                _EZPP_AUX_BASE(il_, EZPP_NODE_IN_LOOP, "")
#define EZPP_EX(desc)                 _EZPP_AUX_BASE(ex_, 0, desc)
#define EZPP_EX_IN_LOOP(desc)         _EZPP_AUX_BASE(ex_il_, EZPP_NODE_IN_LOOP, desc)
#define EZPP_DO()                     _EZPP_AUX_BASE(do_, EZPP_NODE_DIRECT_OUTPUT, "")
#define EZPP_EX_DO(desc)              _EZPP_AUX_BASE(ex_do_, EZPP_NODE_DIRECT_OUTPUT, desc)

//////////////////////////////////////////////////////////////////////////

#define EZPP_BEGIN(x)                 _EZPP_NO_AUX_BEGIN_BASE(cc_##x, 0, "")
#define EZPP_END(x)                   _EZPP_NO_AUX_END_BASE(cc_##x)

#define EZPP_BEGIN_EX(x, desc)        _EZPP_NO_AUX_BEGIN_BASE(cc_ex_##x, 0, desc)
#define EZPP_END_EX(x)                _EZPP_NO_AUX_END_BASE(cc_ex_##x)

#define EZPP_BEGIN_DO(x)              _EZPP_NO_AUX_BEGIN_BASE(cc_##x, EZPP_NODE_DIRECT_OUTPUT, "")
#define EZPP_END_DO(x)                _EZPP_NO_AUX_END_BASE(cc_##x)

#define EZPP_BEGIN_EX_DO(x, desc)     _EZPP_NO_AUX_BEGIN_BASE(cc_ex_do_##x, EZPP_NODE_DIRECT_OUTPUT, desc)
#define EZPP_END_EX_DO(x)             _EZPP_NO_AUX_END_BASE(cc_ex_do_##x)

//////////////////////////////////////////////////////////////////////////

#define EZPP_CLS_REGISTER()           _EZPP_CLS_REGISTER_BASE()
#define EZPP_CLS_INIT()               _EZPP_CLS_INIT_BASE(, 0, "")

#define EZPP_CLS_REGISTER_EX()        _EZPP_CLS_REGISTER_BASE(ex_)
#define EZPP_CLS_INIT_EX(desc)        _EZPP_CLS_INIT_BASE(ex_, 0, desc)

#define EZPP_CLS_REGISTER_DO()        _EZPP_CLS_REGISTER_BASE(do_)
#define EZPP_CLS_INIT_DO()            _EZPP_CLS_INIT_BASE(do_, EZPP_NODE_DIRECT_OUTPUT, "")

#define EZPP_CLS_REGISTER_EX_DO()     _EZPP_CLS_REGISTER_BASE(ex_do_)
#define EZPP_CLS_INIT_EX_DO(desc)     _EZPP_CLS_INIT_BASE(ex_do_, EZPP_NODE_DIRECT_OUTPUT, desc)

//////////////////////////////////////////////////////////////////////////

#define EZPP_ILDO_DECL(x)             _EZPP_ILDO_DECL_BASE(x, 0, "")
#define EZPP_ILDO_DECL_IL(x)          _EZPP_ILDO_DECL_BASE(x, EZPP_NODE_IN_LOOP, "")
#define EZPP_ILDO(x)                  _EZPP_ILDO_BASE(x)
#define EZPP_ILDO_BEGIN(x)            _EZPP_ILDO_BEGIN_BASE(x)
#define EZPP_ILDO_END(x)              _EZPP_ILDO_END_BASE(x)

#define EZPP_ILDO_EX_DECL(x, desc)    _EZPP_ILDO_DECL_BASE(ex_##x, 0, desc)
#define EZPP_ILDO_EX_DECL_IL(x, desc) _EZPP_ILDO_DECL_BASE(ex_##x, EZPP_NODE_IN_LOOP, desc)
#define EZPP_ILDO_EX(x)               _EZPP_ILDO_BASE(ex_##x)
#define EZPP_ILDO_EX_BEGIN(x)         _EZPP_ILDO_BEGIN_BASE(ex_##x)
#define EZPP_ILDO_EX_END(x)           _EZPP_ILDO_END_BASE(ex_##x)

//////////////////////////////////////////////////////////////////////////

#define EZPP_NODE_IN_LOOP             0x08
#define EZPP_NODE_DIRECT_OUTPUT       0x04
#define EZPP_NODE_AUTO_START          0x02
#define EZPP_NODE_CLS                 0x01

//////////////////////////////////////////////////////////////////////////

#if __cplusplus >= 201103L || _MSC_VER >= 1700
  #include <atomic>
#else
namespace std {

  typedef enum memory_order {
    memory_order_relaxed,
    memory_order_consume,
    memory_order_acquire,
    memory_order_release,
    memory_order_acq_rel,
    memory_order_seq_cst
  } memory_order;

#ifdef _MSC_VER
  template <typename T, size_t N = sizeof(T)>
  struct interlocked {};

  template <typename T>
  struct interlocked<T, 4> {
    static inline T incre(T volatile* x) {
      return static_cast<T>(_InterlockedIncrement(reinterpret_cast<volatile long*>(x)));
    }
    static inline T decre(T volatile* x) {
      return static_cast<T>(_InterlockedDecrement(reinterpret_cast<volatile long*>(x)));
    }
    static inline T add(T volatile* x, T delta) {
      return static_cast<T>(_InterlockedExchangeAdd(reinterpret_cast<volatile long*>(x), delta));
    }
    static inline T compare_exchange(T volatile* x, const T new_val, const T expected_val) {
      return static_cast<T>(
        _InterlockedCompareExchange(reinterpret_cast<volatile long*>(x),
          static_cast<const long>(new_val), static_cast<const long>(expected_val)));
    }
    static inline T exchange(T volatile* x, const T new_val) {
      return static_cast<T>(
        _InterlockedExchange(
          reinterpret_cast<volatile long*>(x), static_cast<const long>(new_val)));
    }
  };

  template <typename T>
  struct interlocked<T, 8> {
    static inline T incre(T volatile* x) {
#ifdef EZPP_X64
      return static_cast<T>(_InterlockedIncrement64(reinterpret_cast<volatile __int64*>(x)));
#else
      return add(x, 1);
#endif  // EZPP_X64
    }
    static inline T decre(T volatile* x) {
#ifdef EZPP_X64
      return static_cast<T>(_InterlockedDecrement64(reinterpret_cast<volatile __int64*>(x)));
#else
      return add(x, -1);
#endif  // EZPP_X64
    }
    static inline T add(T volatile* x, T delta) {
#ifdef EZPP_X64
      return static_cast<T>(_InterlockedExchangeAdd64(reinterpret_cast<volatile __int64*>(x), delta));
#else
      __int64 old_val, new_val;
      do {
        old_val = static_cast<__int64>(*x);
        new_val = old_val + static_cast<__int64>(delta);
      } while (_InterlockedCompareExchange64(
                 reinterpret_cast<volatile __int64*>(x), new_val, old_val) !=
               old_val);
      return static_cast<T>(new_val);
#endif  // EZPP_X64
    }
    static inline T compare_exchange(T volatile* x, const T new_val, const T expected_val) {
      return static_cast<T>(
        _InterlockedCompareExchange64(reinterpret_cast<volatile __int64*>(x), 
          static_cast<const __int64>(new_val), static_cast<const __int64>(expected_val)));
    }
    static inline T exchange(T volatile* x, const T new_val) {
#ifdef EZPP_X64
      return static_cast<T>(
        _InterlockedExchange64(reinterpret_cast<volatile __int64*>(x),
          static_cast<const __int64>(new_val)));
#else
      __int64 old_val;
      do {
        old_val = static_cast<__int64>(*x);
      } while (_InterlockedCompareExchange64(
                 reinterpret_cast<volatile __int64*>(x), new_val, old_val) !=
               old_val);
      return static_cast<T>(old_val);
#endif  // EZPP_X64
    }
  };

#else

  template<typename>
  struct hash {};

  template<>
  struct hash<size_t> {
    inline size_t operator()(size_t v) const { return v; }
  };

#endif

  template <typename T>
  class atomic {
  public:
    atomic() : value_(static_cast<T>(0)) {}
    explicit atomic(const T value) : value_(value) {}

    T operator++() {
  #ifdef _MSC_VER
      return interlocked<T>::incre(&value_);
  #else
      return __atomic_add_fetch(&value_, 1, __ATOMIC_SEQ_CST);
  #endif
    }

    T operator++(int) {
      T v = load(); ++(*this); return v;
    }

    T operator--() {
  #ifdef _MSC_VER
      return interlocked<T>::decre(&value_);
  #else
      return __atomic_sub_fetch(&value_, 1, __ATOMIC_SEQ_CST);
  #endif
    }

    T operator+=(T v) {
  #ifdef _MSC_VER
      return interlocked<T>::add(&value_, v);
  #else
      return __atomic_add_fetch(&value_, v, __ATOMIC_SEQ_CST);
  #endif
    }

    bool compare_exchange_strong(T& expected_val, T new_val, memory_order order = memory_order_seq_cst) {
  #ifdef _MSC_VER
      return expected_val == interlocked<T>::compare_exchange(&value_, new_val, expected_val);
  #else
      return __atomic_compare_exchange_n(&value_, &expected_val, new_val, 0, order, __ATOMIC_SEQ_CST);
  #endif
    }

    void store(const T new_val, memory_order order = memory_order_seq_cst) {
  #ifdef _MSC_VER
      interlocked<T>::exchange(&value_, new_val);
  #else
      __atomic_store_n(&value_, new_val, order);
  #endif
    }

    T load(memory_order order = memory_order_seq_cst) const {
  #ifdef _MSC_VER
      return interlocked<T>::add(const_cast<volatile T*>(&value_), 0);
  #else
      return __atomic_load_n(&value_, order);
  #endif
    }

    T operator=(const T new_value) {
      store(new_value);
      return new_value;
    }

    operator T() const {
      return load();
    }

  private:
    volatile T value_;
  };
}
#endif

namespace ezpp {

  #ifdef _WIN32
    namespace detail {
      LARGE_INTEGER init_freq()
      {
        LARGE_INTEGER freq;
        QueryPerformanceFrequency(&freq);
        return freq;
      }
    }
    int64_t time_now()
    {
      static LARGE_INTEGER freq = detail::init_freq();
      LARGE_INTEGER cnter;
      QueryPerformanceCounter(&cnter);
      return cnter.QuadPart / freq.QuadPart * 1000;
    }
  #else
    #include <sys/time.h>
    int64_t time_now()
    {
      timeval tv;
      gettimeofday(&tv, 0);
      return tv.tv_usec / 1000 + tv.tv_sec * 1000;
    }
  #endif

  /*
  * Copyright 2013-present Facebook, Inc.
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *   http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  */
  namespace folly {

    size_t nextPowTwo(size_t v) {
    #ifdef _MSC_VER
      unsigned long x = 0;
      _BitScanForward(&x, v - 1);
    #else
      int x = __builtin_clzll(v - 1);
    #endif
      return v ? (size_t(1) << (v - 1 ? (((sizeof(unsigned long long) << 3) - 1) ^ x) + 1 : 0)) : 1;
    }

    template <
      typename Key,
      typename Value,
      typename Hash = std::hash<Key>,
      typename KeyEqual = std::equal_to<Key>,
      template <typename> class Atom = std::atomic,
      typename IndexType = size_t,
      typename Allocator = std::allocator<char> >

    struct AtomicUnorderedMap {

    typedef Key key_type;
    typedef Value mapped_type;
    typedef std::pair<Key,Value> value_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef Hash hasher;
    typedef KeyEqual key_equal;
    typedef const value_type& const_reference;

    typedef struct ConstIterator {
      ConstIterator(const AtomicUnorderedMap& owner, IndexType slot)
        : owner_(owner)
        , slot_(slot)
      {}

      const value_type& operator*() const {
        return owner_.slots_[slot_].keyValue();
      }

      const value_type* operator->() const {
        return &owner_.slots_[slot_].keyValue();
      }

      // pre-increment
      const ConstIterator& operator++() {
        while (slot_ > 0) {
          --slot_;
          if (owner_.slots_[slot_].state() == LINKED) {
            break;
          }
        }
        return *this;
      }

      // post-increment
      ConstIterator operator++(int /* dummy */) {
        ConstIterator prev = *this;
        ++*this;
        return prev;
      }

      bool operator==(const ConstIterator& rhs) const {
        return slot_ == rhs.slot_;
      }
      bool operator!=(const ConstIterator& rhs) const {
        return !(*this == rhs);
      }

    private:
      const AtomicUnorderedMap& owner_;
      IndexType slot_;
    } const_iterator;

    friend ConstIterator;

    explicit AtomicUnorderedMap(size_t maxSize,
                      float maxLoadFactor = 0.8f,
                      const Allocator& alloc = Allocator())
      : allocator_(alloc)
    {
      size_t capacity = size_t(maxSize / (maxLoadFactor > 1.0f ? 1.0f : maxLoadFactor) + 128);
      size_t avail = size_t(1) << (8 * sizeof(IndexType) - 2);
      if (capacity > avail && maxSize < avail) {
        // we'll do our best
        capacity = avail;
      }
      if (capacity < maxSize || capacity > avail) {
        throw std::invalid_argument(
          "AtomicUnorderedMap capacity must fit in IndexType with 2 bits "
          "left over");
      }

      numSlots_ = capacity;
      slotMask_ = nextPowTwo(capacity * 4) - 1;
      mmapRequested_ = sizeof(Slot) * capacity;
      slots_ = reinterpret_cast<Slot*>(allocator_.allocate(mmapRequested_));
      memset(slots_, 0, mmapRequested_);
      // mark the zero-th slot as in-use but not valid, since that happens
      // to be our nil value
      slots_[0].stateUpdate(EMPTY, CONSTRUCTING);
    }

    ~AtomicUnorderedMap() {
      allocator_.deallocate(reinterpret_cast<char*>(slots_), mmapRequested_);
    }

    template <typename Func, typename V>
    std::pair<const_iterator, bool> findOrConstruct(const Key& key, Func func, const V* value) {
      IndexType const slot = keyToSlotIdx(key);
      IndexType prev = slots_[slot].headAndState_.load(std::memory_order_acquire);

      IndexType existing = find(key, slot);
      if (existing != 0) {
        return std::make_pair(ConstIterator(*this, existing), false);
      }

      IndexType idx = allocateNear(slot);
      new (&slots_[idx].keyValue().first) Key(key);
      func(static_cast<void*>(&slots_[idx].keyValue().second), value);

      while (true) {
        slots_[idx].next_ = prev >> 2;

        // we can merge the head update and the CONSTRUCTING -> LINKED update
        // into a single CAS if slot == idx (which should happen often)
        IndexType after = idx << 2;
        if (slot == idx) {
          after += LINKED;
        } else {
          after += (prev & 3);
        }

        if (slots_[slot].headAndState_.compare_exchange_strong(prev, after)) {
          // success
          if (idx != slot) {
            slots_[idx].stateUpdate(CONSTRUCTING, LINKED);
          }
          return std::make_pair(ConstIterator(*this, idx), true);
        }
        // compare_exchange_strong updates its first arg on failure, so
        // there is no need to reread prev

        existing = find(key, slot);
        if (existing != 0) {
          // our allocated key and value are no longer needed
          slots_[idx].keyValue().first.~Key();
          slots_[idx].keyValue().second.~Value();
          slots_[idx].stateUpdate(CONSTRUCTING, EMPTY);

          return std::make_pair(ConstIterator(*this, existing), false);
        }
      }
    }

    template <class K, class V>
    std::pair<const_iterator,bool> insert(const K& key, const V& value) {
      return findOrConstruct(key, &AtomicUnorderedMap::copyCtor<V>, &value);
    }

    const_iterator find(const Key& key) const {
      return ConstIterator(*this, find(key, keyToSlotIdx(key)));
    }

    const_iterator cbegin() const {
      IndexType slot = numSlots_ - 1;
      while (slot > 0 && slots_[slot].state() != LINKED) {
        --slot;
      }
      return ConstIterator(*this, slot);
    }

    const_iterator cend() const {
      return ConstIterator(*this, 0);
    }

    // Add by orca.zhang@yahoo.com
    void clear() {
      for (size_t i = 1; i < numSlots_; ++i) {
        slots_[i].~Slot();
      }
      memset(slots_, 0, mmapRequested_);
      slots_[0].stateUpdate(EMPTY, CONSTRUCTING);
    }

    // Add by orca.zhang@yahoo.com
    bool erase(const Key& key) const {
      KeyEqual ke;
      IndexType slot = keyToSlotIdx(key);
      IndexType hs = slots_[slot].headAndState_.load(std::memory_order_acquire);
      IndexType last_slot = 0;
      for (IndexType idx = hs >> 2; idx != 0; idx = slots_[idx].next_) {
        if (ke(key, slots_[idx].keyValue().first)) {
          if (!last_slot)
            slots_[slot].headAndState_ = (slots_[idx].next_ & (unsigned)-4) | (hs & 3);
          else
            slots_[last_slot].next_ = slots_[idx].next_;
          slots_[idx].~Slot();
          slots_[idx].stateUpdate(LINKED, EMPTY);
          return true;
        }
        last_slot = idx;
      }
      return false;
    }

    private:
      enum {
        kMaxAllocationTries = 1000, // after this we throw
      };

      typedef IndexType BucketState;

      enum {
        EMPTY = 0,
        CONSTRUCTING = 1,
        LINKED = 2,
      };

      /// Lock-free insertion is easiest by prepending to collision chains.
      /// A large chaining hash table takes two cache misses instead of
      /// one, however.  Our solution is to colocate the bucket storage and
      /// the head storage, so that even though we are traversing chains we
      /// are likely to stay within the same cache line.  Just make sure to
      /// traverse head before looking at any keys.  This strategy gives us
      /// 32 bit pointers and fast iteration.
      struct Slot {
        /// The bottom two bits are the BucketState, the rest is the index
        /// of the first bucket for the chain whose keys map to this slot.
        /// When things are going well the head usually links to this slot,
        /// but that doesn't always have to happen.
        Atom<IndexType> headAndState_;

        /// The next bucket in the chain
        IndexType next_;

        /// Key and Value
        unsigned char raw_[sizeof(value_type)];

        ~Slot() {
          BucketState s = state();
          assert(s == EMPTY || s == LINKED);
          if (s == LINKED) {
            keyValue().first.~Key();
            keyValue().second.~Value();
          }
        }

        BucketState state() const {
          return BucketState(headAndState_.load(std::memory_order_acquire) & 3);
        }

        void stateUpdate(BucketState before, BucketState after) {
          assert(state() == before);
          headAndState_ += (after - before);
        }

        value_type& keyValue() {
          assert(state() != EMPTY);
          union {
            unsigned char* p;
            value_type* v;
          } u;
          u.p = raw_;
          return *u.v;
        }

        const value_type& keyValue() const {
          assert(state() != EMPTY);
          union {
            unsigned char* p;
            value_type* v;
          } u;
          u.p = raw_;
          return *u.v;
        }

      };

      // We manually manage the slot memory so we can bypass initialization
      // (by getting a zero-filled mmap chunk) and optionally destruction of
      // the slots

      size_t mmapRequested_;
      size_t numSlots_;

      /// tricky, see keyToSlodIdx
      size_t slotMask_;

      Allocator allocator_;
      Slot* slots_;

      IndexType keyToSlotIdx(const Key& key) const {
        size_t h = hasher()(key);
        h &= slotMask_;
        while (h >= numSlots_) {
          h -= numSlots_;
        }
        return h;
      }

      IndexType find(const Key& key, IndexType slot) const {
        KeyEqual ke;
        IndexType hs = slots_[slot].headAndState_.load(std::memory_order_acquire);
        for (slot = hs >> 2; slot != 0; slot = slots_[slot].next_) {
          if (ke(key, slots_[slot].keyValue().first)) {
            return slot;
          }
        }
        return 0;
      }

      /// Allocates a slot and returns its index.  Tries to put it near
      /// slots_[start].
      IndexType allocateNear(IndexType start) {
        for (IndexType tries = 0; tries < kMaxAllocationTries; ++tries) {
          IndexType slot = allocationAttempt(start, tries);
          IndexType prev = slots_[slot].headAndState_.load(std::memory_order_acquire);
          if ((prev & 3) == EMPTY &&
            slots_[slot].headAndState_.compare_exchange_strong(
              prev, prev + CONSTRUCTING - EMPTY)) {
            return slot;
          }
        }
        throw std::bad_alloc();
      }

      /// Returns the slot we should attempt to allocate after tries failed
      /// tries, starting from the specified slot.  This is pulled out so we
      /// can specialize it differently during deterministic testing
      IndexType allocationAttempt(IndexType start, IndexType tries) const {
        if (LIKELY(tries < 8 && start + tries < numSlots_)) {
          return IndexType(start + tries);
        } else {
          IndexType rv;
          if (sizeof(IndexType) <= 4) {
            rv = IndexType(rand() % numSlots_);
          } else {
            rv = IndexType(((int64_t(rand()) << 32) + rand()) % numSlots_);
          }
          assert(rv < numSlots_);
          return rv;
        }
      }

      template<typename V>
      static void copyCtor(void* raw, const V* v) {
        assert(v);
        new (raw) Value(*v);
      }
    };

    /// MutableAtom is a tiny wrapper than gives you the option of atomically
    /// updating values inserted into an AtomicUnorderedMap<K,
    /// MutableAtom<V>>.  This relies on AtomicUnorderedMap's guarantee
    /// that it doesn't move values.
    template <typename T, template <typename> class Atom = std::atomic>
    struct MutableAtom {
      mutable Atom<T> data;
      explicit MutableAtom(const T& init) : data(init) {}
    };

    /// MutableData is a tiny wrapper than gives you the option of using an
    /// external concurrency control mechanism to updating values inserted
    /// into an AtomicUnorderedMap.
    template <typename T>
    struct MutableData {
      mutable T data;
      explicit MutableData(const T& init) : data(init) {}
    };

  } // namespace folly

  class node_desc {
  public:
    node_desc(const char* file = 0, int line = 0, const std::string& name = "", const std::string& ext = "")
      : file(file)
      , _line(line)
      , _endLine(0)
      , _name(name)
      , _ext(ext)
    {}

    void outputFullDesc(FILE* fp) const {
      if (_line) {
        fprintf(fp, "%s (%s:%d", _name.c_str(), file, _line);
        if (_endLine) {
          fprintf(fp, "~%d", _endLine);
        }
        fprintf(fp, ")");
      }
      if (!_ext.empty()) {
        fprintf(fp, " \"%s\"", _ext.c_str());
      }
    }

    inline void endLine(int endLine) { _endLine = endLine; }

    inline bool operator==(const node_desc &other) const {
      return _line == other._line && _name == other._name && !strcmp(file, other.file) && _ext == other._ext;
    }

    inline size_t hash() const {
      return _line ^ fast_hash(file, strlen(file)) ^ 
        fast_hash(_name.data(), _name.size()) ^ fast_hash(_ext.data(), _ext.size());
    }

    inline const std::string& name() const { return _name; }

  private:
    const char* file;
    int         _line;
    int         _endLine;
    std::string _name; // __FUNCTION__ \ typeid(*this).name()
    std::string _ext;

    static size_t fast_hash(const char* str, size_t len) {
#ifdef EZPP_X64
        static const size_t mask[] = {0xFF00000000000000ULL, 0xFFFF000000000000ULL, 0xFFFFFF0000000000ULL, 
          0xFFFFFFFF00000000ULL, 0xFFFFFFFFFF000000ULL, 0xFFFFFFFFFFFF0000ULL, 0xFFFFFFFFFFFFFF00ULL};
#else
        static const size_t mask[] = {0xFF000000UL, 0xFFFF0000UL, 0xFFFFFF00UL};
#endif
      if(!len) return 0;
      else if(len < sizeof(size_t)) return *(size_t*)str & mask[len - 1];
      return *(size_t*)str ^ fast_hash(str + sizeof(size_t), len - sizeof(size_t));
    }
  };

  class node {
  public:
    friend class ezpp;

    inline const std::string& name() const { return _desc.name(); }
    inline int64_t callCnt() const         { return _callCnt; }
    inline int64_t costTime() const        { return _totalCost; }
    inline bool checkInUse()               { return _totalRefCnt > 0; }
    inline void setReleaseUntilEnd()       { _releaseUntilEnd = true; }
    inline void endLine(int endLine)       { _desc.endLine(endLine); }

    void begin(int64_t c12n);
    void end(int64_t c12n);
    void call(int64_t c12n);

    void output(FILE* fp);

  protected:
    static inline void atomic_init(void* raw, const folly::MutableAtom<int64_t>*) {
      *(int64_t*)raw = 0;
    }

    node_desc _desc;

    typedef folly::AtomicUnorderedMap<size_t, folly::MutableAtom<int64_t> > time_map;
    time_map _beginMap;
    time_map _costMap;
    time_map _refMap;

    std::atomic<int64_t> _start;
    std::atomic<int64_t> _totalCost;
    std::atomic<int64_t> _callCnt;
    std::atomic<int64_t> _totalRefCnt;

    unsigned char _flags;
    bool _releaseUntilEnd;

  private:
    explicit node(const node_desc& desc, int64_t c12n, unsigned char flags);
  };

  class node_aux {
  public:
    node_aux(node *n = 0, int64_t c12n = 0) : _n(n), _c12n(c12n){}
    inline void set(node *n, int64_t c12n) { _n = n; _c12n = c12n; }
    ~node_aux() { if (_n) _n->end(_c12n); }

  private:
    node *_n;
    int64_t _c12n;
  };

  class ezpp {
  public:
    static node* create(const node_desc& desc, int64_t c12n, unsigned char flags = EZPP_NODE_AUTO_START);
    static void release(const std::pair<node_desc, folly::MutableData<node*> >& node_pair);

    void addOption(unsigned char optModify);
    void removeOption(unsigned char optModify);

    inline void setOutputFileName(const std::string &file) { _file = file; }
    std::string getOutputFileName();

    void print();
    void save(const std::string& file = "");
    void clear();
    inline bool enabled() { return _enabled; }

  protected:
    ezpp(int/* dummy */);
    ~ezpp();

  protected:
    struct node_desc_hasher {
      size_t operator()(const node_desc& desc) { return desc.hash(); }
    };

    friend class node;
    friend ezpp& inst();

    static int init() {
      std::srand(time(0));
      return 0;
    }

    void removeDoNode(const node_desc& desc);

    void output(FILE* fp);
    static void outputTime(FILE* fp, int64_t duration);

    typedef folly::AtomicUnorderedMap<node_desc, folly::MutableData<node*>, node_desc_hasher> node_map;

    node_map _doMap;
    node_map _nodeMap;

    int64_t _begin;

    unsigned char _option;

    bool _enabled;

    std::string _file;
  };

  ezpp& inst() {
    static ezpp inst(ezpp::init());
    return inst;
  }

  namespace detail {
    static bool NameSort(node* lhs, node* rhs) {
      return lhs->name() < rhs->name();
    }

    static bool CallCntSort(node* lhs, node* rhs) {
      return lhs->callCnt() < rhs->callCnt();
    }

    static bool CostTimeSort(node* lhs, node* rhs) {
      return lhs->costTime() < rhs->costTime();
    }
  }

  // protected
  void
  ezpp::outputTime(FILE* fp, int64_t duration) {
    double seconds = (double)duration / 1000;
    double minute = seconds / 60;
    double hour = minute / 60;

    if ((int)hour > 0) {
      fprintf(fp, "%.0f hour%s, ", hour, hour > 1 ? "s" : "");
    }

    minute = (int64_t)minute % 60;
    if ((int)minute > 0) {
      fprintf(fp, "%.0f min%s, ", minute, minute > 1 ? "s" : "");
    }

    seconds -= duration / 60000;
    if (seconds < 1) {
      fprintf(fp, "%2.2f ms", seconds * 1000);
    }
    else {
      fprintf(fp, "%2.2f sec%s", seconds, seconds > 1 ? "s" : "");
    }
  }

  // protected
  ezpp::ezpp(int/* dummy */)
    : _doMap(EZPP_NODE_MAX)
    , _nodeMap(EZPP_NODE_MAX)
    , _begin(0)
    , _option(0)
    , _enabled(false)
    , _file()
  {}

  // protected
  ezpp::~ezpp() {
    print();
    if (_enabled && (_option & EZPP_OPT_SAVE_IN_DTOR)) {
      save();
    }
    clear();
  }

  // public static
  node* 
  ezpp::create(const node_desc& desc, int64_t c12n, unsigned char flags /*= EZPP_NODE_AUTO_START*/) {
    if (!inst().enabled() || !flags) {
      return 0;
    }
    node_map& map = (flags & EZPP_NODE_DIRECT_OUTPUT) ? inst()._doMap : inst()._nodeMap;
    node_map::const_iterator it = map.find(desc);
    if (it != map.cend()) {
      it->second.data->call(c12n);
      return it->second.data;
    }
    node* n = new node(desc, c12n, flags);
    map.insert(desc, n);
    return n;
  }

  // public static
  void
  ezpp::release(const std::pair<node_desc, folly::MutableData<node*> >& node_pair) {
    if (!node_pair.second.data->checkInUse()) {
      delete node_pair.second.data;
    }
    else {
      node_pair.second.data->setReleaseUntilEnd();
    }
  }

  void
  ezpp::output(FILE* fp) {
    if (_nodeMap.cbegin() != _nodeMap.cend()) {
      fprintf(fp, "========== Easy Performance Profiler Report ==========\r\n");

      std::vector<node *> array;
      for (node_map::const_iterator it = _nodeMap.cbegin(); it != _nodeMap.cend(); ++it) {
        array.push_back(it->second.data);
      }

      if ((_option & EZPP_OPT_SORT_BY_NAME) || !(_option & EZPP_OPT_SORT)) {
        std::sort(array.begin(), array.end(), detail::NameSort);
        fprintf(fp, "\r\n     [Sort By Name]\r\n\r\n");
        for (unsigned i = 0; i < array.size(); ++i) {
          fprintf(fp, "No.%u\r\n", i + 1);
          array[i]->output(fp);
        }
      }
      
      if (_option & EZPP_OPT_SORT_BY_CALL) {
        std::sort(array.begin(), array.end(), detail::CallCntSort);
        fprintf(fp, "\r\n     [Sort By Call]\r\n\r\n");
        for (unsigned i = 0; i < array.size(); ++i) {
          fprintf(fp, "No.%u\r\n", i + 1);
          array[i]->output(fp);
        }
      }

      if (_option & EZPP_OPT_SORT_BY_COST) {
        std::sort(array.begin(), array.end(), detail::CostTimeSort);
        fprintf(fp, "\r\n     [Sort By Cost]\r\n\r\n");
        for (unsigned i = 0; i < array.size(); ++i) {
          fprintf(fp, "No.%u\r\n", i + 1);
          array[i]->output(fp);
        }
      }

      fprintf(fp, "====== [Total Time Elapsed] ");
      outputTime(fp, time_now() - _begin);
      time_t timep;
      time(&timep);
      char tmp[64];
      strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
      fprintf(fp, " ======\r\n====== [Generate Date] %s ======\r\n", tmp);
    }
  }

  // public
  void 
  ezpp::print() {
    output(stdout);
  }

  // public
  std::string
  ezpp::getOutputFileName() {
    return _file.empty() ? "ezpp.log" : _file;
  }

  // public
  void
  ezpp::save(const std::string& file/* = ""*/) {
    FILE* fp = fopen(file.empty() ? getOutputFileName().c_str() : file.c_str(), "wb+");
    if(!fp) return;
    output(fp);
    fclose(fp);
  }

  // public
  void
  ezpp::clear() {
    std::for_each(_doMap.cbegin(), _doMap.cend(), ezpp::release);
    _doMap.clear();
    std::for_each(_nodeMap.cbegin(), _nodeMap.cend(), ezpp::release);
    _nodeMap.clear();
    _begin = time_now();
  }

  // protected
  void
  ezpp::removeDoNode(const node_desc& desc) {
    _doMap.erase(desc);
  }

  // public
  void
  ezpp::addOption(unsigned char optModify) {
    if (optModify & EZPP_OPT_SWITCH) {
      if ((optModify & EZPP_OPT_FORCE_ENABLE) && !_enabled) {
        _enabled = true;
        _begin = time_now();
        _option &= ~EZPP_OPT_SWITCH;
        _option |= EZPP_OPT_FORCE_ENABLE;
      }
      if ((optModify & EZPP_OPT_FORCE_DISABLE) && _enabled) {
        _enabled = false;
        _option &= ~EZPP_OPT_SWITCH;
        _option |= EZPP_OPT_FORCE_DISABLE;
      }
    }
    if (optModify & EZPP_OPT_SORT) {
      _option &= ~EZPP_OPT_SORT;
      _option |= (optModify & EZPP_OPT_SORT);
    }
    _option |= (optModify & EZPP_OPT_SAVE_IN_DTOR);
  }

  // public
  void
  ezpp::removeOption(unsigned char optModify) {
    if ((optModify & EZPP_OPT_FORCE_DISABLE) && !_enabled) {
      _enabled = true;
      _begin = time_now();
    }
    if ((optModify & EZPP_OPT_FORCE_ENABLE) && _enabled) {
      _enabled = false;
    }
    _option &= ~optModify;
  }

  // protected
  node::node(const node_desc& desc, int64_t c12n, unsigned char flags)
    : _desc(desc)
    , _beginMap(EZPP_NODE_MAX)
    , _costMap(EZPP_NODE_MAX)
    , _refMap(EZPP_NODE_MAX)
    , _start(0)
    , _totalCost(0)
    , _callCnt(1)
    , _totalRefCnt(1)
    , _flags(flags)
    , _releaseUntilEnd(false)
  {
    if (_flags & EZPP_NODE_AUTO_START)
      begin(c12n);
    else
      ++_totalRefCnt;
    _start = time_now();
  }

  // public
  void 
  node::begin(int64_t c12n) {
    if (_beginMap.cbegin() != _beginMap.cend()) {
      call(c12n);
      return;
    }
    _beginMap.insert(c12n, time_now());
    _costMap.insert(c12n, 0);
    _refMap.insert(EZPP_THREAD_ID, 1);
  }

  #define _GET_(m, k) m.findOrConstruct(k, atomic_init, (const folly::MutableAtom<int64_t>*)0).first->second.data

  // public
  void 
  node::call(int64_t c12n) {
    int64_t now = time_now();
    if (!_GET_(_refMap, c12n)++ || (_flags & EZPP_NODE_CLS))
      _GET_(_beginMap, c12n) = now;
    if (!_totalRefCnt)
      _start = now;
    ++_totalRefCnt;
    ++_callCnt;
  }

  // public
  void 
  node::end(int64_t c12n) {
    --_totalRefCnt;
    int64_t now = time_now();
    if (!--_GET_(_refMap, c12n) || (_flags & EZPP_NODE_CLS)) {
      _GET_(_costMap, c12n) += now - _GET_(_beginMap, c12n);
    }
    if (!_totalRefCnt) {
      _totalCost += now - _start;

      if ((_flags & EZPP_NODE_DIRECT_OUTPUT)) {
        output(stdout);
        inst().removeDoNode(_desc);
        delete this;
      }
      else if (_releaseUntilEnd)
        delete this;
    }
  }

  #undef _GET_

  // public
  void
  node::output(FILE* fp) {
    fprintf(fp, "[Category] ");
    _desc.outputFullDesc(fp);
    fprintf(fp, "\r\n");
    if (_totalRefCnt)
      fprintf(fp, "Warning: Unbalance detected! Mismatch or haven't been ended yet!\r\n");
    fprintf(fp, "[Time] ");
    ezpp::outputTime(fp, _totalCost);
    if (_totalRefCnt) {
      fprintf(fp, " (+ ");
      ezpp::outputTime(fp, time_now() - _start);
      fprintf(fp, ")");
    }
    time_map::const_iterator it = _costMap.cbegin();
    if (it != _costMap.cend()) {
      if (++it == _costMap.cend()) {
        fprintf(fp, (_flags & EZPP_NODE_CLS) ? "   (Object : 0x%0x)" : "   (Thread ID : %u)",
          (unsigned)_costMap.cbegin()->first);
        fprintf(fp, "\r\n");
      }
      else {
        fprintf(fp, "\r\n");
        int64_t total = 0;
        size_t costTimeSize = 0;
        for (time_map::const_iterator it = _costMap.cbegin(); it != _costMap.cend(); ++it) {
          fprintf(fp, (_flags & EZPP_NODE_CLS) ? "    (Object : 0x%0x) " :"    (Thread ID : %u) ",
            (unsigned)it->first);
          ezpp::outputTime(fp, it->second.data);
          fprintf(fp, "\r\n");
          total += it->second.data;
          ++costTimeSize;
        }
        fprintf(fp, "  [Avg] ");
        ezpp::outputTime(fp, total / costTimeSize);
        fprintf(fp, "\r\n");
        fprintf(fp, "  [Total] ");
        ezpp::outputTime(fp, total);
        fprintf(fp, "\r\n");
      }
    }
    fprintf(fp, "[Call] %" PRId64 "\r\n\r\n", _callCnt.load());
  }
}

//////////////////////////////////////////////////////////////////////////

#define _EZPP_SUB_CHECK(expression)            if (::ezpp::inst().enabled()) {expression;}

#define _EZPP_AUX_BASE(sign, flags, desc)      \
  ::ezpp::node_aux _ezpp_a_##sign;             \
  _EZPP_SUB_CHECK(_ezpp_a_##sign.set(::ezpp::ezpp::create(::ezpp::node_desc(__FILE__, __LINE__, __FUNCTION__, desc), EZPP_THREAD_ID, EZPP_NODE_AUTO_START | flags), EZPP_THREAD_ID))

#define _EZPP_NO_AUX_BEGIN_BASE(sign, flags, desc) \
  ::ezpp::node *_ezpp_na_##sign##_ = 0;        \
  _EZPP_SUB_CHECK(_ezpp_na_##sign##_ = ::ezpp::ezpp::create(::ezpp::node_desc(__FILE__, __LINE__, __FUNCTION__, desc), EZPP_THREAD_ID, EZPP_NODE_AUTO_START | flags))

#define _EZPP_NO_AUX_END_BASE(sign)            \
  if (_ezpp_na_##sign##_) {                    \
    _ezpp_na_##sign##_->endLine(__LINE__);     \
    _ezpp_na_##sign##_->end(EZPP_THREAD_ID);   \
  }

#define _EZPP_CLS_REGISTER_BASE(sign)          \
  protected:                                   \
    ::ezpp::node_aux _ezpp_cls_##sign;         \
  public:                                      \

#define _EZPP_CLS_INIT_BASE(sign, flags, desc) \
  _EZPP_SUB_CHECK(_ezpp_cls_##sign.set(::ezpp::ezpp::create(::ezpp::node_desc(__FILE__, __LINE__, typeid(*this).name(), desc), (int64_t)this, EZPP_NODE_AUTO_START | EZPP_NODE_CLS | flags), (int64_t)this))

#define _EZPP_ILDO_DECL_BASE(sign, flags, desc)\
  ::ezpp::node *_ezpp_ildo_##sign##_ = 0;      \
  _EZPP_SUB_CHECK(_ezpp_ildo_##sign##_ = ::ezpp::ezpp::create(::ezpp::node_desc(__FILE__, __LINE__, __FUNCTION__, desc), EZPP_THREAD_ID, EZPP_NODE_DIRECT_OUTPUT | flags))

#define _EZPP_ILDO_BASE(sign)                  \
  ::ezpp::node_aux _ezpp_a_ildo_##sign##_(_ezpp_ildo_##sign##_, EZPP_THREAD_ID);\
  if (_ezpp_ildo_##sign##_) _ezpp_ildo_##sign##_->begin(EZPP_THREAD_ID);

#define _EZPP_ILDO_BEGIN_BASE(sign)            \
  if (_ezpp_ildo_##sign##_) _ezpp_ildo_##sign##_->call(EZPP_THREAD_ID);

#define _EZPP_ILDO_END_BASE(sign)              \
  if (_ezpp_ildo_##sign##_) _ezpp_ildo_##sign##_->end(EZPP_THREAD_ID);
