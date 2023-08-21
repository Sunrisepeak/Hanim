## 设计思想

HAnim是基于 **动画执行器(HEngine)**, **动画行为(HAnimate)**, **动画对象(HObject)** 三者"分离"的思想进行架构和设计的

在HAnim的视角中, 一个动画被划分为**动画行为(HAnimate)**, **动画对象(HObject)** , 通过**动画执行器(HEngine)**把一个**动画行为(HAnimate)**作用到一个**动画对象(HObject)**并进行"播放", 就形成了一个完整的动画

即一个动画可以描述为如下形式:

> **Animation =  HEngine(HAnimate, HObject)**

其中 HAnimate, HObject 为独立变量

通过HAnim动画框架让**动画行为(HAnimate)** 和 **动画对象(HObject)**的设计与开发进行了解耦, 增加更多的复用可能性



- **HEngine:** 动画的执行器, 用于把一个动画(行为)作用到一个对象上 并进行"渲染"
- **HAnimate:** 动画(行为), 描述一个或一种动画的行为.在HAnim体系中一般也称HAnimate为动画
- **HObject:** 动画对象, 动画(行为)的载体, 可视为动画的显视"实体"



注: HAnim体系中名词的一般称谓 引擎(HEngine)、动画(HAnimate)、动画对象(HObject)



### 1.HAnimate 设计





### 2.HObject 设计





### 3.HEngine 设计