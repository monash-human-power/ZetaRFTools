#include <iostream>

#include <ZetaRF.h>

constexpr size_t ZetaRFPacketLength {8};
constexpr uint8_t ZetaRFChannel {4};

template <typename Config>
class ZetaTestBase
{
public:
    ZetaTestBase()
    {
        std::cout << "Starting Zeta TxRx..." << std::endl;

        if (!this->m_zeta.beginWithPacketLengthOf(ZetaRFPacketLength))
            throw "ZetaRF begin failed. Check wiring?";

        if (!this->m_zeta.startListeningSinglePacketOnChannel(ZetaRFChannel))
            throw "ZetaRF startListening failed.";

        std::cout << "Init done." << std::endl;
    }

protected:
    ZetaRFConfig<Config, ZetaRFEZRadio::EZRadioSi4455<SpiHal<ZetaRF::nSEL<6>, ZetaRF::SDN<9>, ZetaRF::nIRQ<8>>>> m_zeta;
};