// 这个头文件有隐藏依赖的问题

#include "param_helper.h"
extern std::shared_ptr<spdlog::logger> logger;

// 小心引起重入,慎用spdlog
void handler_NtAllocateVirtualMemory(minictx* context) {
}

void handler_BitBlt(minictx* context) {
  //logger->info("{} enter", __FUNCTION__);

    int x = get_2_param<int>(context);    
    int y = get_3_param<int>(context);
    int cx = get_4_param<int>(context);
    int cy = get_5_param<int>(context);

    logger->info("x:{} y:{} cx:{} cy:{}", x, y, cx, cy);
}
