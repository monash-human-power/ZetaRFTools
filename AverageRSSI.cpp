#include <algorithm>
#include <chrono>
#include <thread>
#include <vector>

#include "ZetaTest.hpp"

template <typename Config>
class AverageRSSITest : ZetaTestBase<Config>
{
public:
    void measureRssi(int testDuration, int expectedPackets=-1)
    {
        using namespace std::chrono;
        using namespace std::chrono_literals;

        // Expect 1 packet/s by default
        if (expectedPackets == -1)
            expectedPackets = testDuration;

        std::vector<uint8_t> receivedRssi;

        const auto start_time = steady_clock::now();
        const auto end_time = start_time + seconds(testDuration);

        while (steady_clock::now() < end_time)
        {
            if (const auto ev = this->m_zeta.checkForEvent())
            {
                if (ev & ZetaRF::Event::PacketReceived)
                {
                    receivedRssi.push_back(this->m_zeta.latchedRssiValue());
                    this->m_zeta.restartListeningSinglePacket();
                    std::cout << "+" << std::flush;
                }
            }

            std::this_thread::sleep_for(10ms);
        }

        std::cout << std::endl;

        const auto count = receivedRssi.size();
        const auto droppedPercent = (1 - (float) count / expectedPackets) * 100;

        std::cout << "Received " << count << "/" << expectedPackets;
        std::cout << " (" << droppedPercent << "% dropped)" << std::endl;

        if (count == 0) return;

        std::sort(receivedRssi.begin(), receivedRssi.end());
        std::cout << "Min:    " << (int) receivedRssi[0] << std::endl;
        std::cout << "Median: " << (int) receivedRssi[count / 2] << std::endl;
        std::cout << "Max:    " << (int) receivedRssi[count - 1] << std::endl;
    }
};

int main()
{
    AverageRSSITest<ZetaRFConfigs::Config433_FixedLength_CRC_Preamble10_Sync4_Payload8> rssiTest;
    rssiTest.measureRssi(30, 30*4);
}