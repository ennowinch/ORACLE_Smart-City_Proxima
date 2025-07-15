import cv2 # OpenCV

# Preparatiopn for detection of the toycar with OpenCV and coconames
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
# check
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
