#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <algorithm>

#include "CommonDefs.h"
#include "BookConstructor.hpp"

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cerr << "Usage:" << argv[0] << "FileName" << std::endl;
        return 1;
    }

    BookConstructor bookConstructor(argv[1]);
    bookConstructor.start();
    return 0;

}
