
import cv2
import numpy as np
from math import sqrt

ori = cv2.imread("img/sample (4).jpg", cv2.IMREAD_COLOR)
src = cv2.medianBlur(ori, 13) # 블러로 부드럽게 만듦
#src = cv2.cvtColor(src, cv2.COLOR_BGR2HSV)
canny = cv2.Canny(src, 100, 255) # 윤곽 검출
contours, hierarchy = cv2.findContours(canny, cv2.RETR_CCOMP, cv2.CHAIN_APPROX_SIMPLE) # 테두리 검출

height, width = src.shape[:2]
empty = np.zeros((height,width,1), np.uint8) # 빈 행렬 생성

for i in range(len(contours)):
    cv2.drawContours(empty, [contours[i]], 0, (255, 255, 255), 1) # 테두리를 그림
   
_, temp = cv2.threshold(empty, 254, 255, cv2.THRESH_BINARY) # 이진 행렬 생성
lines = cv2.HoughLinesP(temp,1,np.pi/180,100,minLineLength=70,maxLineGap=50) # 허프 변환

row = width * 0.17 # 0.17 = boxWidth / ImageWidth
col = height * 0.28 # 0.28 = boxHeight / ImageHeight

vertical = []
horizontal = []

for line in lines: # 선들을 탐색해 수직인 선과 수평인 선을 구분함. 나머지 선은 소거
    x1,y1,x2,y2 = line[0]
    if y1 < height * 0.8 and y2 < height * 0.8: # 메뉴바 영역 소거
        if abs(x1 - x2) < 2: # 수직
            vertical.append(line[0])
        if abs(y1 - y2) < 2: # 수평
            horizontal.append(line[0])

'''
3가지의 경우
- 1. 박스의 2개의 수평선이 인식되어 평행을 이루는 경우
- 2. 박스의 2개의 수직선이 인식되어 평행을 이루는 경우
- 3. 박스의 2개의 선이 인식되어 서로 직각을 이루는 경우  
'''
rectangles = []

# case 1:
for i in range(len(vertical)):
    line = vertical[i]
    for j in range(len(vertical)):
        if i == j:
            continue
        target = vertical[j]
        if abs(abs(line[0] - target[0]) - row) < 10: # 두 선 사이 거리가 row와 비슷해야함 (오차범위 10)
            if abs(abs(line[1] - target[3]) - col) < 10:
                rectangles.append(((line[0], line[1]), (target[2], target[3])))
                
# case 2:
for i in range(len(horizontal)):
    line = horizontal[i]
    for j in range(len(horizontal)):
        if i == j:
            continue
        target = horizontal[j]
        if abs(abs(line[1] - target[3]) - col) < 10:
            if abs(abs(line[0] - target[2]) - row) < 10:
                rectangles.append(((line[0], line[1]), (target[2], target[3])))

# case 3:
circles = []
for h in horizontal:
    for v in vertical:
        if abs(h[0] - v[0]) < 8:
            circles.append((h[0], h[1]))
        if abs(h[2] - v[2]) < 8:
            circles.append((h[2], h[1]))

for rect in rectangles:
    cv2.rectangle(ori, rect[0], rect[1], (255,0,0), 3)
    cv2.putText(ori, 'BOX', rect[0], 0, 1,(255,0,0), 2)
    break

for cir in circles:
    cv2.circle(ori, cir, 2, (0,0,255), 2)

cv2.imshow("ori", ori)

cv2.waitKey(0)
cv2.destroyAllWindows()