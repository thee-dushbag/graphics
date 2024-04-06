"""
In Qt, a wigdet is the name given to
a component in the UI that a user can
interact with.
Some of them are
    +---+------------------+-------------------------------------+
    | x | QCheckBox        | A checkbox                          |
    | x | QComboBox        | A dropdown list box                 |
    |   | QTimeEdit        | For editing times                   |
    |   | QDateEdit        | For editing dates                   |
    |   | QDateTimeEdit    | For editing times and dates         |
    | x | QDial            | Rotational dial                     |
    | x | QDoubleSpinBox   | A number spinner for floats         |
    |   | QFontComboBox    | A list of fonts                     |
    |   | QLCDNumber       | A quite ugly LCD display            |
    | x | QLabel           | Just a label, not interactive       |
    | x | QLineEdit        | Enter a line of text                |
    |   | QProgressBar     | A progressbar                       |
    |   | QPushButton      | A button                            |
    | x | QListWidget      | A list of items                     |
    |   | QRadioButton     | A group with only one active choice |
    | x | QSlider          | A slider                            |
    | x | QSpinBox         | An integer spinner                  |
    +---+------------------+-------------------------------------+
There are a lot more, checkout Qt documentation.
"""

from PyQt5.QtWidgets import (
    QApplication,
    QMainWindow,
    QLabel,
    QCheckBox,
    QComboBox,
    QListWidget,
    QListWidgetItem,
    QLineEdit,
    QSpinBox,
    QDoubleSpinBox,
    QSlider,
    QDial,
)
from PyQt5.QtGui import QFont, QPixmap
from PyQt5.QtCore import Qt


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Qt Widgets")
        self.setMinimumSize(400, 300)
        self.setCentralWidget(self.widget())

    def widget(self):
        return self.w_dial()
        return self.w_slider()
        return self.w_spinbox()
        return self.w_lineedit()
        return self.w_listbox()
        return self.w_combobox()
        return self.w_checkbox()
        return self.w_label()

    def w_dial(self):
        dial = QDial()
        dial.setRange(-10, 10)
        dial.setSingleStep(2)

        def on_change(value: float):
            print("Value: %r" % value)

        def on_move(p):
            print("Position: %r" % p)

        dial.sliderPressed.connect(lambda: print("Down"))
        dial.sliderReleased.connect(lambda: print("Up"))
        dial.sliderMoved.connect(on_move)
        dial.valueChanged.connect(on_change)
        return dial

    def w_slider(self):
        # slide = QSlider(Qt.Orientation.Vertical)
        slide = QSlider(Qt.Orientation.Horizontal)
        slide.setRange(-10, 10)
        slide.setSingleStep(2)

        def on_change(value: float):
            print("Value: %r" % value)

        def on_move(p):
            print("Position: %r" % p)

        slide.sliderPressed.connect(lambda: print("Down"))
        slide.sliderReleased.connect(lambda: print("Up"))
        slide.sliderMoved.connect(on_move)
        slide.valueChanged.connect(on_change)
        return slide

    def w_spinbox(self):
        # spin = QSpinBox()
        spin = QDoubleSpinBox()
        # spin.setMaximum(10)
        # spin.setMinimum(-10)
        spin.setRange(-10, 10)
        spin.setPrefix("£")
        spin.setSuffix("$")
        # spin.setSingleStep(2)
        spin.setSingleStep(0.1)

        def on_change(value: str | int):
            match value:
                case str():
                    print("String: %r" % value)
                case int():
                    print("Number: %r" % value)

        # spin.valueChanged[int].connect(on_change)
        spin.valueChanged[float].connect(on_change)
        spin.valueChanged[str].connect(on_change)

        return spin

    def w_lineedit(self):
        line = QLineEdit()
        # line.setMaxLength(10)
        line.setPlaceholderText("Hello from LineEdit!")
        line.setText("Ola Mr. Sherlock?")

        def on_return():
            print("Text: %r" % line.text())
            line.setReadOnly(not line.isReadOnly())

        def on_selection():
            print("Selected: %r" % line.selectedText())

        def on_edit():
            print("Edit: %r" % line.text())

        def on_change():
            print("Change: %r" % line.text())

        # line.setReadOnly(True)
        line.selectionChanged.connect(on_selection)
        line.returnPressed.connect(on_return)
        line.textChanged.connect(on_change)
        line.textEdited.connect(on_edit)
        # line.setPlaceholderText("Your IP Address.")
        # line.setInputMask("000.000.000.000;_")
        return line

    def w_listbox(self):
        listb = QListWidget()
        listb.addItems(["Simon", "Nganga", "Njoroge"])
        last_item: QListWidgetItem | None = None

        def on_select(item: QListWidgetItem):
            nonlocal last_item
            item.setText(item.text().upper())
            if last_item is not None:
                last_item.setText(last_item.text().title())
            last_item = item

        on_text = lambda text: print("Selected: %r" % text.title())

        listb.currentTextChanged.connect(on_text)
        listb.currentItemChanged.connect(on_select)
        return listb

    def w_combobox(self):
        combo = QComboBox()
        combo.addItems(["Simon", "Nganga"])
        combo.addItem("Njoroge")
        combo.setMaxCount(5)
        combo.setFixedHeight(50)

        def chosen(name: str):
            print("Chosen Name: %r" % name)

        combo.currentTextChanged.connect(chosen)
        combo.setEditable(True)
        I = QComboBox.InsertPolicy
        combo.setInsertPolicy(I.InsertAlphabetically)
        return combo

    def w_checkbox(self):
        check = QCheckBox("My CheckBox")

        def on_click(state: bool):
            print("Checked?", state)

        # check.clicked.connect(on_click)
        # check.setCheckState(Qt.CheckState.Checked)
        # check.setCheckState(Qt.CheckState.PartiallyChecked)
        # check.setChecked(True)
        # check.setTristate(True)
        check.stateChanged.connect(on_click)
        return check

    def w_label(self):
        label = QLabel("This is a label")
        label.setText("New Label Text")
        # font = label.font()
        image = QPixmap("./image.png")
        font = QFont("ArchitectsDaughter")
        font.setPointSize(30)
        label.setFont(font)
        A = Qt.AlignmentFlag
        label.setAlignment(A(A.AlignHCenter | A.AlignVCenter))
        # label.setAlignment(A(A.AlignRight | A.AlignTop))
        # label.setFixedSize(QSize(500, 100))
        label.setPixmap(image)
        # Scale the image in the label object
        label.setScaledContents(True)
        return label


def main(argv: list[str] | None = None):
    app = QApplication(argv or [])
    window = MainWindow()
    window.show()
    return app.exec_()


if __name__ == "__main__":
    exit(main())
