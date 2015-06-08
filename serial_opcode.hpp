//
// Created by manuel on 04.06.15.
//

#ifndef MEDIATOR_SERIAL_OPCODE_H
#define MEDIATOR_SERIAL_OPCODE_H

enum class serial_opcode
{
    SERIAL_READY = 0x00,
    RESET = 0x01,
    PING = 0x02,
    PONG = 0x03,
    BUZZ = 0x04,
    SET_COLOR = 0x05,
    CONNECT = 0x06,
    DISCONNECT = 0x07
};

#endif //MEDIATOR_SERIAL_OPCODE_H
