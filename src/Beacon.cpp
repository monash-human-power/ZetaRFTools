/**
 * Sends a packet repeatedly at a given interval.
 */

#include <chrono>
#include <thread>

#include "ZetaTest.hpp"

template <typename Config>
class Beacon : TransmitterBase<Config>
{
public:
    Beacon(size_t packetLength) : TransmitterBase<Config>(packetLength) {}

    void begin(const std::chrono::milliseconds interval)
    {
        using namespace std::chrono;

        auto const data = this->generateRandomPacket(this->m_packetLength);

        auto next_packet_time = steady_clock::now();

        while (true)
        {
            std::this_thread::sleep_until(next_packet_time);

            if (this->transmitPacket(data))
            {
                this->waitForPacketTransmitted();
                std::cout << "+" << std::flush;
            } else
                std::cout << "Failed to send packet" << std::endl;

            next_packet_time = max(steady_clock::now(), next_packet_time + interval);
        }
    }
};

int main()
{
    Beacon<ZetaRfConfigs::Config433_FixedLength_CRC_Preamble10_Sync4_Payload8> beacon(64);
    beacon.begin(std::chrono::milliseconds(1000));
}
