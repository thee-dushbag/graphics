"""
My first PyQt5 application
"""

from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QMainWindow
import sys


"""
All root widgets (widgets without parents) are windows.
PyQt5 does however provide a Standard Window that is
designed to be used for windowing. All windows are invisible
by default, you have to call '.show()' to make them visible.
"""

def onclick(initial: int | None = None):
    counter = initial or 0

    def clicked():
        nonlocal counter
        counter += 1
        print("Button Clicked: %r" % counter)

    return clicked


app = QApplication(sys.argv)
# window = QWidget()
window = QMainWindow()
# window = QPushButton("Click Me!")
window.setWindowTitle("First Application")
# window.clicked.connect(onclick(-5))
window.show()
app.exec_()
