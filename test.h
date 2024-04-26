#pragma once
#include <iostream>

#include "HLAengine.h"

class test : public Component {
    DECLARE_CLASS

public:
    test() {}
    ~test() {}
    static void classInit();
    virtual void init(){};
    virtual void tick() override;
    virtual void simulation() override;

public:
    int index;
    vector<int> v;
    int count;
};
