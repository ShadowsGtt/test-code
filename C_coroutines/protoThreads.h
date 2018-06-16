#ifndef _PROTOTHREADS_H
#define _PROTOTHREADS_H

#define PT_WAITING 0
#define PT_YIELDED 1
#define PT_EXITED  2
#define PT_ENDED   3

typedef unsigned int INT16U;

typedef void * lc_t;
#define LC_INIT(s) s = NULL
#define LC_RESUME(s) \
    do { \
        if (s != NULL) { \
            goto *s; \
        }\
    }while(0)
#define LC_CONCAT2(s1, s2) s1##s2
#define LC_CONCAT(s1, s2) LC_CONCAT2(s1, s2)
#define LC_SET(s) \
    do { \
        LC_CONCAT(LC_LABEL, __LINE__): \
        (s) = &&LC_CONCAT(LC_LABEL, __LINE__); \
    } while (0)

struct pt
{
  INT16U lc;
};
/* 初始化一个协程，也即初始化状态变量 */
#define PT_INIT(pt) LC_INIT((pt)->lc)

/* 声明一个函数，返回值为 char 即退出码，表示函数体内使用了 proto thread，（个人觉得有些多此一举） */
#define PT_THREAD(name_args) char name_args

/* 协程入口点， PT_YIELD_FLAG=0表示出让，=1表示不出让，放在 switch 语句前面，下次调用的时候可以跳转到上次出让点继续执行 */
#define PT_BEGIN(pt) { char PT_YIELD_FLAG = 1; LC_RESUME((pt)->lc)

/* 协程退出点，至此一个协程算是终止了，清空所有上下文和标志 */
#define PT_END(pt) LC_END((pt)->lc); PT_YIELD_FLAG = 0; \
                   PT_INIT(pt); return PT_ENDED; }

/* 协程出让点，如果此时协程状态变量 lc 已经变为 __LINE__ 跳转过来的，那么 PT_YIELD_FLAG = 1，表示从出让点继续执行。 */
#define PT_YIELD(pt)        \
  do {            \
    PT_YIELD_FLAG = 0;        \
    LC_SET((pt)->lc);       \
    if(PT_YIELD_FLAG == 0) {      \
      return PT_YIELDED;      \
    }           \
  } while(0)

/* 附加出让条件 */
#define PT_YIELD_UNTIL(pt, cond)    \
  do {            \
    PT_YIELD_FLAG = 0;        \
    LC_SET((pt)->lc);       \
    if((PT_YIELD_FLAG == 0) || !(cond)) { \
      return PT_YIELDED;      \
    }           \
  } while(0)

/* 协程阻塞点(blocking),本质上等同于 PT_YIELD_UNTIL，只不过退出码是 PT_WAITING，用来模拟信号量同步 */
#define PT_WAIT_UNTIL(pt, condition)          \
  do {            \
    LC_SET((pt)->lc);       \
    if(!(condition)) {        \
      return PT_WAITING;      \
    }           \
  } while(0)

/* 同 PT_WAIT_UNTIL 条件反转 */
#define PT_WAIT_WHILE(pt, cond)  PT_WAIT_UNTIL((pt), !(cond))

/* 协程调度，调用协程 f 并检查它的退出码，直到协程终止返回 0，否则返回 1。 */
#define PT_SCHEDULE(f) ((f) < PT_EXITED)

/* 这用于非对称协程，调用者是主协程，pt 是和子协程 thread （可以是多个）关联的上下文句柄，主协程阻塞自己调度子协程，直到所有子协程终止 */
#define PT_WAIT_THREAD(pt, thread) PT_WAIT_WHILE((pt), PT_SCHEDULE(thread))

/* 用于协程嵌套调度，child 是子协程的上下文句柄 */
#define PT_SPAWN(pt, child, thread)   \
  do {            \
    PT_INIT((child));       \
    PT_WAIT_THREAD((pt), (thread));   \
  } while(0)

struct pt_sem {
  unsigned int count;
};

#define PT_SEM_INIT(s, c) (s)->count = c

#define PT_SEM_WAIT(pt, s)  \
  do {            \
    PT_WAIT_UNTIL(pt, (s)->count > 0);    \
    --(s)->count;       \
  } while(0)

#define PT_SEM_SIGNAL(pt, s) ++(s)->count

#endif
