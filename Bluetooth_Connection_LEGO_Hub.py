import asyncio

from bleak import BleakClient # Bluetooth client

# UUID for LEGO Hub Characteristic (Wireless Protocol v3)
CHAR_UUID = "00001624-1212-efde-1623-785feabcd123"

# MAC-Adress
ADDRESS = "90:84:2B:11:3B:47"

# Command to control the motor
def set_motor_command(power):
    return bytearray([0x08, 0x00, 0x81, 0x00, 0x11, 0x51, 0x00, power])

# Main Loop
async def main():
    
    async with BleakClient(ADDRESS) as client:
        print("Connected to hub.")

        # Send motor command
        await client.write_gatt_char(CHAR_UUID, set_motor_command(50), response=True)
        print("Motor on Port A started at 50% speed.")

        # Optional: run for x seconds then stop
        await asyncio.sleep(30)

        # Stop command (set power to 0)
        await client.write_gatt_char(CHAR_UUID, set_motor_command(0), response=True)
        print("Motor stopped.")

asyncio.run(main())

# The code was developed by Luise J. during her student internship at Oracle. All rights reserved --> see "LICENSE" for details.
