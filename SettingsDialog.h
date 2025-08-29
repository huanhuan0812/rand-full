#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QTabWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QScrollArea>
#include <QButtonGroup>
#include <QLabel>

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);

    void setSettings(int min, int max, int count, bool exclusionEnabled, const QList<int> &excludedNumbers);

    int getMinValue() const { return minSpinBox->value(); }
    int getMaxValue() const { return maxSpinBox->value(); }
    int getCountValue() const { return countSpinBox->value(); }
    bool isExclusionEnabled() const { return enableExclusionCheckBox->isChecked(); }
    QList<int> getExcludedNumbers() const;

    signals:
        void settingsChanged();

private slots:
    void updateRangeLimits();
    void updateExclusionCheckboxes();
    void toggleExclusionCheckboxes(bool checked);
    void updateExclusionFromText();
    void updateExclusionFromCheckboxes();
    void accept() override;

private:
    QSpinBox *minSpinBox;
    QSpinBox *maxSpinBox;
    QSpinBox *countSpinBox;
    QCheckBox *enableExclusionCheckBox;
    QLineEdit *exclusionLineEdit;
    QScrollArea *exclusionScrollArea;
    QWidget *exclusionContainer;
    QGridLayout *exclusionLayout;
    QButtonGroup *exclusionButtonGroup;
};

#endif // SETTINGSDIALOG_H