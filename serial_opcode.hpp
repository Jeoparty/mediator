//
// Created by manuel on 04.06.15.
//

#ifndef MEDIATOR_SERIAL_OPCODE_H
#define MEDIATOR_SERIAL_OPCODE_H

enum class serial_opcode
{
    SERIAL_READY = 0x00,
    PING = 0x01,
    PONG = 0x02,
    BUZZ = 0x03,
    SET_COLOR = 0x04,
    CONNECT = 0x05,
    DISCONNECT = 0x06
};

#endif //MEDIATOR_SERIAL_OPCODE_H
