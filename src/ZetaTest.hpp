/**
 * Base classes to handle the boilerplate work for the tools.
 */

#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

#include <ZetaRf.hpp>

#include "configs/si4455_revb1_bidir_fixed_crc_pre10_sync4_pay8_433mhz_20kbps.hpp"
#include "configs/si4455_revb1_bidir_fixed_crc_pre10_sync4_pay8_433mhz_128kbps.hpp"

constexpr uint8_t ZetaRfChannel {4};

/**
 * Simple base class providing initialisation of the Zeta module.
 */
template <typename Config>
class ZetaTestBase
{
public:
    /**
     * Initialises the ZetaRF module represented by m_zeta.
     *
     * @param packetLength The size in bytes of packets to be sent and
     *                     received. Packets not equal to this size will not be
     *                     received. Must be less than or equal to 64.
     */
    ZetaTestBase(const size_t packetLength)
        : m_packetLength(packetLength)
    {
        std::cout << "Starting Zeta TxRx..." << std::endl;

        if (!this->m_zeta.beginWithPacketLengthOf(this->m_packetLength))
            throw "ZetaRf begin failed. Check wiring?";

        if (!this->m_zeta.startListeningSinglePacketOnChannel(ZetaRfChannel))
            throw "ZetaRf startListening failed.";

        std::cout << "Init done." << std::endl;
    }

protected:
    ZetaRfConfig<Config, ZetaRfEZRadio::EZRadioSi4455<SpiHal<ZetaRf::nSEL<6>, ZetaRf::SDN<9>, ZetaRf::nIRQ<8>>>> m_zeta;
    const size_t m_packetLength;
};

/**
 * Base class with helper methods for generating and transmitting packets.
 */
template <typename Config>
class TransmitterBase : public ZetaTestBase<Config>
{
public:
    TransmitterBase(const size_t packetLength) : ZetaTestBase<Config>(packetLength) {}

protected:
    /**
     * Generates a sequence of "random" bytes. In practice, the same packet is
     * generated every time... But it's the thought that counts.
     * https://xkcd.com/221/
     *
     * @param size The size of the random packet to generate, in bytes.
     * @return The random packet, as a vector of size size.
     */
    std::vector<uint8_t> generateRandomPacket(const size_t size) const
    {
        const std::independent_bits_engine<std::default_random_engine, 8, uint8_t> byte_engine;
        std::vector<uint8_t> data(size);
        std::generate(data.begin(), data.end(), byte_engine);

        std::cout << "Generated " << size << " byte packet: " << std::hex;
        for (auto &&byte : data)
            std::cout << std::setfill('0') << std::setw(2) << (int) byte << " ";
        std::cout << std::dec << std::endl;

        return data;
    }

    /**
     * Transmits a packet by radio. The packet should be of size
     * m_packetLength.
     *
     * @param data The packet to be sent.
     * @return true if the packet was sent successfully, else false
     */
    bool transmitPacket(const std::vector<uint8_t> data)
    {
        if (this->m_zeta.requestBytesAvailableInTxFifo() < this->m_packetLength)
        {
            std::cout << "Not enough space in TX FIFO" << std::endl;
            return false;
        }
        return this->m_zeta.sendFixedLengthPacketOnChannel(4, &data[0]);
    }

    /**
     * Blocks and eventually returns once the packet is transmitted.
     */
    void waitForPacketTransmitted()
    {
        while (true)
            if (ZetaRf::Events const ev = this->m_zeta.checkForEvent())
                if (ev & ZetaRf::Event::PacketTransmitted)
                    return;
    }
};
