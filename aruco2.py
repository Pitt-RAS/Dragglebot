import numpy as np
import cv2
import cv2.aruco as aruco
import serial
 
'''Myserial = serial.Serial('com7',9600)''' 

cap = cv2.VideoCapture(0)
 
while(True):
    # Capture frame-by-frame
    ret, frame = cap.read()
    #print(frame.shape) #480x640
    # Our operations on the frame come here
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    aruco_dict = aruco.Dictionary_get(aruco.DICT_6X6_250)
    parameters =  aruco.DetectorParameters_create()

        #lists of ids and the corners beloning to each id
    corners, ids, rejectedImgPoints = aruco.detectMarkers(gray, aruco_dict, parameters=parameters)
    #corners2 = np.zeros((4,2))

    if(len(corners) != 0):
        x = corners.pop()
        print(x)
    #corners2 = corners
    #print(corners2)
    #print(corners(0, 0))
        print(x[0][0][1])
        xcenter = int((x[0][0][0] + x[0][1][0])/2)
        ycenter = int((x[0][0][1] + x[0][2][1])/2)
        print(xcenter)
        print(ycenter)
        cv2.circle(gray, (xcenter, ycenter), 5, (0, 0, 255), -1)
        

    '''
        margin = 15;
    
        if center-margin < 220 and center+margin > 180 and len(faces) == 1:
            print("centered")
            Myserial.write('Y'.encode())
            print('Y')
      
      
        elif(len(faces) == 1):

        print("not centered")
        if center+margin > 220:
            Myserial.write('R'.encode())
            print('R')
        else:
            Myserial.write('L'.encode())
            print('L')'''
    gray = aruco.drawDetectedMarkers(gray, corners)

    # Display the resulting frame
    cv2.imshow('frame',gray)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
 
cap.release()
cv2.destroyAllWindows()