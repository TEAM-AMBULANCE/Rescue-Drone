'''
GENERAL IDEA/STRUCTURE:

-------------------METHOD 1: IF the radios are strong enough to send video at decent fps:--------

1. 3 audio sensors detect which direction the distress is coming from
2. when near the audio distress, the arducam turns on and start sending live video data
3. groundstation then uses comp vision to see if human temp range is being displayed
4. if so the pi then detects contours to see if the object is similar shape to that of a human


--------------------METHOD 2: if the radios arent strong enough to send video at a decent fps:--------------


1. 3 audio sensors detect which direction the distress is coming from
2. when near the audio distress, the arducam turns on 
3. then arducam converts the usual 8-bit colour images to lesser 4-bit gray scale images (for faster transmission) 
4. the arduino detects weither or not theres a temperature range being displayed (that of a human radiating heat of their skin)
5. if the cam detects a valid temp range then the camera sends a picture to ground station, with the drones gps location
6. the groundstation then processes the image to detect contours to see if the object is similar shape to that of a human



'''

#-------------------------------------METHOD 2:--------------------------

import numpy as np
import cv2
import time

Method = 'M' # M indicates main code(how it should look like in the end), 'E' indicates experimental (how data the handling actually is)


    

if Method == 'M':
    colour_img = cv2.imread("D:\Alessandro\FILESFORSCHOOL\AER817\HumanIRimage_2_gray_QCIF.bmp")

    #convert to grayscale
    gray = cv2.cvtColor(colour_img, cv2.COLOR_BGR2GRAY)
    gray2 = gray

    #display original and grayscale images
    cv2.imshow('original image', colour_img)


    #Go pixel- by pixel and find "hotspots"
    rows, cols = gray.shape
    for i in range(rows):
        for j in range(cols):
            k = gray[i, j]

            if (gray[i, j] <= 150): #150 is rgb value for lowest bound gray image
                heatCoordinates = [i, j]
                gray2[i, j] = 0
                
    cv2.imshow('grayed image processed 2nd time', gray2)

    # Search for edges in the image with cv2.Canny().
    edges = cv2.Canny(gray2, 100, 200)

    # Search for contours in the edged image with cv2.findContour().
    contours, hierarchy = cv2.findContours(edges, cv2.RETR_TREE, cv2.CHAIN_APPROX_NONE)

    # Filter out contours that are not in your interest by applying size criterion.
    
    for cnt in contours:
        size = cv2.contourArea(cnt)
        cv2.drawContours(gray2, [cnt], -1, (255, 0, 0), 3)

    cv2.imshow('contour result', gray2)

    cv2.imshow('edges result', edges)



    if cv2.waitKey(1000000) & 0xFF == ord('q'):

        cv2.destroyAllWindows()



