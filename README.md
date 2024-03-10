# Hanim (Hi Animation / Hello Animation)

Hanim是一个轻量级的动画框架.

**Animation =  HEngine(HAnimate, HObject)**

## 特性/功能

- 动画对象和动画行为(分离)设计(也支持耦合模式)
- 支持**立即模式**GUI库(opengl/imgui等)
- 支持**保留模式**GUI库(Qt/其他)
- 内置**基础缓动曲线**并支持**自定义扩展**
- Header-Only库, 无三方依赖(需std & C++ >= 17)
- 支持动画控制,API简单易用

## 用法/示例

```cpp

    ImGui::Begin("Hello Hanim");

    HPlayWidget(1, hanim::Loading("Loading", {100, 100}, {200, 30}));

    HPlayWidget(
        2,
        hanim::Loading(
            "Hello Hanim", {100, 150}, {200, 30},
            60, ImVec4(1.f, 0, 0, 0.5f), ImVec4(0, 0.5f, 0, 0.8f)
        )
    );

    ImGui::End();

```

![hanim-loading](docs/imgs/hanim.demo.loading.gif)

---

[文档仓库](https://github.com/Sunrisepeak/hanim-docs)
[在线文档](https://speaks-organization.gitbook.io/hanim+imgui-dong-hua-kong-jian-she-ji/)