#include "lib/LogStat.h"

#include <gtest/gtest.h>

TEST(LOG_STAT_TEST, SAMPLE_TEST) {
    ohtuzh::LogStat stat;
    stat.CollectStat("../../test/test_directory");
    const std::string result =
            "Process   \tTrace\tDebug\tInfo\tWarn\tError\n"
            "calculator\t2\t1\t1\t1\t1\n"
            "browser   \t0\t1\t1\t1\t1";
    ASSERT_EQ(stat.GetSortedStatTable(), result);
}
