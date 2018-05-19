# ezpp

[![license](https://img.shields.io/badge/license-MIT-brightgreen.svg?style=flat)](https://github.com/ez8-co/ezpp/blob/master/LICENSE)  [![Build Status](https://semaphoreci.com/api/v1/orca-zhang-91/ezpp/branches/master/shields_badge.svg)](https://semaphoreci.com/orca-zhang-91/ezpp)  [![Build status](https://ci.appveyor.com/api/projects/status/x1s4dckwtdmtq25r?svg=true)](https://ci.appveyor.com/project/orca-zhang/ezpp)

- C++性能剖析器（跨平台、无锁、智能）
- Easy Performance Profiler that is cross-platform, lock-free and smart for C++.

## 性能优化的几个层次

#### 原则：基于真实的度量。（《重构——改善现有代码的设计》P69 哪怕你完全了解系统，也请实际度量它的性能，不要臆测。臆测会让你学到一些东西，但十有八九你是错的。）

- 第一个层次是语法层面，要基础扎实，《Efficient C++》、《More Efficient C++》、《Effective C++》、《More Effective C++》熟读于心，不要在语法上产生不必要的性能损耗，回头再来改，成本就大了，比如迭代器前++、RVO/NRVO、内联、避免多余拷贝等等。
- 第二个层次是常规的算法改进、流程优化等等，比如加位图、树改HASH、递归改循环、分支预测、缓存行、并行、异步、批量等等。
- 第三个层次是架构层面的改进，直接去除不必要的流程、模块等等，毕竟没有比不存在的东西性能更快的了。
- 第四个层次是直接加机器（scale-out）或者升级机器配置（scale-up），人力成本可能要比机器贵很多，而很多机器实际是空闲的，没必要去为了扣性能重构现有已经稳定运行的系统，容易引入风险（这条看业务场景，公有云、互联网业务适用，嵌入式、私有云、依赖低成本机器资源的业务不适用）。

## TODO

- 🔥 智能性能分析
  - 🔥 递归调用提示
  - 🔥 热点调用标记
  - 🔥 细分建议
- 🔥 树形调用链
- 🔥 支持在线剖析（golang pprof的http / 管道ipc）
- 🔥 耗时火焰图（golang pprof）
- 🔥 支持协程
- 耗时百分比
- 其他优化
