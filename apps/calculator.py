from PyQt5.QtWidgets import (
    QApplication,
    QMainWindow,
    QGridLayout,
    QPushButton,
    QWidget,
    QVBoxLayout,
    QHBoxLayout,
    QLabel,
)
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QIcon
from itertools import chain
from pathlib import Path


def grid_pos(rows: int, columns: int):
    for row in range(rows):
        for column in range(columns):
            yield row, column


class KeyPad(QGridLayout):
    def __init__(self, sete, gete):
        super().__init__()
        self.set_expr = sete
        self.get_expr = gete
        for btn, (x, y) in self.number_keys():
            self.addWidget(btn, x, y)

    def create_num(self, num: str):
        btn = QPushButton(num)

        def on_click():
            expr = self.get_expr()
            self.set_expr(expr + num)

        btn.clicked.connect(on_click)
        return btn

    def number_keys(self):
        keys = chain(range(1, 10), ["(", 0, ")"])
        npos = zip(map(str, keys), grid_pos(4, 3))
        for number, pos in npos:
            yield self.create_num(number), pos


class Operators(QVBoxLayout):
    def __init__(self, set_expr, get_expr):
        super().__init__()
        self.get_expr = get_expr
        self.set_expr = set_expr
        for op in self.operator_keys():
            self.addWidget(op)

    def create_op(self, op: str):
        btn = QPushButton(op)

        def on_click():
            expr = self.get_expr()
            self.set_expr(expr + op)

        btn.clicked.connect(on_click)
        return btn

    def operator_keys(self):
        ops = ["*", "+", "-", "/"]
        for op in ops:
            yield self.create_op(op)


class SpecialKeys(QHBoxLayout):
    def __init__(self, sete, gete, seta):
        super().__init__()
        self.set_expr = sete
        self.get_expr = gete
        self.set_ans = seta
        for ky in self.special_keys():
            self.addWidget(ky)

    def clr_btn(self):
        btn = QPushButton("clr")

        def on():
            self.set_expr("")
            self.set_ans("")

        btn.clicked.connect(on)
        return btn

    def del_btn(self):
        btn = QPushButton("del")

        def on():
            if expr := self.get_expr():
                self.set_expr(expr[:-1])

        btn.clicked.connect(on)
        return btn

    def eval_btn(self):
        btn = QPushButton("=")

        def on():
            expr = self.get_expr()
            try:
                ans = eval(expr)
            except ZeroDivisionError:
                ans = "ZeroDivision"
            except Exception:
                ans = "SyntaxError"
            self.set_ans(ans)

        btn.clicked.connect(on)
        return btn

    def special_keys(self):
        yield self.clr_btn()
        yield self.del_btn()
        yield self.eval_btn()


class CalculatorButtons(QVBoxLayout):
    def __init__(self, set_expr, get_expr, set_ans):
        super().__init__()
        top = QHBoxLayout()
        top.addLayout(KeyPad(set_expr, get_expr))
        top.addLayout(Operators(set_expr, get_expr))
        self.addLayout(top)
        self.addLayout(SpecialKeys(set_expr, get_expr, set_ans))


class Calculator(QVBoxLayout):
    def __init__(self):
        super().__init__()
        expr = QLabel()
        ans = QLabel()
        font = expr.font()
        font.setPointSize(15)
        expr.setFont(font)
        ans.setFont(font)
        ans.setAlignment(Qt.AlignmentFlag.AlignRight)
        lout = QVBoxLayout()
        lout.addWidget(expr)
        lout.addWidget(ans)
        self.addLayout(lout)
        self.addLayout(CalculatorButtons(self.set_expr, self.get_expr, self.set_ans))
        self._expr_input = expr
        self._expr_output = ans

    def get_expr(self):
        return self._expr_input.text()

    def set_expr(self, expr: str):
        self._expr_input.setText(expr)

    def set_ans(self, ans):
        self._expr_output.setText(str(ans))


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Calculator")
        path = Path(__file__).parent.parent
        icon = QIcon(str(path / "pyqt" / "image.png"))
        self.setWindowIcon(icon)
        wid = QWidget()
        wid.setLayout(Calculator())
        self.setCentralWidget(wid)
        self.setFixedSize(300, 230)


def main(argv: list[str] | None = None):
    app = QApplication(argv or [])
    window = MainWindow()
    window.show()
    return app.exec()


if __name__ == "__main__":
    exit(main())
