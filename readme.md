## new hook engine



编译前提：

用vcpkg安装好spdlog[wchar]、minhook





run_ghook项目运行的时候会默认加载ghook项目编译的dll，桌面上也会生成个日志txt，被hook的函数一些参数会打印在这里。



注意事项：

 1.目前仅支持x64编译。  

2.不能开优化 （好像跟spdlog有关系，不过无所谓）