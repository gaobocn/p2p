#include <unistd.h>

#include <QDebug>

#include "netsocket.hh"

NetSocket::NetSocket()
{
    // Pick a range of four UDP ports to try to allocate by default,
    // computed based on my Unix user ID.
    // This makes it trivial for up to four P2Papp instances per user
    // to find each other on the same host,
    // barring UDP port conflicts with other applications
    // (which are quite possible).
    // We use the range from 32768 to 49151 for this purpose.
    myPortMin = 32768 + (getuid() % 4096)*4;
    myPortMax = myPortMin + 3;
}

bool NetSocket::bind()
{
    // Try to bind to each of the range myPortMin..myPortMax in turn.
    for (quint16 p = myPortMin; p <= myPortMax; p++) {
        if (QUdpSocket::bind(p)) {
            this->myPortNum = p;
            qDebug() << "bound to UDP port " << p;
            return true;
        }
    }

    qDebug() << "Oops, no ports in my default range " << myPortMin
        << "-" << myPortMax << " available";
    return false;
}

quint16 NetSocket::pickNeighbor() {
    if (myPortNum == myPortMax) return myPortNum - 1;
    if (myPortNum == myPortMin) return myPortNum + 1;
    return (qrand() % 2 ? myPortNum - 1 : myPortNum + 1);
}
