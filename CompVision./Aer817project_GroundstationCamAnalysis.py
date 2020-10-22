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

 
# assuming the image comes back as a grayscale:

gray_img = cv2.imread("D:\Alessandro\FILESFORSCHOOL\AER817\HumanThermalimage_2_gray.bmp")

#adding some colour:==>from==> https://stackoverflow.com/questions/58135918/giving-grayscale-image-color
'''
b, g, r = cv2.split(gray_img)

np.multiply(b, 1.5, out=b, casting="unsafe")
np.multiply(g, .75, out=g, casting="unsafe")
np.multiply(r, 1.25, out=r, casting="unsafe")

colour_img = cv2.merge([b, g, r])

cv2.imshow('gray img', gray_img)
cv2.imshow('colour img', colour_img)
'''
# openCV stuff from==> https://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_imgproc/py_colorspaces/py_colorspaces.html#

#convert to hsv for image processing:
#cv2.imshow('gray img', gray_img)



hsv_grayimg = cv2.cvtColor(gray_img, cv2.COLOR_BGR2HSV)
#cv2.imshow("hsv gray image", hsv_grayimg)

#hsv_colourimg = cv2.cvtColor(colour_img, cv2.COLOR_BGR2HSV)
#cv2.imshow("hsv colour image", hsv_colourimg)

#thresholding to only display yellow colour(similar to human skin temp emission):

# Starting with 100's to prevent error while masking
h_lower=s_lower=v_lower=h_upper=s_upper=v_upper=0 
# Creating track bar

#for lower bounds:


def nothing(x):
    pass


# Creating a window for later use (in order to see track bar)

#for upper bounds:
cv2.namedWindow('result')

cv2.createTrackbar('h_lower', 'result', 0, 179,nothing) # ranges H:0-179,S=0-255,v=0-255
cv2.createTrackbar('s_lower', 'result', 0, 255,nothing)
cv2.createTrackbar('v_lower', 'result', 0, 255,nothing)


cv2.createTrackbar('h_upper', 'result', 0, 179,nothing)  # ranges H:0-180,S=0-255,v=0-255
cv2.createTrackbar('s_upper', 'result', 0, 255,nothing)
cv2.createTrackbar('v_upper', 'result', 0, 255,nothing)

while(1):
            
    # get info from track bar and appy to result

    #for lower bounds:
    h_lower = cv2.getTrackbarPos('h_lower', 'result')
    s_lower = cv2.getTrackbarPos('s_lower', 'result')
    v_lower = cv2.getTrackbarPos('v_lower', 'result')

    #for upper bounds:
    h_upper = cv2.getTrackbarPos('h_upper', 'result')
    s_upper = cv2.getTrackbarPos('s_upper', 'result')
    v_upper = cv2.getTrackbarPos('v_upper', 'result')

    #applying the trackbar values:
    lowerBound = np.array([h_lower, s_lower, v_lower]) # array numbers from(3rd array ("v") represents brightness lvl, usually want range to be in betw. 20 and 255)==> https://stackoverflow.com/questions/48528754/what-are-recommended-color-spaces-for-detecting-orange-color-in-open-cv

    upperBound = np.array([h_upper, s_upper,v_upper])# want to cover whole range of colour "rectangle" (so use corner to corner values)




    # Threshold the HSV image to get only yellow colors
    mask = cv2.inRange(hsv_grayimg, lowerBound, upperBound)

    # Bitwise-AND mask and original image
    res = cv2.bitwise_and(hsv_grayimg, hsv_grayimg, mask=mask) #only displays pixel if both images have the same pixel location and colour as tha mask 

    #cv2.imshow('yellow mask', mask)
    cv2.imshow('res', res)

    if cv2.waitKey(10) & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()




