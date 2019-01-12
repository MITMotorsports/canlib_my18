'''
Generate Can_Libary.c file.
Run this file (with the spec path as a command line argument) to write just
Can_Libary.c or main.py to write all files.
'''
import sys
sys.path.append('ParseCAN')
import ParseCAN
from ParseCAN.spec import Endianness, Type

from math import ceil, floor, log2
from common import can_lib_c_path, can_lib_c_base_path, coord


def swap_endianness_fn(type: Type):
    if type.isbool():
        return ''

    return 'swap_' + type.type


def write(can, output_path=can_lib_c_path, base_path=can_lib_c_base_path):
    '''
    Generate Can_Libary.c file.

    :param output_path: file to be written to
    :param can: CAN spec
    :param base_path: File with template code that's not autogenerated
    '''
    with open(output_path, 'w') as f:
        fw = f.write

        fw('#include "CANlib.h"\n')

        # Copy over base
        with open(base_path) as base:
            lines = base.readlines()
            f.writelines(lines)

        fw('\n')

        # Write init functions
<<<<<<< HEAD
        if False:  # TODO: Restore
            for board in can.board:
                if board.arch:  # Means it's a board we program
                    for bus in board.subscribe:
                        fw('CANlib_Init_Error_T CANlib_Init_{}(void) '.format(coord(bus.name, board.name, prefix=False)) + '{' '\n')
                        fw('\t' 'CANlib_Init({});\n'.format(coord(bus.baudrate, prefix=False)))

                        max_id = max(msg.id for msg in bus.frame)

                        # Find mask
                        # The way hardware filtering works is that incoming IDs are
                        # ANDed with the mask and then compared with a preset ID
                        # The goal of this mask is to throw away most higher ID (i.e.,
                        # lower priority) frame
                        mask = 2**(floor(log2(max_id)) + 1) - 1
                        # On a standard bus, IDs are 11 bit
                        max_possible_id = 2**11-1
                        if bus.is_extended:
                            # On an extended bus, IDs are 29 bit
                            max_possible_id = 2**29-1
                        mask = mask ^ max_possible_id

                        # Set mask (pass in binary to make file more readable)
                        fw('\t' 'CANlib_SetFilter(' + hex(mask) + ', 0);' '\n')

                        # Finish up
                        fw('}' '\n\n')

                # We still need to create init functions for board that publish
=======
        for board in car.boards:
            if board.architecture:  # Means it's a board we program
                for bus in board.subscribe:
                    fw('CANlib_Init_Error_T CANlib_Init_{}(void) '.format(coord(bus.name, board.name, prefix=False)) + '{' '\n')
                    fw('\t' 'CANlib_Init({});\n'.format(coord(bus.baudrate, prefix=False)))

                    max_id = max(msg.id for msg in bus.frames)

                    # Find mask
                    # The way hardware filtering works is that incoming IDs are
                    # ANDed with the mask and then compared with a preset ID
                    # The goal of this mask is to throw away most higher ID (i.e.,
                    # lower priority) frames
                    mask = 2**(floor(log2(max_id)) + 1) - 1
                    # On a standard bus, IDs are 11 bit
                    max_possible_id = 2**11-1
                    if bus.is_extended:
                        # On an extended bus, IDs are 29 bit
                        max_possible_id = 2**29-1
                    mask = mask ^ max_possible_id

                    # Set mask (pass in binary to make file more readable)
                    fw('\t' 'CANlib_SetFilter(' + hex(mask) + ', 0);' '\n')

                    # Finish up
                    fw('}' '\n\n')

                # We still need to create init functions for boards that publish
