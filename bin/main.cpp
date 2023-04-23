#include <iostream>

#include "lib/LogStat.h"

int main() {
    ohtuzh::LogStat stat;
    stat.CollectStat("../../test/test_directory");
    std::cout << stat.GetSortedStatTable() << std::endl;
    return 0;
}
