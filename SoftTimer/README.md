# [SoftTimer]软件定时器



>这个库是使用`C++`和`C`分别实现的软件定时器库，支持使用静态或动态的方式创建定时器，可以很方便的管理内存，能有效避免存泄露的风险，可自行选择使用`C`或`C++`版本

---

## 安装

1. 从`gitHub`[下载](https://github.com/jiewen-zheng/SoftTimer.git)这个库的最新版本.
2. 解压后将文件夹`SoftTimer`添加到你的项目当中.
3. 将文件`SoftTimer.h`中的`SOFT_TIMER_MILLIS`宏替换为当前项目中获取时间戳的函数.
4. 选择`C`或`C++`头文件引用至项目中.



## 开始

如果你还不知道怎么使用这个库,请查看以下内容,或参考库中的例子.



- 使用静态方法创建定时器

```
// 实例化一个定时器控制器对象
SoftTimer sTimer;

// 定义一个定时器的回调函数
void timer0_callback(void *)
{
}

// 声明一个静态定时器 tiemr0, 这个定时器超时时间为1秒
// 使用循环运行模式"stModeCirculate"
softTimerDef(timer0, 1000, stModeCirculate, timer0_callback, nullptr);

// 创建这个定时器
sTimer.create(softTimer(timer0));

// 让定时器跑起来
sTimer.start("timer0");

// 在主循环中执行定时器控制器的主要方法
sTimer.process(); // 方法必须尽可能快的被调用,否则定时器可能无法正常运行

```



- 使用动态方法创建定时器 ，使用动态创建时需要开启宏`SOFT_TIMER_DYNAMIC_EN`

```
 // 实例化一个定时器控制器对象
SoftTimer sTimer;

// 定义一个定时器的回调函数
void timer1_callback(void *)
{
}

// 动态创建定时器 tiemr1, 超时时间为500毫秒
// 使用单次运行模式"stModeOnce"
// 这个定时器执行完回调函数后会将自己删除
sTimer.create("timer1", 500, timer1_callback, nullptr, stModeOnce);

// 让定时器跑起来
sTimer.start("timer1");

// 在主循环中执行定时器控制器的主要方法
sTimer.process(); // 方法必须尽可能快的被调用,否则定时器可能无法正常运行
```

  

## 历史版本

- `1.1 (2023-06-25)`：添加C语言版本支持
- `1.0 (2023-06-17)`：原始版本
