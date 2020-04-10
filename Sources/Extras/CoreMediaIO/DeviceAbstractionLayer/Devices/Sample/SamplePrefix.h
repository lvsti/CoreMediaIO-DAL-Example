/*
	    File: SamplePrefix.h
	Abstract: Items used to facilitate building on Lion or Mountain Lion with the same sources.
	 Version: 1.2

*/

#if !defined(__SamplePrefix_h__)
#define __SamplePrefix_h__

#include <functional>

class Defer {
public:
    Defer(std::function<void()> deferredFunc): _deferredFunc(deferredFunc) {}
    ~Defer() { _deferredFunc(); }
private:
    std::function<void()> _deferredFunc;
};

#define _CONCAT(x, y) x ## y
#define _CONCAT2(x, y) _CONCAT(x, y)
#define DEFER(...) Defer _CONCAT2(_defer, __LINE__)( __VA_ARGS__ );

#endif
