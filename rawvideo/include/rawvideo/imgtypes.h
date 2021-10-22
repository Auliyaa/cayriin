#pragma once

#include <cinttypes>
#include <cstdlib>
#include <array>
#include <cstdio>
#include <memory>

namespace rawvideo
{

template<typename __store, size_t __cnt>
using __px_t = std::array<__store,__cnt>;

using px8_t   = __px_t<uint8_t ,3>;
using px10_t  = __px_t<uint16_t,3>;
using px16_t  = __px_t<uint16_t,3>;

template<typename T>
T& bclamp(T& x, size_t s) { x &= ((static_cast<T>(1)<<s)-1); return x; }

// fopen wrappers
using fd_ptr_t = std::unique_ptr<::FILE,decltype(&fclose)>;
inline fd_ptr_t make_fd_ptr(::FILE* p) { return fd_ptr_t(p,&fclose); }
inline fd_ptr_t fopen_ptr(const char *__restrict __filename,
                          const char *__restrict __modes)
{ return make_fd_ptr(::fopen(__filename,__modes)); }

} // ::rawvideo
