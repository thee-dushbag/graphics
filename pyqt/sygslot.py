"""
Signals are notifications send by widgets
when some event occurs.

Slots are the handlers that react to target
signals.

Signals can carry data which will be shared
to the slots hence wigdet communication.

A Signal can be handled by many Slots.
"""

from PyQt5.QtWidgets import QApplication, QMainWindow, QPushButton
from PyQt5.QtCore import Qt, QSize
import sys, time


class MainWindow(QMainWindow):
    def __init__(self) -> None:
        super().__init__()
        self.setWindowTitle("Signals and Slots")
        self.setMinimumSize(QSize(400, 300))
        btn = QPushButton("SayHi")
        # self.setCentralWidget(btn)
        btn.clicked.connect(self.on_click_slot)
        btn2 = QPushButton("Toggle Me!")
        btn2.setCheckable(True)
        btn2.clicked.connect(self.on_toggle_slot)
        # But remember, since toggling is also clicking
        # then connecting the on_click_slot will be called
        # on every toggle but will not receive any data since
        # it doesn't accept any. You can accept and don't use.
        # PyQt will automatically handle the acceptance gracefully.
        btn2.clicked.connect(self.on_click_slot)
        # self.setCentralWidget(btn2)
        self.btn3 = QPushButton("Click me Twice!")
        self.btn3.clicked.connect(self.disable_btn3)
        self.setCentralWidget(self.btn3)
        self.windowTitleChanged.connect(self.window_title_changed)

    def window_title_changed(self, new_title: str):
        print("Window title changed by btn3, will be enabled.")
        print("New title: %r" % new_title)
        self.btn3.setEnabled(True)
        self.btn3.setText("Click me Once!")

    def disable_btn3(self):
        self.btn3.setText("You've already clicked me.")
        self.btn3.setDisabled(True)
        print("Night Night Bro.")
        self.setWindowTitle("Schedule btn3 enablement")
        # self.btn3.setEnabled(False)

    def on_toggle_slot(self, state: bool):
        """
        btn2 is checkable and hence has some data
        which is either it is on or off that can
        be represented as a boolean and passed to
        this slot as an argument state and used to
        tune the application.
        """
        print("I'm now %s." % ("On" if state else "Off"))

    def on_click_slot(self):
        """
        React to the 'SayHi' button click.
        For normal buttons no data is passed
        as there is none to pass.
        """
        print("Hello There?")


def main():
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    return app.exec_()


if __name__ == "__main__":
    exit(main())
