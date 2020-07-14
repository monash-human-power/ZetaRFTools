#include <chrono>
#include <iomanip>
#include <random>
#include <thread>
#include <vector>

#include "ZetaTest.hpp"
#include "configs/si4455_revb1_bidir_fixed_crc_pre10_sync4_pay8_433mhz_20kbps.h"

template <typename Config>
class BandwidthTest : ZetaTestBase<Config>
{
public:
    void measureBandwidth(int packets)
    {
        using namespace std::chrono;

        auto data = generateRandomPacket(ZetaRFPacketLength);

        auto start_time = steady_clock::now();

        for (int packet_num = 0; packet_num < packets; packet_num++)
        {
            this->transmitPacket(data);
            this->waitForPacketTransmitted();
        }
        
        auto end_time = steady_clock::now();

        duration<double, std::milli> duration = end_time - start_time;
        seconds sec(1);
        auto dataRate = (float) ZetaRFPacketLength * packets * 8 / (duration.count() * 1e-3);

        std::cout << "Sent " << packets << " packets in " << duration.count() << " ms (" << duration.count() / packets << " ms/packet) at " << dataRate << " kbps" << std::endl;
    }

private:
    std::vector<uint8_t> generateRandomPacket(size_t size)
    {
        std::independent_bits_engine<std::default_random_engine, 8, uint8_t> byte_engine;
        std::vector<uint8_t> data(size);
        std::generate(data.begin(), data.end(), byte_engine);

        std::cout << "Generated " << size << " byte packet: " << std::hex;
        for (auto &&byte : data)
            std::cout << std::setfill('0') << std::setw(2) << (int) byte << " ";
        std::cout << std::dec << std::endl;

        return data;
    }

    void transmitPacket(std::vector<uint8_t> data)
    {
        if (this->m_zeta.requestBytesAvailableInTxFifo() < ZetaRFPacketLength)
        {
            std::cout << "Not enough space in TX FIFO" << std::endl;
            return;
        }
        this->m_zeta.sendFixedLengthPacketOnChannel(4, &data[0]);
    }

    void waitForPacketTransmitted()
    {
        while (true)
            if (ZetaRF::Events const ev = this->m_zeta.checkForEvent())
                if (ev & ZetaRF::Event::PacketTransmitted)
                    return;
    }
};

int main()
{
    BandwidthTest<ZetaRFConfigs::Config433_FixedLength_CRC_Preamble10_Sync4_Payload8> bandwidthTest;
    bandwidthTest.measureBandwidth(5);
}