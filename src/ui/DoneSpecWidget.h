#pragma once

#include <QWidget>

#include "data.h"

namespace Ui {
class DoneSpecWidget;
}

class DoneSpecWidget : public QWidget {
	Q_OBJECT
public:
	DoneSpecWidget(QWidget* parent = nullptr);
	~DoneSpecWidget();

	void setName(const QString&);
	QString getName() const { return m_name; }

	void setSpec(const Retro::Scenario::DoneSpec&);
	Retro::Scenario::DoneSpec getSpec() const;

signals:
	void removed();

private:
	Ui::DoneSpecWidget* m_ui;
	QString m_name;
};
