// RandomNumberGenerator.cpp
#include "RandomNumberGenerator.h"
#include <QRandomGenerator>
#include <QScrollBar>
#include <QClipboard>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QStyleFactory>

RandomNumberGenerator::RandomNumberGenerator(QWidget *parent)
    : QWidget(parent), settingsDialog(nullptr)
{
    setupUI();
    loadSettings();

    // 创建设置对话框
    settingsDialog = new SettingsDialog(this);
    settingsDialog->setSettings(minValue, maxValue, countValue, exclusionEnabled, excludedNumbers);

    connect(settingsDialog, &SettingsDialog::settingsChanged, this, [this]() {
        minValue = settingsDialog->getMinValue();
        maxValue = settingsDialog->getMaxValue();
        countValue = settingsDialog->getCountValue();
        exclusionEnabled = settingsDialog->isExclusionEnabled();
        excludedNumbers = settingsDialog->getExcludedNumbers();
        saveSettings();
        updateResultDisplay();
    });
}

RandomNumberGenerator::~RandomNumberGenerator()
{
    saveSettings();
    delete settingsDialog;
}

void RandomNumberGenerator::setupUI()
{
    // 设置窗口属性
    setWindowTitle("🎲 随机数生成器");
    setMinimumSize(600, 500);
    setStyleSheet(R"(
        QWidget {
            background-color: #f8f9fa;
            color: #212529;
            font-family: 'Segoe UI', 'Microsoft YaHei', sans-serif;
        }
    )");

    // 创建控件
    settingsButton = new QPushButton("⚙️ 设置");
    generateButton = new QPushButton("🎯 生成随机数");
    copyButton = new QPushButton("📋 复制结果");

    // 设置按钮样式 - 颜色变浅
    QString buttonStyle = R"(
        QPushButton {
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                stop: 0 #8fa0b0, stop: 1 #6c7d8c);
            color: white;
            border: 2px solid #b0bec5;
            border-radius: 8px;
            padding: 10px 20px;
            font-weight: bold;
            font-size: 15px;
            min-width: 100px;
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
        QPushButton:disabled {
            background: #d1d9e0;
            color: #8a99a8;
        }
    )";

    settingsButton->setStyleSheet(buttonStyle);
    generateButton->setStyleSheet(buttonStyle);
    copyButton->setStyleSheet(buttonStyle);

    resultTextEdit = new QTextEdit();
    resultTextEdit->setReadOnly(true);
    resultTextEdit->setStyleSheet(R"(
        QTextEdit {
            background-color: white;
            border: 2px solid #dee2e6;
            border-radius: 8px;
            padding: 15px;
            font-family: 'Consolas', 'Monospace';
            font-size: 22px;
            color: #000000;
            selection-background-color: #4a90e2;
            selection-color: white;
        }
        QTextEdit:focus {
            border-color: #86b7fe;
            outline: 0;
            box-shadow: 0 0 0 0.25rem rgba(13, 110, 253, 0.25);
        }
        QScrollBar:vertical {
            border: none;
            background: #f8f9fa;
            width: 14px;
            margin: 0px;
            border-radius: 7px;
        }
        QScrollBar::handle:vertical {
            background: #adb5bd;
            border-radius: 7px;
            min-height: 30px;
        }
        QScrollBar::handle:vertical:hover {
            background: #6c757d;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
        QScrollBar:horizontal {
            border: none;
            background: #f8f9fa;
            height: 14px;
            margin: 0px;
            border-radius: 7px;
        }
        QScrollBar::handle:horizontal {
            background: #adb5bd;
            border-radius: 7px;
            min-width: 30px;
        }
        QScrollBar::handle:horizontal:hover {
            background: #6c757d;
        }
    )");

    infoLabel = new QLabel("点击\"设置\"按钮配置随机数参数");
    infoLabel->setStyleSheet(R"(
        QLabel {
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                stop: 0 #e9ecef, stop: 1 #dee2e6);
            border: 2px solid #ced4da;
            border-radius: 10px;
            padding: 12px;
            font-weight: bold;
            font-size: 16px;
            color: #4a90e2;
        }
    )");
    infoLabel->setAlignment(Qt::AlignCenter);

    // 连接信号和槽
    connect(settingsButton, &QPushButton::clicked, this, &RandomNumberGenerator::showSettingsDialog);
    connect(generateButton, &QPushButton::clicked, this, &RandomNumberGenerator::generateRandomNumbers);
    connect(copyButton, &QPushButton::clicked, this, &RandomNumberGenerator::copyToClipboard);

    // 布局设置
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);
    buttonLayout->addWidget(settingsButton);
    buttonLayout->addWidget(generateButton);
    buttonLayout->addWidget(copyButton);
    buttonLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(25, 25, 25, 25);

    mainLayout->addWidget(infoLabel);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(resultTextEdit, 1);

    setLayout(mainLayout);
}


void RandomNumberGenerator::generateRandomNumbers()
{
    if (minValue >= maxValue) {
        QMessageBox::warning(this, "输入错误", "最大值必须大于最小值");
        return;
    }

    // 获取排除的数字
    QList<int> excluded = exclusionEnabled ? excludedNumbers : QList<int>();

    // 计算可用数字范围
    int availableNumbers = (maxValue - minValue + 1) - excluded.size();
    if (availableNumbers <= 0) {
        QMessageBox::warning(this, "错误", "所有可能的数字都被排除了！");
        return;
    }

    if (countValue > availableNumbers) {
        QMessageBox::warning(this, "错误",
            QString("请求的数量(%1)超过了可用数字的数量(%2)").arg(countValue).arg(availableNumbers));
        return;
    }

    // 生成随机数
    currentNumbers.clear();

    for (int i = 0; i < countValue; ++i) {
        int randomNumber;
        do {
            randomNumber = QRandomGenerator::global()->bounded(minValue, maxValue + 1);
        } while (isNumberExcluded(randomNumber) || currentNumbers.contains(randomNumber));

        currentNumbers.append(randomNumber);
    }

    updateResultDisplay();
    saveToHistoryFile(); // 自动保存到历史文件
}

