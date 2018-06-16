#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pt.h"

char run_a = 0;
char run_b = 0;

/*
char task_a(struct pt *pt)
{
 { char PT_YIELD_FLAG = 1; switch((pt)->lc) { case 0:;
 while(1) {
  do { (pt)->lc = 10; case 10:; if(!(run_a)) { return 0; } } while(0);
  run_a = 0;
  run_b = 1;
  printf("%s\n", __FUNCTION__);
 }
 }; PT_YIELD_FLAG = 0; (pt)->lc = 0;; return 2; };
}

char task_b(struct pt *pt)
{
 { char PT_YIELD_FLAG = 1; switch((pt)->lc) { case 0:;
 while(1){
  run_a = 1;
  do { (pt)->lc = 23; case 23:; if(!(run_b)) { return 0; } } while(0);
  run_b = 0;
  printf("%s\n", __FUNCTION__);
 }
 }; PT_YIELD_FLAG = 0; (pt)->lc = 0;; return 2; };
}
*/

/*
好处：
1.子线程如果在程序某处条件不满足会阻塞退出线程，在下次得到调度的时候能直接跳转到上次阻塞的地方继续执行。
如果是普通的函数调用，则会从函数开始依次重新执行，这显然不是我们想要的。
*/

PT_THREAD(task_a(struct pt *pt))
{
    PT_BEGIN(pt);
    while(1)
    {
        PT_WAIT_UNTIL(pt, run_a);
        run_a = 0;
        run_b = 1;
        printf("%s\n", __FUNCTION__);
    }
    PT_END(pt);
}

PT_THREAD(task_b(struct pt *pt))
{
    PT_BEGIN(pt);
    while(1)
    {
        run_a = 1;
        PT_WAIT_UNTIL(pt, run_b);
        run_b = 0;
        printf("%s\n", __FUNCTION__);
    }
    PT_END(pt);
}

int main()
{
    struct pt pt_a;
    struct pt pt_b;

    PT_INIT(&pt_a);
    PT_INIT(&pt_b);
    while(1) {
        PT_SCHEDULE(task_a(&pt_a));
        PT_SCHEDULE(task_b(&pt_b));
        sleep(1);
    }
    return 0;
}  
