import cv2

img = cv2.imread('ThermalVisionTestPics\Drone-Thermal-Camera-for-Search-Rescue.jpg')

#convert to grayscale
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
gray2 = gray

#display original and grayscale images
cv2.imshow('image', img)
cv2.imshow('gray', gray)


#Go pixel- by pixel and find "hotspots"
rows,cols = gray.shape
for i in range(rows):
  for j in range(cols):
    k = gray[i,j]
    
    if (gray[i, j] <= 150):
        heatCoordinates = [i,j]
        gray2[i, j] = 0
    
cv2.imshow('gray2', gray2)


cv2.waitKey(10000)
cv2.destroyAllWindows()

