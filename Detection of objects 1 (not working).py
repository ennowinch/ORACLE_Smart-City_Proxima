import cv2 
import asyncio
from bleak import BleakClient

ADDRESS = "90:84:2B:11:3B:47" 
CHAR_UUID = "00001624-1212-EFDE-1623-785FEABCD123"

START_CMD = bytearray([0x08, 0x00, 0x81, 0x00, 0x11, 0x51, 0x00, 0x32])
STOP_CMD = bytearray([0x08, 0x00, 0x81, 0x00, 0x11, 0x51, 0x00, 0x00])

# Objekterkennung
classNames = []
with open("/home/piuser/Desktop/Object_Detection_Files/coco.names", "rt") as f:
	classNames = f.read().rstrip("\n").split("\n")

configPath = "/home/piuser/Desktop/Object_Detection_Files/ssd_mobilenet_v3_large_coco_2020_01_14.pbtxt"
weightsPath = "/home/piuser/Desktop/Object_Detection_Files/frozen_inference_graph.pb"

net = cv2.dnn_DetectionModel(weightsPath, configPath)
net.setInputSize(320, 320)
net.setInputScale(1.0 / 127.5)
net.setInputMean((127.5, 127.5, 127.5))
net.setInputSwapRB(True)


# Hauptteil
async def control_motor(client, command): 
	await client.write_gatt_char(CHAR_UUID, command)

async def main():
	async with BleakClient(ADDRESS) as client: 
		print("verbunden") 
		await control_motor(client, START_CMD)
		print("zug startet")

		cap = cv2.VideoCapture(0) 
		cap.set(3, 640)
		cap.set(4, 480)

		try: 
			while True:
				success, img = cap.read()
				if not success:
					print("Fehler")
					break
				result, objectInfo = getObjects(img, 0.45, 0.2, objects=['car'])
# Prüfung
				if objectInfo:
					print("Auto erkannt")
					await client.write_gatt_char(CHAR_UUID, STOP_CMD)
					break


				cv2.imshow("Kamera", img)
				if cv2.waitKey(1) & 0xFF == ord('q'):
					break

		finally:
			cap.release()
			cv2.destroyAllWindows()

asyncio.run(main()) 
