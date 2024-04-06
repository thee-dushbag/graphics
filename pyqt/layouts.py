"""
Layouts enable us to add more than
one widget to the main window. The
layouts are in charge of the arrangement
of the widgets they hold.
There are 4 basic layouts
    +---+-----------------+-------------------------------------+
    | x | QHBoxLayout     | Linear horizontal layout            |
    | x | QVBoxLayout     | Linear vertical layout              |
    | x | QGridLayout     | In indexible grid MxN               |
    | x | QStackedLayout  | Stacked(z) in front of one another  |
    +---+-----------------+-------------------------------------+
The first three being 2-dimensional (left, right)
while the last 3 dimensinal; as it allows
an extra direction, up (stacking).
"""

from PyQt5.QtWidgets import (
    QApplication,
    QMainWindow,
    QWidget,
    QVBoxLayout,
    QHBoxLayout,
    QLayout,
    QGridLayout,
    QStackedLayout,
    QComboBox,
)
from PyQt5.QtGui import QPalette, QColor
from PyQt5.QtCore import QMargins
import typing as t, functools as f


colors = [
    "crimson",
    "forestgreen",
    "greenyellow",
    "orangered",
    "goldenrod",
    "cornflowerblue",
    "tomato",
    "teal",
    "springgreen",
]
color_widgets = lambda: map(Color, colors)


class Color(QWidget):
    def __init__(self, color: str):
        super().__init__()
        self.setAutoFillBackground(True)
        palette = self.palette()
        palette.setColor(QPalette.ColorRole.Window, QColor(color))
        self.setPalette(palette)


@t.overload
def widgetize_layout(
    method: t.Callable[[QMainWindow], QLayout],
    *,
    spacing: int | None = None,
    margins: tuple[int, int, int, int] | QMargins | None = None,
) -> t.Callable[[QMainWindow], QWidget]: ...


@t.overload
def widgetize_layout(
    *,
    spacing: int | None = None,
    margins: tuple[int, int, int, int] | QMargins | None = None,
) -> t.Callable[
    [t.Callable[[QMainWindow], QLayout]], t.Callable[[QMainWindow], QWidget]
]: ...


def widgetize_layout(
    method: t.Callable[[QMainWindow], QLayout] | None = None,
    *,
    spacing: int | None = None,
    margins: tuple[int, int, int, int] | QMargins | None = None,
):
    if isinstance(margins, tuple):
        margins = QMargins(*margins)

    def widgetize(
        method: t.Callable[[QMainWindow], QLayout], /
    ) -> t.Callable[[QMainWindow], QWidget]:
        @f.wraps(method)
        def new_method(window: QMainWindow) -> QWidget:
            widget = QWidget()
            layout = method(window)
            if spacing is not None:
                layout.setSpacing(spacing)
            if margins is not None:
                layout.setContentsMargins(margins)
            widget.setLayout(layout)
            return widget

        return new_method

    return widgetize if method is None else widgetize(method)


def color_layout(LayoutT: type[QLayout]) -> t.Callable[[QMainWindow], QLayout]:
    def method(_: QMainWindow) -> QLayout:
        layout = LayoutT()
        for color in color_widgets():
            layout.addWidget(color)
        return layout

    method.__name__ = "l_" + LayoutT.__name__.lower()

    return method


def grid_indices(X: int, Y: int | None = None, /):
    return ((x, y) for x in range(X) for y in range(Y or X))


layouts: list[tuple[str, t.Callable[[QMainWindow], QLayout]]] = []


def register(
    method: t.Callable[[QMainWindow], QLayout]
) -> t.Callable[[QMainWindow], QLayout]:
    layouts.append((method.__name__, method))
    return method


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("My App")
        self.setMinimumSize(400, 300)
        self.setCentralWidget(self.setup_layout())

    @widgetize_layout(spacing=2, margins=(2, 2, 2, 2))
    def setup_layout(self) -> QLayout:
        return self.l_all()
        return self.l_cstack()
        return self.l_stack()
        return self.l_grid()
        return self.l_nested()
        return self.l_qvbox()
        return self.l_qhbox()

    def l_all(self):
        lout = QVBoxLayout()
        chooser = QComboBox()
        chooser.addItems(l[0] for l in layouts)
        chooser.setFixedHeight(30)
        stack = QStackedLayout()
        for _, method in layouts:
            wrapped = widgetize_layout(method, margins=(1, 1, 1, 1))
            stack.addWidget(wrapped(self))

        def on_select(index: int):
            stack.setCurrentIndex(index)

        chooser.currentIndexChanged.connect(on_select)
        lout.addLayout(stack)
        lout.addWidget(chooser)
        lout.setContentsMargins(0, 0, 0, 2)
        lout.setSpacing(0)
        return lout

    @register
    def l_stack(self):
        lout = QStackedLayout()
        for color in color_widgets():
            lout.addWidget(color)
        lout.setCurrentIndex(4)
        return lout

    @register
    def l_cstack(self):
        lout = QVBoxLayout()
        chooser = QComboBox()
        stack = QStackedLayout()
        for color in color_widgets():
            stack.addWidget(color)
        lout.addLayout(stack)
        lout.addWidget(chooser)
        chooser.addItems(colors)

        def on_select(index: int):
            stack.setCurrentIndex(index)

        chooser.currentIndexChanged.connect(on_select)
        chooser.setFixedHeight(30)
        return lout

    @register
    def l_grid(self):
        lout = QGridLayout()
        # Both of these have Nine items hence zippable.
        colors = color_widgets()
        slot = grid_indices(3)
        for (x, y), color in zip(slot, colors):
            lout.addWidget(color, x, y)
        return lout

    @register
    def l_nested(self):
        inner = QGridLayout()
        slots = grid_indices(2)
        for (x, y), color in zip(slots, map(Color, colors[:4])):
            inner.addWidget(color, x, y)
        middle = QHBoxLayout()
        for color in map(Color, colors[4:6]):
            middle.addWidget(color)
        middle.addLayout(inner)
        middle2 = QHBoxLayout()
        for color in map(Color, colors[6:8]):
            middle2.addWidget(color)
        # Space between widgets
        # inner.setSpacing(20)
        outer = QVBoxLayout()
        outer.addLayout(middle)
        outer.addLayout(middle2)
        for color in map(Color, colors[8:]):
            outer.addWidget(color)
        margin = QMargins(2, 4, 8, 10)
        # Space around the layout
        # outer.setContentsMargins(margin)
        return outer

    # Strictly linear layout types
    l_qvbox = register(color_layout(QVBoxLayout))
    l_qhbox = register(color_layout(QHBoxLayout))


def main(argv: list[str] | None = None):
    app = QApplication(argv or [])
    window = MainWindow()
    window.show()
    return app.exec_()


if __name__ == "__main__":
    exit(main())
