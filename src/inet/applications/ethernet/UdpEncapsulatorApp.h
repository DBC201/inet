#ifndef __INET_UDPENCAPSULATORAPP_H
#define __INET_UDPENCAPSULATORAPP_H

#include "inet/applications/base/ApplicationBase.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"

namespace inet {

class INET_API UdpEncapsulatorApp: public ApplicationBase, public UdpSocket::ICallback
{
protected:
    UdpSocket socket;
    L3Address destination_address;
    int source_port = -1;
    int destination_port = -1;
    bool encapsulate = true;
    bool decapsulate = true;

    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;

    virtual void handleMessageWhenUp(cMessage *msg) override;

    virtual void handleStartOperation(LifecycleOperation *operation) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
    virtual void handleCrashOperation(LifecycleOperation *operation) override;

    virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;
    virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
    virtual void socketClosed(UdpSocket *socket) override;
};

} // namespace inet

#endif

