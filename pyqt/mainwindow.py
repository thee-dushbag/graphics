from PyQt5.QtCore import QSize, Qt
from PyQt5.QtWidgets import QApplication, QMainWindow, QPushButton
from itertools import count
import sys

"""
Qt Sizing is done with the methods
    setFixedSize
    setFixedHeight
    setFixedWidth
    setMaximumSize
    setMaximumHeight
    setMaximumWidth
    setMinimumSize
    setMinimumHeight
    setMinimumWidth
on all widgets including the Window.
"""

class MainWindow(QMainWindow):
    def __init__(self) -> None:
        super().__init__()
        self.setWindowTitle("Subclass QMainWindow")
        self.btn = QPushButton("Click Me!!")
        self.btn.setFixedSize(QSize(300, 50))
        self.setCentralWidget(self.btn)
        self.counter = count(1)
        self.btn.clicked.connect(self.on_click)
        # self.setFixedSize(QSize(400, 300))
        # self.setFixedHeight(500)
        # self.setFixedWidth(500)
        # self.setMaximumSize(QSize(400, 300))
        # self.setMaximumHeight(400)
        # self.setMaximumWidth(300)
        # self.setMinimumSize(QSize(400, 300))
        # self.setMinimumHeight(400)
        # self.setMinimumWidth(300)

    def on_click(self):
        self.btn.setText("Clicked %r times." % next(self.counter))


app = QApplication(sys.argv)
window = MainWindow()
window.show()
app.exec_()
