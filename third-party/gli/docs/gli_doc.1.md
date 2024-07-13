# GLI: 抽象层接口的头文件设计 - 函数实现和动态库symbol冲突解决思路记录



# 背景

最近在写一个[mini的图形库抽象层GLI](https://github.com/Sunrisepeak/gl-interface), 为了保持单接口文件。在头文件的实现和设计上遇到了 **接口文件中函数实现 - 重复定义 和 动态库symbol 二者不可兼得** 这样的一个问题。问题可以简单描述如下:

- 接口头文件中需要有函数实现
- 要避免多重定义问题(即头文件被包含到多个源文件中)
- 引入`static`修饰后引起了**动态库函数symbol找不到的问题**

下面就从问题的引入及解决思路上来介绍一下这个过程


# 问题起因

由于`gli_interface.h`中不仅要定义接口签名, 还需要实现把"直观"图形定义分解成顶点组来描述图形。这就需要在接口文件中**即要定义接口签名, 还要做对应的函数实现**, 于是就有了如下函数

```cpp
// gl-interface.h 接口文件的函数实现
void gli_rectangle(PointGLI p1, PointGLI p2, ColorGLI col, float thickness) {
    struct GraphicsDataGLI data;

    float deltaX = p2.x - p1.x;
    float deltaY = p2.y - p1.y;
    float deltaZ = p2.z - p1.z;

    float vertices[3 * 4] = {
        p1.x, p1.y, p1.z, // LT
        p1.x + deltaX, p1.y, p1.z + deltaZ, // RT
        p2.x, p2.y, p2.z, // RB
        p1.x, p1.y + deltaY, p1.z // LB
    };

    data.mode.draw = GLI_LINE_LOOP;
    data.mode.color = GLI_COL_ONE;
    data.vertexNums = 4;
    data.vertices = vertices;
    data.colors = &col.r;

    gli_draw(&data);
}
```

这样的实现在构建动态库的时候并不会有什么问题, 但当开发者使用 **`gli_x.cpp + gl_interface.h`** 或 **`libgli-xx.a + gl_interface.h`** 的组合去使用时, 就会报如下问题 - `multiple definition of 'gli_rectangle'`

```C++
//  gli_x.cpp + gl_interface.h
/usr/bin/ld: build/.objs/gli-opengl-c/linux/x86_64/release/examples/opengl/main.c.o: in function `gli_rectangle':
main.c:(.text+0x1e0): multiple definition of `gli_rectangle'; build/.objs/gli-opengl-c/linux/x86_64/release/backends/gli_opengl.cpp.o:gli_opengl.cpp:(.text+0x19e0): first defined here

// libgli-xx.a + gl_interface.h
/usr/bin/ld: build/linux/x86_64/release/libgli-opengl-static.a(gli_opengl.cpp.o): in function `gli_rectangle':
gli_opengl.cpp:(.text+0x19e0): multiple definition of `gli_rectangle'; build/.objs/gli-opengl-c/linux/x86_64/release/examples/opengl/main.c.o:main.c:(.text+0x1e0): first defined here
```


# 使用static关键字做文件隔离

对于上面的问题, 我们可以引入static来修饰, 把头文件中的函数隔离在每个文件域中

```cpp
static void gli_rectangle(PointGLI p1, PointGLI p2, ColorGLI col, float thickness) {
 //...
}
```

这样基本解决了这个问题。在c/cpp中可以完美编译和使用了

```C++
speak@speak-pc:~/workspace/github/gl-interface$ xmake build gli-opengl-c
[ 37%]: cache compiling.release backends/gli_opengl.cpp
[ 50%]: cache compiling.release examples/opengl/main.c
[ 62%]: archiving.release libgli-opengl-static.a
[ 87%]: linking.release gli-opengl-c
[100%]: build ok, spent 0.086s
```

但是, 在做gli对应的python的ffi接口的时候发现, 动态库中找不到对应的函数symbol - `undefined symbol: gli_rectangle`

```pyhon
speak@speak-pc:~/workspace/github/gl-interface$ xmake r gli-opengl-py
python - gli_backend_init is available.
python - gli_backend_init: 0x7aea6c114300
Traceback (most recent call last):
  File "/home/speak/workspace/github/gl-interface/examples/opengl/main.py", line 8, in <module>
    from api.python.gl_interface import *
  File "/home/speak/workspace/github/gl-interface/api/python/gl_interface.py", line 80, in <module>
    gli_loader()
  File "/home/speak/workspace/github/gl-interface/api/python/gl_interface.py", line 44, in gli_loader
    _gli_lib.gli_rectangle.argtypes = [PointGLI, PointGLI, ColorGLI, ctypes.c_float]
  File "/usr/lib/python3.10/ctypes/__init__.py", line 387, in __getattr__
    func = self.__getitem__(name)
  File "/usr/lib/python3.10/ctypes/__init__.py", line 392, in __getitem__
    func = self._FuncPtr((name_or_ordinal, self))
AttributeError: /home/speak/workspace/github/gl-interface/build/linux/x86_64/release/libgli-opengl.so: undefined symbol: gli_rectangle
error: exec(python3 examples/opengl/main.py) failed(1)
```

**由于引入的static, 表示了函数被限制为内部链接(internal linkage), 导致python中在动态库的符号表中没有找到**


# 引入GLI_API控制符号导出

通过上面的背景介绍, 遇到了一个矛盾

- 为了解决重复定义 - 引入static
- static导致函数不可见 - 阻碍了库的symbol导出

在不改变 **单接口文件 + 有函数实现** 的基础下, 恍惚进入了死循环。但仔细分析后如果能实现**接口的灵活定义**不就能解决问题了吗? 即:

- **编译库的时候:** 它是没有static修饰的
- **使用是接口文件的时候:** 它是有static修饰的

于是有了下面的一个解决方案 —— 通过GLI_API宏来动态控制接口的定义

```cpp
// gl_interface.h
#ifdef GLI_BUILD_LIB
#define GLI_API
#else
#define GLI_API static inline
#endif

GLI_API void gli_rectangle(PointGLI p1, PointGLI p2, ColorGLI col, float thickness) {
 //...
}
```

通过`GLI_API`修饰的接口, 并本着不增加库使用者的使用负担的原则, **规定gli库的后端渲染实现中来用GLI_BUILD_LIB进行标识, 这是一个 GLI的实现 而是不是GLI库的应用程序**

```cpp
// backends - gli_opengl.cpp
// GL
#include <glad/glad.h>
#include <GL/gl.h>

// GLI
#define GLI_BUILD_LIB
#include <gl_interface.h>

//...
```

这样就通过一个简单的`GLI_API`接口控制宏, 解决了这个问题



### 最后

总体来说, 通过`GLI_API`基本较完美的解决了这个问题, 至于有没有其他更优雅的方法, 目前还有想到。如果你有更优雅的思路, 欢迎评论区交流讨论

---

[GLI - 一个迷你的图形库抽象层](https://github.com/Sunrisepeak/gl-interface)