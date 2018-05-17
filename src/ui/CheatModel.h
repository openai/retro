#pragma once

#include <QAbstractTableModel>
#include <QStringList>

#include "EmulatorController.h"

class CheatModel : public QAbstractTableModel {
	Q_OBJECT
public:
	CheatModel(EmulatorController* = nullptr);

	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const override;
	virtual QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;
	virtual bool setData(const QModelIndex&, const QVariant& value, int role = Qt::EditRole) override;
	virtual Qt::ItemFlags flags(const QModelIndex&) const override;
	virtual bool removeRows(int row, int count, const QModelIndex& = QModelIndex()) override;

	void setEmulator(EmulatorController* = nullptr);
	void addCheat(const Cheat&);

public slots:
	void refresh();

private:
	void refreshImpl();

	EmulatorController* m_controller = nullptr;

	QList<Cheat> m_cheats;
};
