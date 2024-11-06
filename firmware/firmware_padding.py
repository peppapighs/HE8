import shutil
import sys

CRC32_TABLE = [
    0x00000000,
    0x04C11DB7,
    0x09823B6E,
    0x0D4326D9,
    0x130476DC,
    0x17C56B6B,
    0x1A864DB2,
    0x1E475005,
    0x2608EDB8,
    0x22C9F00F,
    0x2F8AD6D6,
    0x2B4BCB61,
    0x350C9B64,
    0x31CD86D3,
    0x3C8EA00A,
    0x384FBDBD,
]


def crc32(path: str):
    with open(path, "rb") as f:
        crc = 0xFFFFFFFF
        while True:
            data = f.read(4)
            if not data:
                break
            crc ^= int.from_bytes(data, "little")
            for _ in range(8):
                crc = ((crc << 4) & 0xFFFFFFFF) ^ CRC32_TABLE[crc >> 28]

    return crc


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"Usage: python {sys.argv[0]} <path> <output>")
        sys.exit(1)

    crc = crc32(sys.argv[1])
    print(f"CRC32: {crc:08X}")

    shutil.copyfile(sys.argv[1], sys.argv[2])
    with open(sys.argv[2], "ab") as f:
        f.write(b"\xEF\xBE\xAD\xDE")
        f.write(b"\xBA\xDC\xCD\xAB")
        f.write(crc.to_bytes(4, "little"))
