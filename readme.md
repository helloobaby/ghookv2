用minhook库实现用户层的hook dll框架

监视哪个，就注入DLL到哪个进程



ProcessHacker库的头文件有个问题，缺少点东西，具体看下面  
https://www.mail-archive.com/mingw-w64-public@lists.sourceforge.net/msg18925.html



vcpkg安装spdlog    vcpkg install spdlog[wchar]:x86-windows-static    vcpkg install spdlog[wchar]:x64-windows-static



API日志格式为

```
[时间][进程全路径][API名][API参数信息]
```



用spdlog库的时候有个问题，他依赖fmt，而vcpkg默认以dll的形式来链接，结果他会生成一个fmt.dll和ghook.dll，但是我们很显然只能注入一个dll。所以我们必须要vcpkg以静态的形式链接。

##### 做法：项目的配置属性 -> vcpkg -> Use Static Libraries 选择是











注意事项：

hook的代理函数要声明为NTAPI的，因为我们写的函数都是_cdecl约定，windows api都是_stdcall。

