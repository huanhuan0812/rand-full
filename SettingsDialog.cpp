#include "SettingsDialog.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("⚙️ 随机数设置");
    setMinimumSize(700, 550); // 增加宽度以容纳更大的SpinBox
    setStyleSheet(R"(
        QDialog {
            background-color: #f8f9fa;
            color: #212529;
            font-family: 'Segoe UI', 'Microsoft YaHei', sans-serif;
        }
        QGroupBox {
            font-weight: bold;
            font-size: 13px;
            border: 2px solid #ced4da;
            border-radius: 12px;
            margin-top: 1.5ex;
            padding-top: 15px;
            background-color: white;
            color: #4a90e2;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 15px;
            padding: 0 8px 0 8px;
            background-color: white;
        }
        QLabel {
            color: #495057;
            font-weight: 500;
        }
        QSpinBox {
            background-color: white;
            border: 2px solid #ced4da;
            border-radius: 6px;
            padding: 8px;
            color: #212529;
            font-size: 16px;
            min-width: 100px;
            min-height: 20px; /* 增加最小高度 */
            selection-background-color: #4a90e2;
            selection-color: white;
        }
        QSpinBox:focus {
            border-color: #86b7fe;
            outline: 0;
            box-shadow: 0 0 0 0.25rem rgba(13, 110, 253, 0.25);
        }

        QSpinBox::up-button{
            width: 20px;
            border-left: 1px solid #ced4da;
            radius: 5px;
            font-size: 12px;
            height: 20px;
        }
        QSpinBox::down-button{
            width: 20px;
            border-left: 1px solid #ced4da;
            radius: 5px;
            font-size: 12px;
            height: 20px;
            image: url(./chevron-down.svg);
        }

        QSpinBox::up-button{
            width: 20px;
            image: url(./chevron-up.svg);
            height: 20px;

        }

        QSpinBox::up-button:hover, QSpinBox::down-button:hover {
            background-color: #e9ecef;
        }

        QLineEdit {
            background-color: white;
            border: 2px solid #ced4da;
            border-radius: 6px;
            padding: 8px;
            color: #212529;
            font-size: 16px;
            min-height: 20px; /* 与SpinBox保持一致的高度 */
            selection-background-color: #4a90e2;
            selection-color: white;
        }
        QLineEdit:focus {
            border-color: #86b7fe;
            outline: 0;
            box-shadow: 0 0 0 0.25rem rgba(13, 110, 253, 0.25);
        }
        QCheckBox {
            color: #495057;
            font-weight: 500;
            spacing: 8px;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
            border: 2px solid #adb5bd;
            border-radius: 4px;
            background: white;
        }
        QCheckBox::indicator:checked {
            background: #4a90e2;
            border-color: #357abd;
            image: url(./check.svg);
        }
        QCheckBox::indicator:hover {
            border-color: #6c757d;
        }
        QScrollArea {
            border: 2px solid #ced4da;
            border-radius: 8px;
            background-color: white;
        }
        QScrollBar::up-arrow:vertical{
            width: 15px;
            height: 15px;
            image: url(./chevron-up.svg);
        }
        QScrollBar::down-arrow:vertical{
            width: 15px;
            height: 15px;
            image: url(./chevron-down.svg);
        }
        QScrollBar::up-arrow:hover, QScrollBar::down-arrow:hover {
            background-color: #e9ecef;
        }
        QScollBar::vertical {
            border: none;
            background: #f8f9fa;
            width: 14px;
            margin: 0px;
            border-radius: 7px;
        }
        QScrollBar::handle:vertical {
            background: #adb5bd;
            border-radius: 7px;
        }
        QScrollBar::handle:vertical:hover {
            background: #6c757d;
        }
        QScrollArea QWidget {
            background-color: white;
        }
    )");



    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    // 基本设置
    QGroupBox *basicGroup = new QGroupBox("📊 基本设置");
    QVBoxLayout *basicLayout = new QVBoxLayout(basicGroup);
    basicLayout->setSpacing(15);
    basicLayout->setContentsMargins(20, 20, 20, 20);

    QHBoxLayout *rangeLayout = new QHBoxLayout();
    rangeLayout->setSpacing(15);

    rangeLayout->addWidget(new QLabel("最小值:"));
    minSpinBox = new QSpinBox();
    minSpinBox->setRange(-999999, 999999);
    minSpinBox->setMinimumWidth(120);
    minSpinBox->setMinimumHeight(20); // 设置最小高度
    rangeLayout->addWidget(minSpinBox);

    rangeLayout->addSpacing(20);
    rangeLayout->addWidget(new QLabel("最大值:"));
    maxSpinBox = new QSpinBox();
    maxSpinBox->setRange(-999999, 999999);
    maxSpinBox->setMinimumWidth(120);
    maxSpinBox->setMinimumHeight(20); // 设置最小高度
    rangeLayout->addWidget(maxSpinBox);

    rangeLayout->addSpacing(20);
    rangeLayout->addWidget(new QLabel("生成数量:"));
    countSpinBox = new QSpinBox();
    countSpinBox->setRange(1, 1000);
    countSpinBox->setMinimumWidth(120);
    countSpinBox->setMinimumHeight(20); // 设置最小高度

    rangeLayout->addWidget(countSpinBox);

    rangeLayout->addStretch();
    basicLayout->addLayout(rangeLayout);

    // 排除设置
    QGroupBox *exclusionGroup = new QGroupBox("🚫 数字排除设置");
    QVBoxLayout *exclusionLayoutMain = new QVBoxLayout(exclusionGroup);
    exclusionLayoutMain->setSpacing(15);
    exclusionLayoutMain->setContentsMargins(20, 20, 20, 20);

    enableExclusionCheckBox = new QCheckBox("启用数字排除");
    exclusionLayoutMain->addWidget(enableExclusionCheckBox);

    exclusionLayoutMain->addWidget(new QLabel("输入要排除的数字(用逗号分隔):"));
    exclusionLineEdit = new QLineEdit();
    exclusionLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9,-]*"), this));
    exclusionLineEdit->setMinimumHeight(20); // 设置最小高度
    exclusionLayoutMain->addWidget(exclusionLineEdit);

    exclusionLayoutMain->addWidget(new QLabel("或从列表中选择要排除的数字:"));

    // 创建滚动区域和复选框容器
    exclusionScrollArea = new QScrollArea();
    exclusionScrollArea->setWidgetResizable(true);

    exclusionContainer = new QWidget();
    exclusionLayout = new QGridLayout(exclusionContainer);
    exclusionLayout->setSpacing(8);
    exclusionLayout->setContentsMargins(10, 10, 10, 10);
    exclusionButtonGroup = new QButtonGroup(this);
    exclusionButtonGroup->setExclusive(false);

    exclusionScrollArea->setWidget(exclusionContainer);
    exclusionLayoutMain->addWidget(exclusionScrollArea);

    // 按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("✅ 确定");
    QPushButton *cancelButton = new QPushButton("❌ 取消");

    // 设置按钮样式 - 颜色变浅
    QString buttonStyle = R"(
        QPushButton {
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                stop: 0 #6c9bd2, stop: 1 #4a7bb0);
            color: white;
            border: 2px solid #8fb4e2;
            border-radius: 8px;
            padding: 10px 24px;
            font-weight: bold;
            font-size: 13px;
            min-width: 80px;
            min-height: 25px; /* 按钮高度 */
        }
        QPushButton:hover {
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                stop: 0 #5a8ac2, stop: 1 #3a6ba0);
            border-color: #7aa4d2;
        }
        QPushButton:pressed {
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                stop: 0 #4a7bb0, stop: 1 #2a5b90);
        }
    )";

    QString cancelButtonStyle = R"(
        QPushButton {
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                stop: 0 #8fa0b0, stop: 1 #6c7d8c);
            color: white;
            border: 2px solid #b0bec5;
            border-radius: 8px;
            padding: 10px 24px;
            font-weight: bold;
            font-size: 16px;
            min-width: 80px;
            min-height: 25px; /* 按钮高度 */
        }
        QPushButton:hover {
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                stop: 0 #7a8a9a, stop: 1 #5a6a7a);
            border-color: #90a4ae;
        }
        QPushButton:pressed {
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                stop: 0 #6c7d8c, stop: 1 #4a5a6a);
        }
    )";

    okButton->setStyleSheet(buttonStyle);
    cancelButton->setStyleSheet(cancelButtonStyle);

    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addSpacing(15);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addWidget(basicGroup);
    mainLayout->addWidget(exclusionGroup, 1);
    mainLayout->addLayout(buttonLayout);

    // 连接信号和槽
    connect(minSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SettingsDialog::updateRangeLimits);
    connect(maxSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &SettingsDialog::updateRangeLimits);
    connect(enableExclusionCheckBox, &QCheckBox::toggled, this, &SettingsDialog::toggleExclusionCheckboxes);
    connect(exclusionLineEdit, &QLineEdit::textChanged, this, &SettingsDialog::updateExclusionFromText);
    connect(okButton, &QPushButton::clicked, this, &SettingsDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    // 初始化
    updateExclusionCheckboxes();
}

