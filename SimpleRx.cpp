#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

#include "ZetaTest.hpp"

/* Derived from the SimpleRx example of the ZetaRF library.
   Much of this code is not original. */

template <typename Config>
class SimpleRx : ZetaTestBase<Config>
{
public:
    SimpleRx(size_t packetLength) : ZetaTestBase<Config>(packetLength) {}

    void listen()
    {
        while (true)
        {
            if (ZetaRF::Events const ev = this->m_zeta.checkForEvent())
                this->processEvent(ev);

            if (this->m_zeta.available())
                this->readPacket();

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

private:
    void processEvent(ZetaRF::Events const ev)
    {
        if (ev & ZetaRF::Event::DeviceBusy)
        {
            // DeviceBusy error usually means the radio module is unresponding and need a reset.
            std::cout << "Error: Device Busy! Restarting..." << std::endl;

            if (!this->m_zeta.beginWithPacketLengthOf(this->m_packetLength))
            {
                std::cout << "ZetaRF begin failed after comm error." << std::endl;
                while (true) ;
            }
            this->m_zeta.restartListeningSinglePacket();
        }
        if (ev & ZetaRF::Event::PacketReceived)
        {
            // We'll read data later
            // Get RSSI (only valid in single packet RX, before going back to RX)
            uint8_t const rssi = this->m_zeta.latchedRssiValue();

            // Restart listening on the same channel
            this->m_zeta.restartListeningSinglePacket();

            std::cout << "Packet received with RSSI: " << (int)rssi << std::endl;
        }
    }

    void readPacket()
    {
        char data[this->m_packetLength] = "Hello world!";

        if (this->m_zeta.readPacketTo((uint8_t *)data))
        {
            // Print!
            std::cout << "RX  >" << data << "<" << std::endl;
            // Print in HEX
            std::cout << "HEX >" << std::hex;
            for (uint8_t i = 0; i < this->m_zeta.packetLength(); i++)
                std::cout << std::setfill('0') << std::setw(2) << (int)data[i];
            std::cout << std::dec << "<" << std::endl;
        }
    }
};

int main()
{
    SimpleRx<ZetaRFConfigs::Config433_FixedLength_CRC_Preamble10_Sync4_Payload8> rx(64);
    rx.listen();
}
