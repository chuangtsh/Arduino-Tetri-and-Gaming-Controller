import pyautogui
import time
import math
import keyboard
pyautogui.FAILSAFE = False
state = False
while True:
    if keyboard.is_pressed('o'):
        state = True
    while state:
        cur_x, cur_y = pyautogui.position()
        length = math.sqrt(pow((cur_x-950), 2) + pow((cur_y-530), 2))
        if (length > 225):
            sine = (cur_y-530) / length
            cosine = (cur_x-950) / length
            movex = 950 + 225.0 * cosine
            movey = 530 + 225.0 * sine
            pyautogui.moveTo(movex, movey, duration=0.000001)
        if keyboard.is_pressed('c'):
            state = False
            break