void SettingsDialog::setSettings(int min, int max, int count, bool exclusionEnabled, const QList<int> &excludedNumbers)
{
    minSpinBox->setValue(min);
    maxSpinBox->setValue(max);
    countSpinBox->setValue(count);
    enableExclusionCheckBox->setChecked(exclusionEnabled);
    
    // 设置排除的数字
    QStringList numStrs;
    for (int num : excludedNumbers) {
        numStrs.append(QString::number(num));
    }
    exclusionLineEdit->setText(numStrs.join(","));
    
    // 更新复选框状态
    QList<QAbstractButton*> buttons = exclusionButtonGroup->buttons();
    for (QAbstractButton *button : buttons) {
        QCheckBox *checkbox = qobject_cast<QCheckBox*>(button);
        if (checkbox) {
            int num = exclusionButtonGroup->id(button);
            checkbox->setChecked(excludedNumbers.contains(num));
        }
    }
    
    toggleExclusionCheckboxes(exclusionEnabled);
    updateExclusionCheckboxes();
}

void SettingsDialog::updateRangeLimits()
{
    // 确保最小值不超过最大值
    if (minSpinBox->value() >= maxSpinBox->value()) {
        minSpinBox->setValue(maxSpinBox->value() - 1);
    }
    
    // 更新排除数字标签页中的复选框
    updateExclusionCheckboxes();
}

