import tkinter as tk
import random as rd
import time

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    def __add__(self, rhp):
        return Point(self.x + rhp.x, self.y + rhp.y)
    def __div__(self, rhf):
        return Point(self.x / rhf, self.y / rhf)

startPoint = Point(5,5)
endPoint = Point(635, 475)
minDiff = 5

def drawLighting(beg, end, diff, painter):
    if diff < minDiff:
        painter.create_line(beg.x, beg.y, end.x, end.y, fill = "blue")
        return
    mid = beg + end
    mid.x /= 2
    mid.y /= 2
    mid.x += (rd.random() - 0.5) * diff
    mid.y += (rd.random() - 0.5) * diff
    drawLighting(beg, mid, diff / 2, painter)
    drawLighting(mid, end, diff / 2, painter)

app = tk.Tk()
app.title('Lighting Python3')
cvs = tk.Canvas(app, width = 640, height = 480)
cvs.pack()

def mainloop(painter):
    while True:
        painter.delete(tk.ALL)
        drawLighting(startPoint, endPoint, 100, painter)
        app.update()
        time.sleep(0.1)

mainloop(cvs)
