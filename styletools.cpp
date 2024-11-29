#include "mainwindow.h"
QString radioButtonStyle = R"(
    QRadioButton {
        font: 12px;
        color: black;
        spacing: 10px;
    }
)";

// 设置输入框样式
QString lineEditStyle = R"(
    QLineEdit {
        font: bold 20px;
        color: black;
        background-color: white;
        border: 1px solid #A0A0A0;
        border-radius: 5px;
        text-align: center;
    }
)";
QString labelStyle = R"(
    QLabel {
        font: 12px;
        color: black;
        padding: 0 5px; /* 增加水平内间距，避免文字贴边 */
    }
)";

QString comboBoxStyle = R"(
    QComboBox {
        font: 12px;
        color: black;
        background-color: white; /* 白色背景 */
        border: 1px solid #A0A0A0; /* 浅灰色边框 */
        border-radius: 4px; /* 圆角 */
    }
)";
// 设置按钮、输入框、分组框、标签和单选按钮的样式
QString groupBoxStyle = R"(
    QGroupBox {
        background-color: #ecf0f1; /* 浅灰色背景 */
        border: 2px solid #bdc3c7; /* 边框颜色 */
        border-radius: 8px; /* 圆角 */
        padding: 10px; /* 内部间距 */
        margin-top: 15px; /* 标题与边框顶部的距离 */
    }
    QGroupBox::title {
        subcontrol-origin: margin; /* 标题相对于边框外边距 */
        subcontrol-position: top left; /* 标题位置：顶部左侧 */
        padding: 0 10px; /* 标题文字与边框的水平距离 */
        font: bold 14px; /* 加粗字体 */
        color: #2c3e50; /* 深灰色文字 */
        background-color: transparent; /* 标题背景透明 */
    }
    QChartView {
        border: 1px solid #bdc3c7; /* 边框颜色 */
        border-radius: 8px; /* 圆角 */
        background-color: #ffffff; /* 白色背景 */
        padding: 5px; /* 内部留白 */
    }
)";
