#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class ConfigDialog;
}

class QAbstractButton;
class ConfigDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ConfigDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
	~ConfigDialog();

	void setIniPath(const QString & _strIniPath);
	void setRomName(const char * _romName);
	void setTitle();
	bool isAccepted() const { return m_accepted; }

public Q_SLOTS:
	virtual void accept(bool justSave);

private slots:
	void on_PickFontColorButton_clicked();

	void on_buttonBox_clicked(QAbstractButton *button);

	void on_fullScreenResolutionComboBox_currentIndexChanged(int index);

	void on_windowedResolutionComboBox_currentIndexChanged(int index);

	void on_windowedResolutionComboBox_currentTextChanged(QString text);

	void on_overscanCheckBox_toggled(bool checked);

	void on_aliasingWarningLabel_linkActivated(QString link);

	void on_frameBufferInfoLabel_linkActivated(QString link);

	void on_customSettingsCheckBox_toggled(bool checked);

	void on_frameBufferInfoLabel2_linkActivated(QString link);

	void on_frameBufferCheckBox_toggled(bool checked);

	void on_aliasingSlider_valueChanged(int value);

	void on_gammaLevelSpinBox_valueChanged(double value);

	void on_gammaCorrectionCheckBox_toggled(bool checked);

	void on_resolutionFactorSpinBox_valueChanged(int value);

	void on_fontTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

	void on_fontSizeSpinBox_valueChanged(int value);

	void on_tabWidget_currentChanged(int tab);

	void on_texPackPathButton_clicked();

	void on_texCachePathButton_clicked();

	void on_texDumpPathButton_clicked();

	void on_profilesComboBox_currentIndexChanged(const QString &arg1);

	void on_settingsDestProfileRadioButton_toggled(bool checked);

	void on_removeProfilePushButton_clicked();

	void on_nativeRes2DComboBox_currentIndexChanged(int index);

	void on_n64DepthCompareComboBox_currentIndexChanged(int index);

private:
	void _init(bool reInit = false, bool blockCustomSettings = false);
	void _getTranslations(QStringList & _translationFiles) const;
	void _switchDest(bool isGame);

	Ui::ConfigDialog *ui;
	QFont m_font;
	QColor m_color;
	bool m_accepted;
	bool m_fontsInited;
	bool m_blockReInit;
	QString m_strIniPath;
	const char * m_romName;
};

#endif // CONFIGDIALOG_H
