from PyQt5.QtGui import QContextMenuEvent, QIcon, QMouseEvent, QPalette, QColor
from layouts import widgetize_layout, colors
from PyQt5.QtCore import Qt, QPoint
from PyQt5.QtWidgets import (
    QApplication,
    QMainWindow,
    QLayout,
    QVBoxLayout,
    QLabel,
    QAction,
    QMenu,
)


class Label(QLabel):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.setAlignment(Qt.AlignmentFlag.AlignCenter)
        palette = QPalette()
        palette.setColor(QPalette.ColorRole.Foreground, Qt.GlobalColor.green)
        palette.setColor(QPalette.ColorRole.Background, QColor("#191919"))
        self.setAutoFillBackground(True)
        self.setPalette(palette)
        self.setContextMenuPolicy(Qt.ContextMenuPolicy.CustomContextMenu)
        self.customContextMenuRequested.connect(self.custom_context_menu)

    # def contextMenuEvent(self, ev: QContextMenuEvent | None) -> None:
    #     self.custom_context_menu(ev.globalPos() if ev else self.pos())
    #     # self.custom_context_menu(QPoint(0, 0))
    #     return super().contextMenuEvent(ev)

    def custom_context_menu(self, pos: QPoint) -> None:
        menu = QMenu("Set Label Color")
        fore = QMenu("Foreground")
        back = QMenu("Background")

        def Action(color: str, role: QPalette.ColorRole):
            action = QAction(color.title(), self)

            def on_trigger():
                palette = self.palette()
                palette.setColor(role, QColor(color))
                self.setPalette(palette)

            action.triggered.connect(on_trigger)
            return action

        def Actions(colors: list[str], role: QPalette.ColorRole):
            for color in colors:
                yield Action(color, role)

        fore.addActions(Actions(colors, QPalette.ColorRole.Foreground))
        back.addActions(Actions(colors, QPalette.ColorRole.Background))
        menu.addMenu(fore)
        menu.addMenu(back)
        menu.exec(self.mapToGlobal(pos))

    def mouseMoveEvent(self, ev: QMouseEvent | None) -> None:
        self.setText("Mouse Moved")

    def mousePressEvent(self, ev: QMouseEvent | None) -> None:
        self.setText("Mouse Pressed")

    def mouseReleaseEvent(self, ev: QMouseEvent | None) -> None:
        self.setText("Mouse Released")

    def mouseDoubleClickEvent(self, a0: QMouseEvent | None) -> None:
        self.setText("Double Click")
        assert a0, a0
        self.customContextMenuRequested.emit(a0.pos())


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        icon = QIcon("./icons.ign/brands/apple.svg")
        self.setWindowIcon(icon)
        self.setCentralWidget(self.maincomp())

    @widgetize_layout
    def maincomp(self) -> QLayout:
        lout = QVBoxLayout()
        label = Label("PyQT5 Widgets.")
        lout.addWidget(label)
        return lout


def main(argv: list[str] | None = None):
    app = QApplication(argv or [])
    window = MainWindow()
    window.show()
    return app.exec()


if __name__ == "__main__":
    exit(main())
