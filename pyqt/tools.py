"""
PyQt5 offers Actions, Toolbars, Menus
and Tabs to nest widgets and dispatch
events across your application.

Tabs can be made manually using layouts
buttons attached to different actions
or using the standard provided tab widget
QTabWidget that has more features like
moving the tab actions.
"""

from layouts import Color, color_widgets, widgetize_layout
from PyQt5.QtCore import Qt, QSize
from PyQt5.QtGui import QIcon
from PyQt5.QtWidgets import (
    QStackedLayout,
    QApplication,
    QMainWindow,
    QHBoxLayout,
    QPushButton,
    QVBoxLayout,
    QStatusBar,
    QTabWidget,
    QComboBox,
    QCheckBox,
    QToolBar,
    QLayout,
    QWidget,
    QAction,
    QLabel,
    QMenu,
)


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("My PyQt5 App")
        self.setCentralWidget(self.qt_tools())
        self.setMinimumSize(400, 300)
        wicon = QIcon("./icons.ign/brands/paypal.svg")
        self.setWindowIcon(wicon)

    def qt_tools(self):
        return self.w_menus()
        return self.w_toolbar()
        return self.w_tab()
        return self.w_custom_tab()
        return self.w_qttab()

    def w_menus(self):
        menu = QMenu()
        label = QLabel("PyQt5 Menus Bitches.")
        label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        tbar = QToolBar()
        tbar.setIconSize(QSize(16, 16))
        tbar.setMovable(False)

        def pro_lang(lang: str, a: QAction):
            return lambda: (print("Language selected: %s" % lang.title()) or a.setDisabled(True))

        def browser_action(browser: str):
            return lambda: print("Browser clicked: %s" % browser.title())

        a1 = QAction(QIcon("./icons.ign/brands/chrome.svg"), "Chrome", self)
        a1.setStatusTip("Action with Chrome icon")
        a1.triggered.connect(browser_action("Chrome"))
        a2 = QAction(QIcon("./icons.ign/brands/opera.svg"), "Opera", self)
        a2.setStatusTip("Action with Opera icon")
        a2.triggered.connect(browser_action("Opera"))
        b1 = QAction(QIcon("./icons.ign/brands/python.svg"), "Python", self)
        b1.setStatusTip("Action with Python icon")
        b1.triggered.connect(pro_lang("python", b1))
        b1.setShortcut("Ctrl+P")
        b2 = QAction(QIcon("./icons.ign/brands/rust.svg"), "Rust", self)
        b2.setStatusTip("Action with Rust icon")
        b2.triggered.connect(pro_lang("rust", b2))
        b2.setShortcut("Ctrl+R")
        b3 = QAction(QIcon("./icons.ign/brands/java.svg"), "Java", self)
        b3.setStatusTip("Action with Java icon")
        b3.triggered.connect(pro_lang("java", b3))
        b3.setShortcut("Ctrl+J")
        tbar.addAction(a1)
        tbar.addSeparator()
        tbar.addAction(b1)
        menu = self.menuBar()
        assert menu is not None
        emenu = menu.addMenu("&Everything")
        assert emenu is not None
        emenu.addSection("Browsers")
        emenu.addActions([a1, a2])
        langs = emenu.addMenu("Languages")
        assert langs is not None
        # emenu.addSection("Languages")
        # emenu.addActions([b1, b2, b3])
        langs.addActions([b1, b2, b3])
        self.addToolBar(tbar)
        self.setStatusBar(QStatusBar())
        return label

    def w_toolbar(self):
        def Label():
            label = QLabel("Hello World")
            label.setStatusTip("This is the main content area.")
            A = Qt.AlignmentFlag
            label.setAlignment(A.AlignCenter)
            return label

        toolbar = QToolBar("My toolbar.")
        toolbar.setIconSize(QSize(24, 24))
        # toolbar.setToolButtonStyle(Qt.ToolButtonStyle.ToolButtonTextUnderIcon)
        icon = QIcon("./icons.ign/brands/rust.svg")
        toolbar.setStatusTip("Main toolbar.")
        # A QAction is like a button hence have
        # some few things in common like the type
        # of data send through signals to slots.
        action = QAction(icon, "Rusty Icon", self)
        action.setToolTip("This is a fucking tip")
        action.setStatusTip("This is my first QAction")

        def on_click(data):
            print("Action Clicked: %r" % data)

        action.triggered.connect(on_click)
        action.setCheckable(True)
        toolbar.addAction(action)
        toolbar.addSeparator()

        def tb_state(state: bool):
            print("Toggle Toolbar Visibility: %r" % state)
            if state:
                print("--> Hiding Toolbar")
                self.removeToolBar(toolbar)
                self.setCentralWidget(Cbox(True))
            else:
                print("--> Showing Toolbar")
                self.addToolBar(toolbar)
                self.setCentralWidget(Label())

        def Cbox(state: bool = False):
            cbox = QCheckBox()
            cbox.setChecked(state)
            cbox.setText("ToolBar")
            cbox.setStatusTip("Show/Hide Toolbar")
            cbox.toggled.connect(tb_state)
            return cbox

        toolbar.addWidget(Cbox())
        self.addToolBar(toolbar)
        sbar = QStatusBar(self)
        font = sbar.font()
        font.setBold(True)
        font.setItalic(True)
        font.setFamily("ArchitectsDaughter")
        font.setPointSize(10)
        sbar.setFont(font)
        sbar.setDisabled(True)
        sbar.setStatusTip("Main StatusBar")
        self.setStatusBar(sbar)
        return Label()

    @widgetize_layout
    def w_tab(self) -> QLayout:
        chooser = QComboBox()
        stack = QStackedLayout()

        custom_tab = self.w_custom_tab()
        qt_tab = self.w_qttab()

        # Order matters here
        stack.addWidget(custom_tab)
        stack.addWidget(qt_tab)
        chooser.addItems(["Custom Tab", "Qt Tab"])

        def on_select(index: int):
            stack.setCurrentIndex(index)

        chooser.currentIndexChanged.connect(on_select)
        chooser.setFixedHeight(30)

        lout = QVBoxLayout()
        lout.addLayout(stack)
        lout.addWidget(chooser)
        return lout

    def w_qttab(self) -> QWidget:
        tab = QTabWidget()
        # Where the tab will be placed
        # on the parent widget: North, top;
        # West, right; Left, east; and
        # South, bottom.
        P = QTabWidget.TabPosition
        tab.setTabPosition(P.North)
        tab.setMovable(True)
        for color in color_widgets():
            tab.addTab(color, color.widget_color)
        return tab

    @widgetize_layout
    def w_custom_tab(self) -> QLayout:
        SPACE = 2
        inner = QStackedLayout()
        btns = QHBoxLayout()
        btns.setSpacing(SPACE)

        def build_button(color: Color):
            text = color.widget_color.title()
            slot = lambda: inner.setCurrentWidget(color)
            btn = QPushButton(text)
            btn.clicked.connect(slot)
            return btn

        for color in color_widgets():
            inner.addWidget(color)
            btn = build_button(color)
            btns.addWidget(btn)

        lout = QVBoxLayout()
        lout.setSpacing(SPACE)
        lout.setContentsMargins(SPACE, SPACE, SPACE, SPACE)
        lout.addLayout(btns)
        lout.addLayout(inner)
        return lout


def main(argv: list[str] | None = None):
    app = QApplication(argv or [])
    window = MainWindow()
    window.show()
    return app.exec_()


if __name__ == "__main__":
    exit(main())