void RandomNumberGenerator::copyToClipboard()
{
    QString text = resultTextEdit->toPlainText();
    if (!text.isEmpty()) {
        QApplication::clipboard()->setText(text);
        QMessageBox::information(this, "复制成功", "结果已复制到剪贴板");
    }
}

// 删除了 saveToFile() 方法

void RandomNumberGenerator::saveToHistoryFile()
{
    if (currentNumbers.isEmpty()) {
        return;
    }

    QString historyFilePath = QDir::current().filePath("history.txt");
    QFile file(historyFilePath);

    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << QString("=").repeated(50) << "\n";
        writeRandomNumbersToStream(out, false);
        file.close();
    }
}

void RandomNumberGenerator::writeRandomNumbersToStream(QTextStream &out, bool includeHeader)
{
    if (includeHeader) {
        out << "随机数生成记录\n";
        out << "生成时间: " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n";
        out << "范围: " << minValue << " - " << maxValue << "\n";
        out << "数量: " << countValue << "\n";
        out << "排除数字: ";

        if (exclusionEnabled && !excludedNumbers.isEmpty()) {
            for (int i = 0; i < excludedNumbers.size(); ++i) {
                out << excludedNumbers[i];
                if (i < excludedNumbers.size() - 1) {
                    out << ", ";
                }
            }
        } else {
            out << "无";
        }
        out << "\n\n";
    } else {
        out << "生成时间: " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n";
        out << "范围: " << minValue << " - " << maxValue << "\n";
        out << "数量: " << countValue << "\n";
    }

    out << "生成的随机数:\n";
    for (int i = 0; i < currentNumbers.size(); ++i) {
        out << currentNumbers[i];
        if (i < currentNumbers.size() - 1) {
            out << ", ";
        }
        if (i % 10 == 9) {
            out << "\n";
        }
    }
    out << "\n\n";
}

void RandomNumberGenerator::showSettingsDialog()
{
    settingsDialog->setSettings(minValue, maxValue, countValue, exclusionEnabled, excludedNumbers);
    settingsDialog->exec();
}

void RandomNumberGenerator::updateResultDisplay()
{
    QString infoText = QString("范围: %1 - %2, 数量: %3, 排除: ")
                          .arg(minValue)
                          .arg(maxValue)
                          .arg(countValue);

    if (exclusionEnabled && !excludedNumbers.isEmpty()) {
        infoText += QString::number(excludedNumbers.size()) + "个数字";
    } else {
        infoText += "无";
    }

    infoLabel->setText(infoText);

    // 更新结果显示
    if (currentNumbers.isEmpty()) {
        resultTextEdit->setText("点击\"生成随机数\"按钮开始生成");
        return;
    }

    QString result;
    for (int i = 0; i < currentNumbers.size(); ++i) {
        result += QString::number(currentNumbers[i]);
        if (i < currentNumbers.size() - 1) {
            result += ", ";
        }
        if (i % 10 == 9) {
            result += "\n";
        }
    }

    resultTextEdit->setText(result);
}

bool RandomNumberGenerator::isNumberExcluded(int number) const
{
    return exclusionEnabled && excludedNumbers.contains(number);
}

void RandomNumberGenerator::loadSettings()
{
    // 使用应用程序目录下的配置文件
    QString configPath = QDir::current().filePath("RandomNumberGenerator.ini");
    QSettings settings(configPath, QSettings::IniFormat);

    minValue = settings.value("Settings/minValue", 1).toInt();
    maxValue = settings.value("Settings/maxValue", 100).toInt();
    countValue = settings.value("Settings/countValue", 10).toInt();
    exclusionEnabled = settings.value("Settings/exclusionEnabled", false).toBool();

    // 加载排除的数字
    QString exclusionText = settings.value("Settings/excludedNumbers").toString();
    excludedNumbers.clear();
    if (!exclusionText.isEmpty()) {
        QStringList numbers = exclusionText.split(',', Qt::SkipEmptyParts);
        for (const QString &numStr : numbers) {
            bool ok;
            int num = numStr.trimmed().toInt(&ok);
            if (ok) {
                excludedNumbers.append(num);
            }
        }
    }

    updateResultDisplay();
}

void RandomNumberGenerator::saveSettings()
{
    // 使用应用程序目录下的配置文件
    QString configPath = QDir::current().filePath("RandomNumberGenerator.ini");
    QSettings settings(configPath, QSettings::IniFormat);

    settings.setValue("Settings/minValue", minValue);
    settings.setValue("Settings/maxValue", maxValue);
    settings.setValue("Settings/countValue", countValue);
    settings.setValue("Settings/exclusionEnabled", exclusionEnabled);

    // 保存排除的数字
    QStringList numStrs;
    for (int num : excludedNumbers) {
        numStrs.append(QString::number(num));
    }
    settings.setValue("Settings/excludedNumbers", numStrs.join(","));
}