#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

#include <ZetaRF.h>

#include "configs/si4455_revb1_bidir_fixed_crc_pre10_sync4_pay8_433mhz_20kbps.h"
#include "configs/si4455_revb1_bidir_fixed_crc_pre10_sync4_pay8_433mhz_128kbps.h"

constexpr uint8_t ZetaRFChannel {4};

template <typename Config>
class ZetaTestBase
{
public:
    ZetaTestBase(size_t packetLength)
        : m_packetLength(packetLength)
    {
        std::cout << "Starting Zeta TxRx..." << std::endl;

        if (!this->m_zeta.beginWithPacketLengthOf(this->m_packetLength))
            throw "ZetaRF begin failed. Check wiring?";

        if (!this->m_zeta.startListeningSinglePacketOnChannel(ZetaRFChannel))
            throw "ZetaRF startListening failed.";

        std::cout << "Init done." << std::endl;
    }

protected:
    ZetaRFConfig<Config, ZetaRFEZRadio::EZRadioSi4455<SpiHal<ZetaRF::nSEL<6>, ZetaRF::SDN<9>, ZetaRF::nIRQ<8>>>> m_zeta;
    size_t m_packetLength;
};

template <typename Config>
class TransmitterBase : public ZetaTestBase<Config>
{
public:
    TransmitterBase(size_t packetLength) : ZetaTestBase<Config>(packetLength) {}

protected:
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
