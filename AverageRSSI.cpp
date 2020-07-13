#include "ZetaTestHelper.hpp"

int main()
{
    ZetaTestHelper<ZetaRFConfigs::Config433_FixedLength_CRC_Preamble10_Sync4_Payload8> zetaTest;
    zetaTest.measureRssi(30, 30*4);
}