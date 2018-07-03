#pragma once

#include <QAbstractTableModel>
#include <QVector>
#include <QMap>
#include <QStringList>

#include "data.h"

class SearchResultsModel : public QAbstractTableModel {
	Q_OBJECT
public:
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const override;
	virtual QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;
	virtual bool setData(const QModelIndex&, const QVariant& value, int role = Qt::EditRole) override;
	virtual Qt::ItemFlags flags(const QModelIndex&) const override;

	void setDataBacking(Retro::GameData* = nullptr);
	Retro::GameData* getDataBacking() { return m_data; }
	void setVariable(const QString& name);
	QString getVariable() const { return m_variable; }

public slots:
	void refresh();

private:
	void refreshImpl();

	Retro::GameData* m_data = nullptr;

	Retro::Search* m_search = nullptr;
	const std::vector<Retro::TypedSearchResult>* m_results = nullptr;
	QString m_variable;
};
