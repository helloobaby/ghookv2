// 这个头文件有隐藏依赖的问题

#include "param_helper.h"
extern std::shared_ptr<spdlog::logger> logger;

void handler_NtAllocateVirtualMemory(minictx* context) {

  //DbgBreakPoint(); //中断到windbg


  //size_t* r9 = get_4_param<size_t*>(context);
  //DWORD AllocType = get_5_param<DWORD>(context);
  //DWORD ProtectType = get_6_param<DWORD>(context);
  //logger->info(std::format(L"[-] NtAllocateVirtualMemory size {:#x}\n", *r9));
  //logger->info(std::format(L"[-] NtAllocateVirtualMemory AlloctionType {:#x}\n",AllocType));
  //logger->info(std::format(L"[-] NtAllocateVirtualMemory ProtectType {:#x}\n",ProtectType));
}

void handler_BitBlt(minictx* context) {
  //logger->info("{} enter", __FUNCTION__);

    int x = get_2_param<int>(context);    
    int y = get_3_param<int>(context);
    int cx = get_4_param<int>(context);
    int cy = get_5_param<int>(context);

    logger->info("x:{} y:{} cx:{} cy:{}", x, y, cx, cy);
}

void handler_NtReadFile(minictx* context) {

}