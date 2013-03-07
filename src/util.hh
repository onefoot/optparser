// util.hh

#ifndef __UTIL_HH__
#define __UTIL_HH__

#include <stdexcept.hh>

namespace optparser {

  // sp:
  //   A reference-counting smart pointer.
  // 
  template <class T>
  class sp {
    T* ptr;
    size_t* refcount;

  public:
    // constructor & destructor
    sp(): ptr(0), refcount(0) {}
    sp(const sp& src);
    sp(T* ps);
    ~sp();

    // operator
    sp& operator=(const sp& src);
    sp& operator=(T* ps);
    T* operator->() const;
    T& operator*() const;
    operator T* () const;

    // For debugging.
    size_t count() const { return refcount ? *refcount : 0; }

  };


  // sp:
  //   Member functions.
  // 
  template <class T>
  sp<T>::sp(const sp<T>& src): ptr(0), refcount(0) {
    if (src.refcount) {
      ptr = src.ptr;
      refcount = src.refcount;
      ++(*refcount);
    }
  }


  template <class T>
  sp<T>::sp(T* ps): ptr(0), refcount(0) {
    if (ps) {
      ptr = ps;
      refcount = new size_t(1);
    }
  }


  template <class T>
  sp<T>::~sp() {
    if (refcount && *refcount) {
      --(*refcount);
      if (! *refcount) {
	delete ptr;
	delete refcount;
      }
    }
  }


  template <class T>
  sp<T>&
  sp<T>::operator=(const sp& src) {
    T* t_ptr = ptr;
    size_t* t_refcount = refcount;

    ptr = src.ptr;
    refcount = src.refcount;
    if (refcount) {
      ++(*refcount);
    }

    if (t_refcount && *t_refcount) {
      --(*t_refcount);
      if (! *t_refcount) {
	delete t_ptr;
	delete t_refcount;
      }
    }  
    return *this;
  }


  template <class T>
  sp<T>&
  sp<T>::operator=(T* ps) {
    T* t_ptr = ptr;
    size_t* t_refcount = refcount;

    ptr = ps;
    if (ps) {
      refcount = new size_t(1);
    }
    else {
      refcount = 0;
    }

    if (t_refcount && *t_refcount) {
      --(*t_refcount);
      if (! *t_refcount) {
	delete t_ptr;
	delete t_refcount;
      }
    }  
    return *this;
  }


  template <class T>
  T*
  sp<T>::operator->() const {
    if (ptr) {
      return ptr;
    }
    throw std::runtime_error("sp error: The pointer is null.");
  }


  template <class T>
  T&
  sp<T>::operator*() const {
    if (ptr) {
      return *ptr;
    }
    throw std::runtime_error("sp error: The pointer is null.");
  }


  template <class T>
  sp<T>::operator T* () const {
    return ptr;
  }
  
}


#endif
