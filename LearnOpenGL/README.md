# 前言 & 第一节

本教程的网址是[LearnOpenGL CN](https://learnopengl-cn.github.io/)。



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

创建一个`.cpp`文件，并且在文件的最前面：

```c++
#include <glad/glad.h>
#include <GLFW/glfw3.h>
```

注意上面两行的顺序不能够颠倒。GLAD的头文件包含了正确的OpenGL头文件（例如`GL/gl.h`），所以需要在其它依赖于OpenGL的头文件之前包含GLAD（我觉得这个是header guard的写法问题？先不管这个）。

创建main函数，其中实例化GLFW窗口：
```c++
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	return 0;
}
```

以上。



## 你好，三角形

先记住三个单词：

- 顶点数组对象：Vertex Array Object，VAO
- 顶点缓冲对象：Vertex Buffer Object，VBO
- 索引缓冲对象：Element Buffer Object，EBO或Index Buffer Object，IBO

在OpenGL中，任何事物都在**3D空间**中，而**屏幕和窗口**却是**2D像素数组**，这导致OpenGL的大部分工作都是关于把3D坐标转变为适应你屏幕的2D像素。3D坐标转为2D坐标的处理过程是由OpenGL的**图形渲染管线**（**Graphics Pipeline**，大多译为管线，实际上指的是一堆原始图形数据途经一个输送管道，期间经过各种变化处理最终出现在屏幕的过程）管理的。

图形渲染管线可以被划分为两个主要部分：

- 第一部分把**3D坐标**转换为**2D坐标**；
- 第二部分是把**2D坐标**转变为实际的**有颜色的像素**。

> 2D坐标和像素也是不同的，2D坐标精确表示一个点在2D空间中的位置，而2D像素是这个点的近似值，2D像素受到你的屏幕/窗口分辨率的限制。

上面一段话是很容易理解的，像素本身是有一定的尺寸的，虽然很小，但是会导致2D坐标无法精确表示。所以2D像素是一个近似值。

图形渲染管线接受一组3D坐标，然后把它们转变为你屏幕上的有色2D像素输出。图形渲染管线可以被划分为几个阶段，每个阶段将会把前一个阶段的输出作为输入。所有这些阶段都是高度专门化的（它们都有一个特定的函数），并且很容易**并行执行**。正是由于它们具有并行执行的特性，当今大多数显卡都有成千上万的**小处理核心**，它们在GPU上为每一个（渲染管线）阶段运行各自的小程序，从而在图形渲染管线中快速处理你的数据。这些**小程序**叫做**着色器(Shader)**。OpenGL着色器是用OpenGL着色器语言(OpenGL Shading Language, GLSL)写成的。



下图是图形渲染pipeline的每个阶段的抽象表示（要注意**蓝色部分**代表的是我们可以注入**自定义的着色器**的部分）：

![](images/pipeline.png)

概括性地解释一下上面图形渲染管线的各部分的功能：

- **Input**（输入顶点数据到图形渲染管线）：以数组的形式传递3个3D坐标作为图形渲染管线的输入，用来表示一个三角形，这个数组叫做顶点数据(Vertex Data)；**顶点数据是一系列顶点的集合**。一个顶点(Vertex)是一个3D坐标的数据的集合。而顶点数据是用**顶点属性**(Vertex Attribute)表示的，它可以包含任何我们想用的数据，但是简单起见，我们假定每个顶点只由一个3D位置和一些颜色值组成。
- **Part-1**（顶点着色器，Vertex Shader）：把一个单独的顶点作为输入。顶点着色器主要的目的是把3D坐标转为另一种3D坐标（后面会解释），同时顶点着色器允许我们对顶点属性进行一些基本处理。
- **Part-2**（图元装配，Primitive Assembly）：
  - 首先解释图元：为了让OpenGL知道我们的坐标和颜色值构成的到底是什么，OpenGL需要你去指定这些数据所表示的渲染类型。我们是希望把这些数据渲染成一系列的点？一系列的三角形？还是仅仅是一个长长的线？做出的**这些提示叫做图元**(Primitive)，**任何一个绘制指令的调用都将把图元传递给OpenGL**。这是其中的几个：GL_POINTS、GL_TRIANGLES、GL_LINE_STRIP。
  - 图元装配阶段将**顶点着色器输出的所有顶点**作为输入（如果是GL_POINTS，那么就是一个顶点），并将所有的点装配成指定图元的形状；本节例子中的图元是一个三角形。
- **Part-3**（几何着色器，Geometry Shader）：把图元形式的一系列顶点的集合作为输入，它可以通过产生新顶点构造出新的（或是其它的）图元来生成其他形状。例子中，它生成了另一个三角形。
- **Part-4**（光栅化，Rasterization Stage）：把图元映射为最终屏幕上相应的像素，生成供片段着色器(Fragment Shader)使用的片段(Fragment)，OpenGL中的一个**片段**是**OpenGL渲染一个像素所需的所有数据**。在片段着色器运行之前会执行**裁切**(Clipping)。**裁切会丢弃超出你的视图以外的所有像素**，用来提升执行效率。
- **Part-5**（片段着色器，Fragment Shader）：计算一个像素的最终颜色，这也是**所有OpenGL高级效果产生的地方**。通常，片段着色器包含3D场景的数据（比如光照、阴影、光的颜色等等），这些数据可以被用来计算最终像素的颜色。
- **Part-6**（Alpha测试和混合，tests & Blending）：检测片段的对应的深度（和模板(Stencil)）值（后面会讲），用它们来判断这个像素是其它物体的前面还是后面，决定是否应该丢弃。这个阶段也会检查alpha值（alpha值定义了一个物体的透明度）并对物体进行混合(Blend)。所以，即使在片段着色器中计算出来了一个像素输出的颜色，在渲染多个三角形的时候最后的像素颜色也可能完全不同。

大多数场合，我们只需要配置**顶点着色器**和**片段着色器**就行了（在现代OpenGL中，我们**必须**定义至少一个顶点着色器和一个片段着色器，因为GPU中没有默认的顶点/片段着色器）。几何着色器是可选的，通常使用它默认的shader即可。



### 顶点输入























