#pragma once
#include <stdio.h>
class Person
{
public:
    virtual void display() = 0;
    virtual void call() = 0;
    void nothing();
};