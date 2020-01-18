import pcapng
import sys


def dump_information(scanner):
    FLAG = ''
    data = ''

    for block in scanner:
        if type(block) is pcapng.blocks.EnhancedPacket:
            if block.packet_payload_info[0] == 31:
                if block.packet_payload_info[2][-1] == 127:
                    hex(block.packet_payload_info[2][-2])[-1]
                    
                    data += '{}'.format(hex(block.packet_payload_info[2][-2])[-1])

    for k in [(i + j) for (i, j) in zip(data[::2], data[1::2])]:
        FLAG += '{}'.format(chr(int(k, 16)))

    print(FLAG)


if __name__ == '__main__':
    if len(sys.argv) == 2:
        with open(sys.argv[1], 'rb') as packet_file:
            scanner = pcapng.FileScanner(packet_file)
            dump_information(scanner)
