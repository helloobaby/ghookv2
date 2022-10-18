// ���ͷ�ļ�����������������

#include "param_helper.h"
extern std::shared_ptr<spdlog::logger> logger;

// 
namespace backup {
WCHAR MaxPath[MAX_PATH];
}

// 
void handler_NtAllocateVirtualMemory(minictx* context) {
  size_t* RegionSize = get_4_param<size_t*>(context);
}

void handler_BitBlt(minictx* context) {
    int x = get_2_param<int>(context);    
    int y = get_3_param<int>(context);
    int cx = get_4_param<int>(context);
    int cy = get_5_param<int>(context);

    logger->info("x:{} y:{} cx:{} cy:{}", x, y, cx, cy);
}

void handler_NtCreateFile(minictx* context) {
  POBJECT_ATTRIBUTES oa = get_3_param<POBJECT_ATTRIBUTES>(context);

  // ������һ����΢��ȫ��
  memcpy(backup::MaxPath, oa->ObjectName->Buffer, oa->ObjectName->Length);

  logger->info(std::format(L"[-] NtCreateFile path {}", backup::MaxPath));
}