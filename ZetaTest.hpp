#include <iostream>

#include <ZetaRF.h>

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