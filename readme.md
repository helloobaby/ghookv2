## new hook engine





编译前提：

用vcpkg安装好spdlog、minhook





1.为什么将api跳转到shellcode，而不是hook_handler_asm？

直接jmp到handler，无法知道被hook的是哪个函数跳进来的

