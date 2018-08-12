#!/bin/bash
#1.直接使用test命令
#2.使用[  ]将测试条件括住

n=0

while [ $n -lt 5 ]
do 
    n=`expr $n + 1`
    echo $n
done
