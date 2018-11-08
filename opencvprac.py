import cv2
import numpy as np
import serial
import time

Myserial = serial.Serial('com6',9600)

face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
eye_cascade = cv2.CascadeClassifier('haarcascade_eye.xml')

cap = cv2.VideoCapture(1)

'''while(1):
    ret, frame = cap.read()
    #print(height)
    #cv2.imshow("Cropped Image", crop_img)
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    cv2.imshow('frame',frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()'''
while True:
  ret, img = cap.read()
  gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
  faces = face_cascade.detectMultiScale(gray, 1.3, 5)
  for (x,y,w,h) in faces:
    cv2.rectangle(img, (x,y), (x+w, y+h), (255,0,0), 2)
    roi_gray = gray[y:y+h, x:x+w]
    roi_color = img[y:y+h, x:x+w]
    eyes = eye_cascade.detectMultiScale(roi_gray)
    for(ex, ey, ew, eh) in eyes:
      cv2.rectangle(roi_color, (ex, ey), (ex+ew, ey+eh), (0,255,0), 2)

  cv2.imshow('img', img)
  
  if (len(faces)) == 0:
    Myserial.write('N'.encode())
    print ("No face is detected")
  elif (len(faces)) == 1:
    Myserial.write('Y'.encode())
    print ("Face is detected")
    
  key = cv2.waitKey(5) & 0xFF
    # tests if the q key has been pressed
  if key == ord("q"):
    break

    # releases the webcam
cap.release()

cv2.destroyAllWindows()

