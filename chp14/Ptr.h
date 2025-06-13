// Ptr.h
#pragma once
#ifndef GUARD_Ptr_h
#define GUARD_Ptr_h

#include <cstddef>
#include <stdexcept>

using namespace std;

template <class T> class Ptr {
public:
	// 필요하면 조건에 따라 객체를 복사하는 새로운 멤버 함수
	void make_unique() {
		if (*refptr != 1) {
			--*refptr;
			refptr = new size_t(1);
			p = p ? p->clone() : 0;
		}
	}

	// 참조 카운트를 포인터처럼 관리
	Ptr() : p(0), refptr(new size_t(1)) { }
	Ptr(T* t) : p(t), refptr(new size_t(1)) { }
	Ptr(const Ptr& h) : p(h.p), refptr(h.refptr) {
		++*refptr;

	}
	Ptr& operator=(const Ptr&);
	~Ptr();

	//이전과 같음
	operator bool() const {
		return p;
	}
	T& operator*() const {
		if (p) return *p;
		throw runtime_error("unbound Ptr");
	}
	T* operator->() const {
		if (p) return p;
		throw runtime_error("unbound Ptr");
	}
private:
	T* p;
	size_t* refptr;
};

template<class T> T* clone(const T* tp);

template<>
Vec<char>* clone(const Vec<char>* vp) {
	return new Vec<char>(*vp);
}

// 새로운 함수 
template <class T> T* clone(const T* tp) {
	return tp->clone();
}

template <class T>
Ptr<T>& Ptr<T>::operator=(const Ptr& rhs) {
	++*rhs.refptr;

	// 상황에 따라 좌변을 할당 해제하고 포인터 소멸
	if (--*refptr == 0) {
		delete refptr;
		delete p;
	}

	// 우변 값들을 복사
	refptr = rhs.refptr;
	p = rhs.p;

	return *this;
}

template <class T> Ptr<T>::~Ptr() {
	if (--*refptr == 0) {
		delete refptr;
		delete p;
	}
}

#endif