void SettingsDialog::updateExclusionCheckboxes()
{
    // 清除现有复选框
    QLayoutItem *child;
    while ((child = exclusionLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    
    int min = minSpinBox->value();
    int max = maxSpinBox->value();
    
    if (min >= max) {
        return;
    }
    
    // 创建新的复选框
    int row = 0;
    int col = 0;
    const int maxCols = 10;
    
    for (int i = min; i <= max; ++i) {
        QCheckBox *checkbox = new QCheckBox(QString::number(i));
        checkbox->setChecked(false);
        checkbox->setEnabled(enableExclusionCheckBox->isChecked());
        
        connect(checkbox, &QCheckBox::toggled, this, &SettingsDialog::updateExclusionFromCheckboxes);
        
        exclusionLayout->addWidget(checkbox, row, col);
        exclusionButtonGroup->addButton(checkbox, i);
        
        col++;
        if (col >= maxCols) {
            col = 0;
            row++;
        }
    }
}

void SettingsDialog::toggleExclusionCheckboxes(bool checked)
{
    QList<QAbstractButton*> buttons = exclusionButtonGroup->buttons();
    for (QAbstractButton *button : buttons) {
        button->setEnabled(checked);
    }
    
    exclusionLineEdit->setEnabled(checked);
}

void SettingsDialog::updateExclusionFromText()
{
    if (!enableExclusionCheckBox->isChecked()) {
        return;
    }
    
    // 更新复选框状态
    QString text = exclusionLineEdit->text();
    QStringList numbers = text.split(',', Qt::SkipEmptyParts);
    QList<int> excludedNumbers;
    
    for (const QString &numStr : numbers) {
        bool ok;
        int num = numStr.trimmed().toInt(&ok);
        if (ok) {
            excludedNumbers.append(num);
        }
    }
    
    QList<QAbstractButton*> buttons = exclusionButtonGroup->buttons();
    for (QAbstractButton *button : buttons) {
        QCheckBox *checkbox = qobject_cast<QCheckBox*>(button);
        if (checkbox) {
            int num = exclusionButtonGroup->id(button);
            checkbox->setChecked(excludedNumbers.contains(num));
        }
    }
}

void SettingsDialog::updateExclusionFromCheckboxes()
{
    if (!enableExclusionCheckBox->isChecked()) {
        return;
    }
    
    // 更新文本输入
    QList<int> excludedNumbers;
    QList<QAbstractButton*> buttons = exclusionButtonGroup->buttons();
    for (QAbstractButton *button : buttons) {
        QCheckBox *checkbox = qobject_cast<QCheckBox*>(button);
        if (checkbox && checkbox->isChecked()) {
            int num = exclusionButtonGroup->id(button);
            excludedNumbers.append(num);
        }
    }
    
    QStringList numStrs;
    for (int num : excludedNumbers) {
        numStrs.append(QString::number(num));
    }
    exclusionLineEdit->setText(numStrs.join(","));
}

QList<int> SettingsDialog::getExcludedNumbers() const
{
    QList<int> excludedNumbers;
    
    if (!enableExclusionCheckBox->isChecked()) {
        return excludedNumbers;
    }
    
    // 从文本输入获取排除的数字
    QString text = exclusionLineEdit->text();
    if (!text.isEmpty()) {
        QStringList numbers = text.split(',', Qt::SkipEmptyParts);
        for (const QString &numStr : numbers) {
            bool ok;
            int num = numStr.trimmed().toInt(&ok);
            if (ok) {
                excludedNumbers.append(num);
            }
        }
    }
    
    // 从复选框获取排除的数字
    QList<QAbstractButton*> buttons = exclusionButtonGroup->buttons();
    for (QAbstractButton *button : buttons) {
        QCheckBox *checkbox = qobject_cast<QCheckBox*>(button);
        if (checkbox && checkbox->isChecked()) {
            int num = exclusionButtonGroup->id(button);
            if (!excludedNumbers.contains(num)) {
                excludedNumbers.append(num);
            }
        }
    }
    
    // 排序并去重
    std::sort(excludedNumbers.begin(), excludedNumbers.end());
    excludedNumbers.erase(std::unique(excludedNumbers.begin(), excludedNumbers.end()), excludedNumbers.end());
    
    return excludedNumbers;
}

void SettingsDialog::accept()
{
    emit settingsChanged();
    QDialog::accept();
}