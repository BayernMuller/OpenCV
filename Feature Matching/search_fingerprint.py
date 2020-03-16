from PIL import ImageGrab
import numpy as np
import cv2
from time import sleep

def run():
	sift = cv2.xfeatures2d.SIFT_create()
	bf = cv2.BFMatcher(normType=cv2.NORM_L2, crossCheck=False)
	finger_print = cv2.cvtColor(np.array(ImageGrab.grab(bbox=(973, 153, 1317, 655))), cv2.COLOR_BGR2RGB)

	pattern = -1
	matches = None
	kp1 = None
	kp2 = None
	for i in range(1,5):
		finger = cv2.imread('sample\\print{}.png'.format(i), 0)
		kp1, des1 = sift.detectAndCompute(finger, None)
		kp2, des2 = sift.detectAndCompute(finger_print, None)
		matches = bf.knnMatch(des1, des2, k=2)	
		cnt = 0
		for m,n in matches:
		    if m.distance < 0.3 * n.distance:
		        cnt = cnt + 1

		if (cnt > 20):
			pattern = i;
			break
	 
	if pattern == -1:
		print('There is no fingerprint')
		return

	print(pattern)

	finger_print = cv2.cvtColor(np.array(ImageGrab.grab(bbox=(469,261,745,827))), cv2.COLOR_BGR2RGB)
	pattern_img = cv2.imread('pattern\\pattern{}.png'.format(pattern), 0)

	height, width = pattern_img.shape[:2]
	height = int(height / 2)
	width = int(width / 2)


	for i in range(2):
		for j in range(2):
			pattern_split = pattern_img[i * height : (i+1) * height, j * width : (j+1) * width]

			kp1, des1 = sift.detectAndCompute(pattern_split, None)
			kp2, des2 = sift.detectAndCompute(finger_print, None)
			matches = bf.knnMatch(des1, des2, k = 2)	

			for m,n in matches:
					if m.distance < 0.4 * n.distance:
						point = kp2[m.trainIdx].pt
						cv2.circle(finger_print, (int(point[0] + 0.5), int(point[1] + 0.5)), 3, (0, 0, 255), -1)
			

	cv2.imshow("", finger_print)
	cv2.waitKey()
	cv2.destroyAllWindows()

if __name__ == '__main__':
	while (True):
		try:
			run()
		except:
			print('failed')
		
	
		sleep(0.5)