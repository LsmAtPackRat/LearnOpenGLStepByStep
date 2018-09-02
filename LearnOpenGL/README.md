# 前言 & 第一节





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

上述例子中，

- 先创建了一个对象，用一个id保存它的引用；
- 然后将对象绑定至上下文的目标位置（例子中窗口对象目标的位置被定义成GL_WINDOW_TARGET）；
- 接下来设置窗口的选项；
- 最后将目标位置的对象id设回0，**解绑**这个对象；
- 设置的选项被保存在objectId所引用的对象中，一旦我们**重新绑定**这个对象到GL_WINDOW_TARGET位置，这些选项就会**重新生效**。

上述是一个OpenGL的使用的一个大致描述。

在程序中**使用对象的好处**：我们可以定义多个对象，并设置它们的选项，每个对象都可以是**不同的设置**。在我们执行一个使用OpenGL状态的操作的时候，只需要**绑定**含有需要的设置的对象即可。比如说我们有一些作为3D模型数据（一栋房子或一个人物）的容器对象，在我们想绘制其中任何一个模型的时候，只需绑定一个包含对应模型数据的对象就可以了（当然，我们需要先创建并设置对象的选项）。拥有数个这样的对象允许我们指定多个模型，在想画其中任何一个的时候，直接将对应的对象绑定上去，便**不需要再重复设置选项**了。



## 附加资源

- [opengl.org](https://www.opengl.org/)：OpenGL官方网站。
- [OpenGL registry](https://www.opengl.org/registry/)：包含OpenGL各版本的规范和扩展。



# 第三节

## 创建窗口

本教程使用GLAD库取代了之前老教程中的GLEW库。

在画出图形之前，首先要做的就是创建一个OpenGL上下文(Context)和一个用于显示的窗口。然而，这些操作在每个系统上都是不一样的，这些操作被从OpenGL中抽象(Abstract)出去。这意味着我们不得不自己处理**创建窗口**，**定义OpenGL上下文**以及**处理用户输入**。

有一些库已经提供了我们所需的功能，这些库提供给我们一个窗口和上下文用来渲染，从而节省了我们写操作系统相关的代码的时间。最流行的几个库有GLUT，SDL，SFML和GLFW。在教程里我们将使用**GLFW**。



## GLFW & GLAD配置

g++使用-I选项（**-Iinclude**）来将include文件夹中的头文件包含进来。所以include中放上要使用的头文件。



## 附加资源

- [GLFW: Window Guide](http://www.glfw.org/docs/latest/window_guide.html)：GLFW官方的配置GLFW窗口的指南。
- [Building applications](http://www.opengl-tutorial.org/miscellaneous/building-your-own-c-application/)：提供了很多编译或链接相关的信息和一大列错误及对应的解决方案。
- [GLFW with Code::Blocks](http://wiki.codeblocks.org/index.php?title=Using_GLFW_with_Code::Blocks)：使用Code::Blocks IDE编译GLFW。
- [Running CMake](http://www.cmake.org/runningcmake/)：简要的介绍如何在Windows和Linux上使用CMake。
- [Writing a build system under Linux](http://learnopengl.com/demo/autotools_tutorial.txt)：Wouter Verholst写的一个autotools的教程，讲的是如何在Linux上编写构建系统，尤其是针对这些教程。
- [Polytonic/Glitter](https://github.com/Polytonic/Glitter)：一个简单的样板项目，它已经提前配置了所有相关的库；如果你想要很方便地搞到一个LearnOpenGL教程的范例工程，这也是很不错的。



# 第四节

## 你好，窗口









