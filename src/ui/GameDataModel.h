#pragma once

#include <QAbstractTableModel>
#include <QStringList>

namespace Retro {
class GameData;
}

class GameDataModel : public QAbstractTableModel {
	Q_OBJECT
public:
	GameDataModel(Retro::GameData* = nullptr);

	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const override;
	virtual QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;
	virtual bool setData(const QModelIndex&, const QVariant& value, int role = Qt::EditRole) override;
	virtual Qt::ItemFlags flags(const QModelIndex&) const override;
	virtual bool removeRows(int row, int count, const QModelIndex& = QModelIndex()) override;

	void setDataBacking(Retro::GameData* = nullptr);
	Retro::GameData* getDataBacking() { return m_data; }

public slots:
	void refresh();

private:
	void refreshImpl();

	Retro::GameData* m_data = nullptr;

	QStringList m_variableCache;
};
