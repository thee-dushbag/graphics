"""
Dialogs are a way to prompt, confirm and
notify the user as printing and reading
from terminal are to terminal applications.
They enable us to ask for some required
data, inform the user of any errors or
updates they need to know about.

PyQt5 provides dialogs using the QDialog
class objects; they appear as small windows
that block the event loop until they are
closed.
"""

from layouts import widgetize_layout
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import (
    QDialogButtonBox,
    QApplication,
    QMainWindow,
    QMessageBox,
    QVBoxLayout,
    QPushButton,
    QDialog,
    QWidget,
    QLabel,
)


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        icon = QIcon("./image.png")
        self.setWindowIcon(icon)
        self.setWindowTitle("PyQt5 Dialogs")
        self.setMinimumSize(400, 300)
        widget = self.setup_dialog_btn()
        self.setCentralWidget(widget)

    @widgetize_layout
    def setup_dialog_btn(self):
        lout = QVBoxLayout()
        dialog = QPushButton("Show Dialog")
        lout.setAlignment(Qt.AlignmentFlag.AlignCenter)
        dialog.clicked.connect(self.show_dialog)
        dialog.setFixedSize(130, 40)
        lout.addWidget(dialog)
        return lout

    def show_dialog(self):
        return self.simple_msg_dialog()
        return self.custom_dialog()
        return self.empty_dialog()

    def empty_dialog(self):
        dialog = QDialog(self)
        dialog.setWindowTitle("Empty Dialog")
        dialog.setMinimumSize(250, 150)
        dialog.exec_()

    def simple_msg_dialog(self):
        dialog = QMessageBox(self)
        dialog.setWindowTitle("I have a question!")
        dialog.setText("Is Simon your name?")
        dialog.setIcon(QMessageBox.Icon.Question)
        StdBtn = QMessageBox.StandardButton
        btns = StdBtn(StdBtn.Yes | StdBtn.No | StdBtn.Ignore)
        dialog.setStandardButtons(btns)
        # dialog.setDefaultButton(StdBtn.No)
        shortcut = lambda: QMessageBox.critical(
            self,
            "You are fucked",
            "You just dropped production database!",
            StdBtn.Ignore,
        )
        # result = dialog.exec_()
        result = shortcut()
        match result:
            case StdBtn.Yes:
                print("It's an honor knowing your name.")
            case StdBtn.No:
                print("I thought it was your name, we need to talk more.")
            case StdBtn.Ignore:
                print("You don't have to be rude about it!!")
            case _:
                print("What is this? %r" % result)

    def custom_dialog(self):
        dialog = CustomDialog(self)
        dialog.exec_()
        match dialog.result():
            case CustomDialog.MyCodes.Okay:
                print("MyCodes.Okay")
            case CustomDialog.MyCodes.Cancel:
                print("MyCodes.Cancel")
            case CustomDialog.MyCodes.Unknown:
                print("MyCodes.Unknown")
            case result:
                print("Unknown result: %r, expected MyCodes." % result)
        # match dialog.result():
        #     case QDialog.DialogCode.Accepted:
        #         print("Accepted: Okay.")
        #     case QDialog.DialogCode.Rejected:
        #         print("Rejected: Cancel.")
        #     case result:
        #         print("Unknown value: %r" % result)


class CustomDialog(QDialog):
    class MyCodes:
        Okay = 0
        Cancel = 1
        Unknown = 2

    def __init__(self, parent: QWidget | None = None) -> None:
        super().__init__(parent)
        self.setWindowTitle("Custom Dialog")
        StdBtn = QDialogButtonBox.StandardButton
        QBtn = StdBtn(StdBtn.Ok | StdBtn.Cancel)
        # btn = QDialogButtonBox.StandardButtons(QBtn)
        slot = lambda n: self.done(n)
        accept_slot = lambda: slot(self.MyCodes.Okay)
        reject_slot = lambda: slot(self.MyCodes.Cancel)
        unknown_slot = lambda: slot(self.MyCodes.Unknown)

        button_box = QDialogButtonBox(QBtn)
        # button_box.accepted.connect(self.accept)
        # button_box.rejected.connect(self.reject)
        button_box.rejected.connect(reject_slot)
        button_box.accepted.connect(accept_slot)
        # button_box.accepted.connect(unknown_slot)
        # button_box.accepted.connect(lambda: slot(30))
        layout = QVBoxLayout()
        layout.setSpacing(10)
        message = QLabel("Something happened, is that Okay?")
        message.setMinimumHeight(50)
        # message.setAlignment(Qt.AlignmentFlag.AlignHCenter)
        layout.addWidget(message)
        layout.addWidget(button_box)
        self.setLayout(layout)


def main(argv: list[str] | None = None):
    app = QApplication(argv or [])
    window = MainWindow()
    window.show()
    return app.exec_()


if __name__ == "__main__":
    exit(main())
