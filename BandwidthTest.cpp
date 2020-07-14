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
    BandwidthTest(size_t packetLength) : ZetaTestBase<Config>(packetLength) {}

    void measureBandwidth(int packets)
    {
        using namespace std::chrono;

        auto data = generateRandomPacket(this->m_packetLength);

        auto start_time = steady_clock::now();

        for (int packet_num = 0; packet_num < packets; packet_num++)
        {
            if (this->transmitPacket(data))
                this->waitForPacketTransmitted();
            else
                std::cout << "Failed to send packet" << std::endl;
        }
        
        auto end_time = steady_clock::now();

        duration<double, std::milli> duration_ms = end_time - start_time;
        auto dataRate = (float) this->m_packetLength * packets * 8 / (duration_ms.count());

        std::cout << "Sent " << packets << " packets in " << duration_ms.count() << " ms"
            << " (" << duration_ms.count() / packets << " ms/packet)"
            << " at " << dataRate << " kbps" << std::endl;
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

    bool transmitPacket(std::vector<uint8_t> data)
    {
        if (this->m_zeta.requestBytesAvailableInTxFifo() < this->m_packetLength)
        {
            std::cout << "Not enough space in TX FIFO" << std::endl;
            return false;
        }
        return this->m_zeta.sendFixedLengthPacketOnChannel(4, &data[0]);
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
    BandwidthTest<ZetaRFConfigs::Config433_FixedLength_CRC_Preamble10_Sync4_Payload8> bandwidthTest(64);
    bandwidthTest.measureBandwidth(5);
}