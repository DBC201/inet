#include "inet/common/ProtocolTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "UdpEncapsulatorApp.h"

namespace inet {

Define_Module(UdpEncapsulatorApp);

void UdpEncapsulatorApp::initialize(int stage) {
    ApplicationBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        source_port = par("source_port");
        destination_port = par("destination_port");
        encapsulate = par("encapsulate");
        decapsulate = par("decapsulate");
    }
}

void UdpEncapsulatorApp::handleStartOperation(LifecycleOperation *operation) {
    socket.setOutputGate(gate("socketOut"));
    const char *source_address = par("source_address");
    socket.bind(*source_address ? L3AddressResolver().resolve(source_address) : L3Address(), source_port);

    const char *destination_address = par("destination_address");
    L3AddressResolver().tryResolve(destination_address, this->destination_address);

    if (this->destination_address.isUnspecified())
        throw cRuntimeError("cannot resolve destination address.");
    socket.setCallback(this);
}

void UdpEncapsulatorApp::handleStopOperation(LifecycleOperation *operation) {
    socket.close();
}

void UdpEncapsulatorApp::handleCrashOperation(LifecycleOperation *operation) {
    socket.destroy();
}

void UdpEncapsulatorApp::handleMessageWhenUp(cMessage* message) {
    if (socket.belongsToSocket(message)) {
        socket.processMessage(message);
        return;
    }

    if (!encapsulate) {
        EV_INFO << "Received packet from ethernetIn, discarding..." << endl;
        return;
    }

    // if packet is received from ethernetIn, encapsulate it
    Packet *incoming = check_and_cast<Packet *>(message);
    if (incoming->getTag<PacketProtocolTag>()->getProtocol() != &Protocol::ethernetMac)
            throw cRuntimeError("Unaccepted packet protocol specified on upper layer incoming packet");
    auto payload = incoming->removeData();
    payload->addTag<PacketProtocolTag>()->setProtocol(&Protocol::ethernetMac);
    std::string name(incoming->getName());
    delete incoming;
    Packet *outgoing = new Packet(name.c_str(), payload);
    socket.sendTo(outgoing, destination_address, destination_port);
}

void UdpEncapsulatorApp::socketDataArrived(UdpSocket *socket, Packet *packet) {
    // decapsulate and write to ethernetOut

    std::string name(packet->getName());

    if (!decapsulate) {
        EV_INFO << "Discarding " << name << endl;
        delete packet;
        return;
    }

    auto payload = packet->removeData();
    auto protocol = payload->findTag<PacketProtocolTag>();
    delete packet;

    if (protocol == nullptr || protocol->getProtocol() != &Protocol::ethernetMac) {
        EV_INFO << "Discarding " << name << endl;
        return;
    }

//    payload->removeTag<PacketProtocolTag>();

    auto decapsulated_packet = new Packet(name.c_str(), payload);
    decapsulated_packet->addTag<PacketProtocolTag>()->setProtocol(&Protocol::ethernetMac);
    send(decapsulated_packet, "ethernetOut");
}

void UdpEncapsulatorApp::socketErrorArrived(UdpSocket *socket, Indication *indication) {
    EV_ERROR << "Socket error arrived: " << indication->getName() << endl;
    delete indication;
}

void UdpEncapsulatorApp::socketClosed(UdpSocket *socket) {
    // do nothing
}

} // namespace inet

