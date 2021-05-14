import serial
import time


flash_mem = 0x08000000



f = open('dump_firm', 'w+b')
ser = serial.Serial('/dev/ttyUSB0',115200,8,parity=serial.PARITY_EVEN)
print(ser)
ser.write(b'\x7F')
ser.timeout=5
print(ser.read(1))

for i in range (1024):
    flash_mem_bytes = flash_mem.to_bytes(4,byteorder="big") 

    checksum = flash_mem_bytes[0]^flash_mem_bytes[1]^flash_mem_bytes[2]^flash_mem_bytes[3]
    checksum_bytes = checksum.to_bytes(1,byteorder="big")

    flash_mem_with_checksum = flash_mem_bytes + checksum_bytes

    ser.write(b"\x11\xEE")
    ser.read(1)
    ser.write(flash_mem_with_checksum)
    ser.timeout=5
    ser.read(1)

    ser.write(b"\xFF\x00")
    ser.timeout=5
    ser.read(1)
    f.write(ser.read(255))
    flash_mem+=256




