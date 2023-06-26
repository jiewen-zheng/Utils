# [SimpleList]简单链表库
>这是一个使用`C语言`实现的链表库, 具体实现方法参考了[LinkedList](https://github.com/ivanseidel/LinkedList), 内部使用`malloc`函数实现链表节点创建,可在任意节点上添加或删除内容,在添加节点时请注意堆栈的使用情况.

---



## 安装

1. 从`gitHub`[下载]()这个库的最新版本.
2. 解压后将文件夹`simple_list`添加到你的项目当中.



## 开始

如果你不知道这个库的用途请查阅`链表`相关内容.



- 声明链表对象并初始化它
```
// 声明一个名叫 mylist 的链表
simpleListDef(mylist); //!< 声明
simpleListInit(mylist); //!< 初始化


// 或者直接使用结构体直接声明
List_t list = {...};
ListMethod_t method = {...};
```



- 注意,  使用宏声明链表, 需要使用指针来指向这个链表
```
// 用指针获取链表
List_t *list = simpleList(mylist);
// 用指针获取操作链表的方法
ListMethod_t *method = simpleListMethod(mylist);
```



- 获取链表大小
```
// 使用 size() 方法获取大小
int size = method.size(list);

// 注意, 如果是指向链表的指针,你应该使用
int size = method->size(list);
```



- 添加元素
```
// add() 方法将在链表尾部添加元素
method->add(list, myObject);

// unshift() 方法将在链表头部添加元素
method->unshift(list, myObject);
```



- 获取元素
```
// 获取第一个元素
method->get(list, 0);

// 获取最后一个元素
method->get(list, method->size(list) - 1);
```



- 改变元素
```
// Change the first element to myObject
method->set(list, 0, myObject);
```



- 插入元素
```
// 将元素插入到位置1处，（链表中已经存在两个及以上元素）
method->insert(list, 1, myObject);
```



- 弹出元素

```
// 弹出第一个元素
method->unshift(list);

// 弹出最后一个元素
method->pop(list);
```



- 删除元素
```
// 删除第一个元素
method->remove(list, 0);

// 删除最后一个元素
method->remove(list, method->size(list) - 1);
```



## 历史版本

- `1.1 (2023-06-18)`：添加`insert()`方法

- `1.0 (2013-06-17)`: 原始版本