>>>>>>> my19-integration
                # on a bus but don't subscribe
                # Filtering doesn't matter here
                for bus in board.publish:
                    if bus.name not in board.subscribe.name:
                        fw(
                            'void CANlib_Init_' + coord(bus.name, board.name) +
                            '(void) {' '\n'
                        )
                        fw('\t' 'CANlib_Init({});\n'.format(bus.baudrate))
                        fw('}' '\n\n')

        for bus in can.bus:
            # Write switch statement
            fw((
                '{}_T CANlib_Identify_{}(Frame* frame)'.format(coord(bus.name), coord(bus.name, prefix=False)) + '{' '\n'
                '\t' 'switch(frame->id) {' '\n'
            ))

            for msg in bus.frame:
                fw(
                    '\t' '\t' 'case {}_id:\n'.format(coord(bus.name, msg.name)) +
                    '\t' '\t' '\t' 'return {};\n'.format(coord(bus.name, msg.name))
                )

            fw(
                '\t' 'default:' '\n'
                '\t' '\t' 'return CAN_UNKNOWN_MSG;' '\n'
                '\t' '}' '\n'
                '}' '\n\n'
            )

        for bus in can.bus:
            for msg in bus.frame:

                # Write CAN_PACK
                fw(
                    'CAN_PACK(' + coord(bus.name, msg.name, prefix=False) + ') {' '\n'
                    '\t' 'uint64_t bitstring = 0;' '\n'
                )

                for atom in msg.atom:
                    # HACK/TODO: This is assuming big endian systems that run CANlib
                    if atom.type.endianness == Endianness.LITTLE:
                        fw(
                            '\t' 'bitstring = INSERT(' + swap_endianness_fn(atom.type) + '(type_in->' + atom.name + '), bitstring, ' +
                            str(atom.slice.start) + ', ' + str(atom.slice.length) + ');' '\n\n'
                        )
                    else:
                        fw(
                            '\t' 'bitstring = INSERT(type_in->' + atom.name + ', bitstring, ' + str(atom.slice.start) +
                            ', ' + str(atom.slice.length) + ');' '\n'
                        )

                length = max(atom.slice.start + atom.slice.length for atom in msg.atom)

                fw(
                    '\t' 'from_bitstring(&bitstring, can_out->data);' '\n'
                    '\t' 'can_out->id = {}_id;'.format(coord(bus.name, msg.name)) + '\n'
                    '\t' 'can_out->dlc = ' + str(ceil(length / 8)) + ';' '\n'
                    '\t' 'can_out->extended = ' + str(bus.extended).lower() + ';' '\n'
                    '}' '\n\n'
                )

                # Write CAN_UNPACK
                fw(
                    'CAN_UNPACK(' + coord(bus.name, msg.name, prefix=False) + ') {' '\n'
                    '\t' 'uint64_t bitstring = 0;' '\n'
                    '\t' 'to_bitstring(can_in->data, &bitstring);\n'
                )

                for atom in msg.atom:
                    if atom.type.isenum():
                        enum_name = coord(bus.name, msg.name, atom.name) + '_T'

                        fw(
                            '\t' 'type_out->' + atom.name + ' = (' + enum_name + ')EXTRACT(bitstring, ' +
                            str(atom.slice.start) + ', ' + str(atom.slice.length) + ');' '\n'
                        )
                    elif atom.type.type == 'bool':
                        fw(
                            '\t' 'type_out->' + atom.name + ' = EXTRACT(bitstring, ' + str(atom.slice.start) + ', ' +
                            str(atom.slice.length) + ');' '\n'
                        )
                    else:
                        if atom.type.endianness == Endianness.LITTLE:
                            fw(
                                '\t' 'type_out->' + atom.name + ' = ' + swap_endianness_fn(atom.type) +
                                '(EXTRACT(bitstring, ' + str(atom.slice.start) + ', ' +
                                str(atom.slice.length) + '));' '\n'
                            )
                        else:
                            if atom.type.issigned():
                                fw(
                                    '\t' 'type_out->' + atom.name + ' = SIGN(EXTRACT(bitstring, ' +
                                    str(atom.slice.start) + ', ' + str(atom.slice.length) + '), ' +
                                    str(atom.slice.length) + ');' '\n'
                                )
                            else:
                                fw(
                                    '\t' 'type_out->' + atom.name + ' = EXTRACT(bitstring, ' +
                                    str(atom.slice.start) + ', ' + str(atom.slice.length) + ');' '\n'
                                )

                fw('}' '\n\n')

        # Write DEFINE statements
        for bus in can.bus:
            for msg in bus.frame:
                fw('DEFINE(' + coord(bus.name, msg.name, prefix=False) + ')\n')
