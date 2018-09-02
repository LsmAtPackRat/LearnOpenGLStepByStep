# 第二节

## OpenGL

OpenGL本身并不是一个API，它仅仅是Khronos组织制定并维护的规范。OpenGL库的开发者可以自行决定实现方式（在遵守规范的情况下）。OpenGL的大多数实现通常是由显卡厂商编写的，位于显卡驱动里。

以下的笔记是基于OpenGL3.3（一个架构的关键版本）进行的。



## 核心模式与立即渲染模式

- **立即渲染模式**（Immediate mode）：即固定渲染管线，早期OpenGL使用，现已经废弃，OpenGL的大多数功能都被库隐藏起来，开发者很少能控制OpenGL的计算。这种模式虽然绘制图形很简单，但是效率太低。
- **核心模式**（Core-profile）：即可编程管线。OpenGL3.2开始废弃固定渲染管线，转而使用此模式。核心模式需要使用者真正理解OpenGL和图形编程，有一定难度，但是它更加灵活以及高效率。

注意：新版本的OpenGL特性有时候只有新一代的显卡支持。



## 扩展

OpenGL支持扩展（Extension），当一个显卡公司提出一个新特性或者渲染上的大优化，通常会以扩展的方式在驱动中实现（如果显卡能够支持的话）。这样，OpenGL本身的特性相当于规范相当于扩展了。一个广泛使用的扩展最终会成为未来OpenGL的规范的一部分。

使用扩展的代码示例：


```c++
if(GL_ARB_extension_name)
{
    // 使用硬件支持的全新的现代特性
}
else
{
    // 不支持此扩展: 用旧的方式去做
}
```



## 状态机

OpenGL自身是一个巨大的状态机：一系列的变量描述OpenGL此时应当如何运行。OpenGL的状态通常被称为**OpenGL上下文**(Context)。

我们通常使用如下途径去更改OpenGL状态：

- 设置选项（例如，设置接下来画线/三角形）；
- 操作缓冲；
- 使用当前OpenGL上下文来渲染。



使用OpenGL时，会使用一些函数：

- **状态设置函数**(State-changing Function)：这类函数将会改变上下文。
- **状态使用函数**(State-using Function)：这类函数会根据当前OpenGL的状态执行一些操作。



## 对象

OpenGL的内核是C语言编写的。OpenGL引入了一些抽象层，“对象（Object）”就是其中一个。

在OpenGL中一个**对象**是指**一些选项的集合**，它代表**OpenGL状态的一个子集**。比如，我们可以用一个对象来代表绘图窗口的设置，之后我们就可以设置它的大小、支持的颜色位数等等。可以把对象看做一个C风格的结构体(Struct)：

```c++
struct object_name {
	float  option1;
	int    option2;
	char[] name;
};
```

使用OpenGL的基本类型的好处是保证了在各个平台中每一种类型的大小是统一的。你也可以使用其它的定宽类型(Fixed-width Type)来实现这一点。



OpenGL上下文可以看作一个大的结构体：

```c++
// OpenGL的状态
struct OpenGL_Context {
	...
	object* object_Window_Target;
	...     
};
```

使用OpenGL是常见的工作流如下：
```c++
// 创建对象
unsigned int objectId = 0;
glGenObject(1, &objectId);
// 绑定对象至上下文
glBindObject(GL_WINDOW_TARGET, objectId);
// 设置当前绑定到 GL_WINDOW_TARGET 的对象的一些选项
glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_WIDTH, 800);
glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_HEIGHT, 600);
// 将上下文对象设回默认
glBindObject(GL_WINDOW_TARGET, 0);
```










