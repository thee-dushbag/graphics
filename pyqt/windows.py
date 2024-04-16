from PyQt5.QtWidgets import QApplication, QMainWindow, QPushButton
from PyQt5.QtGui import QIcon, QMouseEvent


class NewWindow(QPushButton):
    def __init__(self):
        super().__init__()
        self.setText("")
        self.setFixedSize(300, 200)
        icon = QIcon("./icons.ign/brands/google.svg")
        self.setWindowIcon(icon)
        self.setWindowTitle("New Window")


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        icon = QIcon("./icons.ign/brands/apple.svg")
        self.setWindowIcon(icon)
        self.setWindowTitle("Main Window")
        btn = QPushButton("New Window")
        btn.clicked.connect(self.create_window)
        self.setCentralWidget(btn)
        self.setFixedSize(400, 300)
        self.new_window: NewWindow | None = None

    def mouseMoveEvent(self, a0: QMouseEvent | None) -> None:
        print("Mouse Moved")
        return super().mouseMoveEvent(a0)

    def create_window(self):
        if self.new_window is None:
            self.new_window = NewWindow()
            self.new_window.clicked.connect(self.action_slot)
            self.new_window.show()
        elif self.new_window.isHidden():
            self.new_window.show()
        elif self.new_window.isvisible():
            self.new_window.hide()
        else:  # Unreachable
            self.new_window.close()
            self.new_window = None

    def action_slot(self):
        print("Button on the new window was clicked.")


def main(argv: list[str] | None = None):
    app = QApplication(argv or [])
    window = MainWindow()
    window.show()
    return app.exec()


if __name__ == "__main__":
    exit(main())
