# [SoftTimer]软件定时器



>这个库是使用`C++`和`C`分别实现的软件定时器库，支持使用静态或动态的方式创建定时器，可以很方便的管理内存，能有效避免存泄露的风险，可自行选择使用`C`或`C++`版本

---

## 安装

1. 从`gitHub`[下载](https://github.com/jiewen-zheng/Utils/tree/main/soft_timer)这个库的最新版本.
2. 解压后将文件夹`SoftTimer`添加到你的项目当中.



## 开始

如果你还不知道怎么使用这个库,请查看以下内容,或参考库中的例子.



- 使用静态方法创建定时器，关闭宏`SOFT_TIMER_DYNAMIC_EN`

```
#define SOFT_TIMER_DYNAMIC_EN 0 // 赋值0表示启用静态数组存储定时器句柄

#define SOFT_TIMER_MAX_NUM  16  // 设置最大定时器数量

```



- 使用动态方法创建定时器 ，开启宏`SOFT_TIMER_DYNAMIC_EN`

```
//! 实例化一个定时器控制器对象
SoftTimer sTimer(timestamp); //! 将"timestamp"替换会获取时间戳的函数指针

//! 定义一个定时器的回调函数
void timer1_callback(void *)
{
	printf("timer1 timeout.\n");
}

/** 动态创建定时器 tiemr1, 超时时间为 500ms
 * 使用单次运行模式"OnceMode"
 * 这个定时器执行完回调函数后会将删除自身并回收内存。
 */
sTimer.create("timer1", 500, timer1_callback, nullptr, OnceMode);

//! 让定时器跑起来
sTimer.start("timer1");

//! 在主循环中执行定时器控制器的主要方法
sTimer.process(); //! 方法必须尽可能快的被调用,否则定时器的超时时间可能错位
```

  

## 注意

这个库使用到了"std::vector"会连接`C++`库，如果你是小型嵌入式设备对存储空间有要求，请使用`C`语言版本的”soft_timer“库，它会一定程度的缩减代码大小。



## 历史版本

- `1.0 (2023-06-17)`：原始版本
