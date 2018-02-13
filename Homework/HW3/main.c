/*
 * main.c for homework 3
 * ECE 266, Spring 2018
 * Created by Zhao Zhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "homework3.h"

int main(void)
{
    char msg[40] = "Assembly programming is scaring.";
    char msg2[40] = "Assembly programming is fun.";

    lpInit();

    // Test sub2()
    uprintf("\n\r*** Test sub2() ***\n\r");
    uprintf("sub2(%d, %d) = %d\n\r", 1, 2, sub2(1, 2));
    uprintf("sub2(%d, %d) = %d\n\r", 10, -5, sub2(10, -5));
    uprintf("sub2(%d, %d) = %d\n\r", -3, 0, sub2(-3, 0));

    // Test sub3()
    uprintf("\n\r*** Test sub3() ***\n\r");
    uprintf("sub3(%d, %d, %d) = %d\n\r", 1, 2, 3, sub3(1, 2, 3));
    uprintf("sub3(%d, %d, %d) = %d\n\r", -1, 2, 3, sub3(-1, 2, 3));
    uprintf("sub3(%d, %d, %d) = %d\n\r", 0, 10, -5, sub3(0, 10, -5));
    uprintf("sub3(%d, %d, %d) = %d\n\r", -5, -10, -15, sub3(-5, -10, -15));

    // Test strcpy()
    uprintf("\n\r*** Test strcpy() ***\n\r");
    uprintf("Old msg: %s\n\r", msg);
    mystrcpy(msg, msg2);
    uprintf("Now msg: %s\n\r", msg);

    // Test calc()
    uprintf("\n\r*** Test calc() ***\n\r");
    uprintf("calc(%d, %d, %d) = %d\n\r", 1, 2, 3, calc(1, 2, 3));
    uprintf("calc(%d, %d, %d) = %d\n\r", -1, 2, 3, calc(-1, 2, 3));
    uprintf("calc(%d, %d, %d) = %d\n\r", 0, 10, -5, calc(0, 10, -5));
    uprintf("calc(%d, %d, %d) = %d\n\r", -5, -10, -15, calc(-5, -10, -15));

    // Never exit
    while (true) {
    }
}

