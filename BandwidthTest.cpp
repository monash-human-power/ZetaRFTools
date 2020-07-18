#include <chrono>
#include <thread>

#include "ZetaTest.hpp"

template <typename Config>
class BandwidthTest : TransmitterBase<Config>
{
public:
    BandwidthTest(size_t packetLength) : TransmitterBase<Config>(packetLength) {}

    void measureBandwidth(int packets)
    {
        using namespace std::chrono;

        auto data = this->generateRandomPacket(this->m_packetLength);

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
};

int main()
{
    BandwidthTest<ZetaRFConfigs::Config433_FixedLength_CRC_Preamble10_Sync4_Payload8> bandwidthTest(64);
    bandwidthTest.measureBandwidth(5);
}
