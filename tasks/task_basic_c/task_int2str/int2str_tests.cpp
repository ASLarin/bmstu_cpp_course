#include <gtest/gtest.h>
#include "int2str.h"

    EXPECT_STREQ(int2str(0), "0");
}

    EXPECT_STREQ(int2str(0), "0");
}

    EXPECT_STREQ(int2str(-0), "0");
}

    EXPECT_STREQ(int2str(-5), "-5");
    EXPECT_STREQ(int2str(-66), "-66");
    EXPECT_STREQ(int2str(-123), "-123");
    EXPECT_STREQ(int2str(2147483647), "2147483647");
    EXPECT_STREQ(int2str(-2147483648), "-2147483648");
}
