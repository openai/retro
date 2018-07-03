#pragma once

#include <QDialog>
#include <QMap>

class QGroupBox;

namespace Ui {
class ControlsWidget;
}

class ControlsWidget : public QDialog {
	Q_OBJECT
public:
	ControlsWidget(const QString& platform, QWidget* parent = nullptr);
	~ControlsWidget();

	void setKeys(const QStringList&);
	void setBindings(const QMap<QString, int>&, unsigned player = 0);
	void setPlayers(unsigned players);

	QMap<QString, int> getBindings(unsigned player = 0) const;

public slots:
	void accept() override;

signals:
	void bindingChanged(const QString&, int binding, int player);

private:
	Ui::ControlsWidget* m_ui;

	QStringList m_keyNames;
	QList<QMap<QString, int>> m_bindings;
	QList<QGroupBox*> m_sections;
	unsigned m_players = 1;
};
