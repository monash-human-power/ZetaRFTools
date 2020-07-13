#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include <ZetaRF.h>

constexpr size_t ZetaRFPacketLength {8};
constexpr uint8_t ZetaRFChannel {4};

template <typename Config>
class ZetaTestHelper
{
public:
    ZetaTestHelper()
    {
        std::cout << "Starting Zeta TxRx..." << std::endl;

        if (!this->m_zeta.beginWithPacketLengthOf(ZetaRFPacketLength))
            throw "ZetaRF begin failed. Check wiring?";

        if (!this->m_zeta.startListeningSinglePacketOnChannel(ZetaRFChannel))
            throw "ZetaRF startListening failed.";

        std::cout << "Init done." << std::endl;
    }

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

private:
    ZetaRFConfig<Config, ZetaRFEZRadio::EZRadioSi4455<SpiHal<ZetaRF::nSEL<6>, ZetaRF::SDN<9>, ZetaRF::nIRQ<8>>>> m_zeta;
};