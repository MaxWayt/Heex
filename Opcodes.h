/*
** Opcodes.h for Heex
*/

#ifndef OPCODES_H_
# define OPCODES_H_

// MMSG -> Map Generator MSG
// SMSG -> Server MSG
// CMSG -> Client MSG

enum Opcodes
{
    MSG_TEXT            = 0x0001,
    SMSG_HASK_MAP       = 0x0002,
    MMSG_MAP_RESPONSE   = 0x0003,
    MMSG_SEND_OBJECT    = 0x0004,
    MMSG_PROGRESS       = 0x0005,

};

#endif /* !OPCODES_H_ */
