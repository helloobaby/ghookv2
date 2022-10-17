#include "minictx.h"

// 按照x64fastcall调用约定
// https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention?view=msvc-170
// https://learn.microsoft.com/en-us/cpp/build/x64-software-conventions?view=msvc-170
// https://learn.microsoft.com/en-us/cpp/build/stack-usage?view=msvc-170
// 目前不支持带浮点参数的函数和大于8字节的单个参数,比如__m128
 


template <typename _T>
_T get_1_param(minictx* ctx) {
  ULONG64 rcx = ctx->Rcx;
  return (_T)rcx;
}

template <typename _T>
_T get_2_param(minictx* ctx) {
  ULONG64 rdx = ctx->Rdx;
  return (_T)rdx;
}

template <typename _T>
_T get_3_param(minictx* ctx) {
  ULONG64 r8 = ctx->R8;
  return (_T)r8;
}

template <typename _T>
_T get_4_param(minictx* ctx) {
  ULONG64 r9 = ctx->R9;
  return (_T)r9;
}

template <typename _T>
_T get_5_param(minictx* ctx) {
  DWORD64 rsp = (DWORD64)ctx->Rsp;
  return (_T)*((ULONG64*)((ULONG64)rsp + 0x28));
}

template <typename _T>
_T get_6_param(minictx* ctx) {
  DWORD64 rsp = (DWORD64)ctx->Rsp;
  return (_T) * ((ULONG64*)((ULONG64)rsp + 0x28 +8));
}

template <typename _T>
_T get_7_param(minictx* ctx) {
  DWORD64 rsp = (DWORD64)ctx->Rsp;
  return (_T) * ((ULONG64*)((ULONG64)rsp + 0x28+8+8));
}

template <typename _T>
_T get_8_param(minictx* ctx) {
  DWORD64 rsp = (DWORD64)ctx->Rsp;
  return (_T) * ((ULONG64*)((ULONG64)rsp + 0x28+8+8+8));
}

template <typename _T>
_T get_9_param(minictx* ctx) {
  DWORD64 rsp = (DWORD64)ctx->Rsp;
  return (_T) * ((ULONG64*)((ULONG64)rsp + 0x28+8+8+8+8));
}

template <typename _T>
_T get_10_param(minictx* ctx) {
  DWORD64 rsp = (DWORD64)ctx->Rsp;
  return (_T) * ((ULONG64*)((ULONG64)rsp + 0x28+8+8+8+8+8));
}

template <typename _T>
void write_1_param(minictx ctx,_T value) {	//_T为一个值或者一个指针
  ctx->Rcx = (ULONG64)value;
  return;
}

template <typename _T>
void write_2_param(minictx ctx,_T value) {  
  ctx->Rdx = (ULONG64)value;
  return;
}

template <typename _T>
void write_3_param(minictx ctx,_T value) {
  ctx->R8 = (ULONG64)value;
  return;
}

template <typename _T>
void write_4_param(minictx ctx,_T value) {
  ctx->R9 = (ULONG64)value;
  return;
}

template <typename _T>
void write_5_param(minictx ctx,_T* value) {
  DWORD64 rsp = (DWORD64)ctx->Rsp;
  *((ULONG64*)((ULONG64)rsp + 0x28)) = (ULONG64)value;
}

template <typename _T>
void write_6_param(minictx ctx,_T* value) {
  DWORD64 rsp = (DWORD64)ctx->Rsp;
  *((ULONG64*)((ULONG64)rsp + 0x28+8)) = (ULONG64)value;
}

template <typename _T>
void write_7_param(minictx ctx,_T* value) {
  DWORD64 rsp = (DWORD64)ctx->Rsp;
  *((ULONG64*)((ULONG64)rsp + 0x28+8+8)) = (ULONG64)value;
}

template <typename _T>
void write_8_param(minictx ctx,_T* value) {
  DWORD64 rsp = (DWORD64)ctx->Rsp;
  *((ULONG64*)((ULONG64)rsp + 0x28+8+8+8)) = (ULONG64)value;
}

template <typename _T>
void write_9_param(minictx ctx,_T* value) {
  DWORD64 rsp = (DWORD64)ctx->Rsp;
  *((ULONG64*)((ULONG64)rsp + 0x28+8+8+8+8)) = (ULONG64)value;
}

template <typename _T>
void write_10_param(minictx ctx,_T* value) {
  DWORD64 rsp = (DWORD64)ctx->Rsp;
  *((ULONG64*)((ULONG64)rsp + 0x28+8+8+8+8+8)) = (ULONG64)value;
}