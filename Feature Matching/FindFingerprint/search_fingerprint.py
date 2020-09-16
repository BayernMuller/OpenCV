import handle_finder
import win32api
import win32con
import win32gui
from PIL import ImageGrab
import numpy as np
import cv2
windowName = 'fingerprint'


class finder:
	def __init__(self):
		self.output = np.ones((566,276,3), np.uint8)
		cv2.imshow(windowName, self.output)

		hf = handle_finder.handle_finder()
		self.hwnd = hf.get_handle(windowName)

		win32gui.SetWindowPos(self.hwnd, win32con.HWND_TOPMOST, 0, 0, 0, 0, win32con.SWP_NOMOVE | win32con.SWP_NOSIZE)

	def run(self):
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
			self.output = img = np.ones((566,276,3), np.uint8)
			print('There is no fingerprint')
			return

		print(pattern)

		self.output = cv2.cvtColor(np.array(ImageGrab.grab(bbox=(469,261,745,827))), cv2.COLOR_BGR2RGB)
		pattern_img = cv2.imread('pattern\\pattern{}.png'.format(pattern), 0)

		height, width = pattern_img.shape[:2]
		height = int(height / 2)
		width = int(width / 2)

		for i in range(2):
			for j in range(2):
				pattern_split = pattern_img[i * height : (i+1) * height, j * width : (j+1) * width]

				kp1, des1 = sift.detectAndCompute(pattern_split, None)
				kp2, des2 = sift.detectAndCompute(self.output, None)
				matches = bf.knnMatch(des1, des2, k = 2)	

				for m,n in matches:
						if m.distance < 0.4 * n.distance:
							point = kp2[m.trainIdx].pt
							cv2.circle(self.output, (int(point[0] + 0.5), int(point[1] + 0.5)), 3, (0, 0, 255), -1)


if __name__ == '__main__':
	finder = finder()
	while (True):
		try:
			finder.run()
			cv2.imshow(windowName, finder.output)
		except:
			print('failed')
		if (cv2.waitKey(500) == 27):
			break