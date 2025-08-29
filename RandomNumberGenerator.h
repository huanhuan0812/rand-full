// RandomNumberGenerator.h
#ifndef RANDOMNUMBERGENERATOR_H
#define RANDOMNUMBERGENERATOR_H

#include <QWidget>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include "SettingsDialog.h"

class RandomNumberGenerator : public QWidget
{
    Q_OBJECT

public:
    explicit RandomNumberGenerator(QWidget *parent = nullptr);
    ~RandomNumberGenerator();

private slots:
    void generateRandomNumbers();
    void copyToClipboard();
    void showSettingsDialog();
    void updateResultDisplay();

private:
    void setupUI();
    void loadSettings();
    void saveSettings();
    bool isNumberExcluded(int number) const;
    void saveToHistoryFile(); // 保存到历史文件
    void writeRandomNumbersToStream(QTextStream &out, bool includeHeader = true); // 写入数据到流

    SettingsDialog *settingsDialog;

    // 控件
    QPushButton *settingsButton;
    QPushButton *generateButton;
    QPushButton *copyButton;
    QTextEdit *resultTextEdit;
    QLabel *infoLabel;

    // 当前设置
    int minValue;
    int maxValue;
    int countValue;
    bool exclusionEnabled;
    QList<int> excludedNumbers;

    // 当前生成的数字
    QList<int> currentNumbers;
};

#endif // RANDOMNUMBERGENERATOR_H