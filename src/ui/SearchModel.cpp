#include "SearchModel.h"

using namespace Retro;

int SearchModel::rowCount(const QModelIndex&) const {
	return m_sortedKeys.size();
}

int SearchModel::columnCount(const QModelIndex&) const {
	return 2;
}

QVariant SearchModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (orientation != Qt::Horizontal) {
		return QVariant();
	}
	if (role != Qt::DisplayRole) {
		return QVariant();
	}
	switch (section) {
	case 0:
		return tr("Name");
	case 1:
		return tr("Found");
	}
	return QVariant();
}

QVariant SearchModel::data(const QModelIndex& index, int role) const {
	if (role != Qt::DisplayRole) {
		return QVariant();
	}

	const QString& key = m_sortedKeys[index.row()];

	switch (index.column()) {
	case 0:
		return key;
	case 1:
		return static_cast<qlonglong>(m_data->getSearch(key.toStdString())->numResults());
	}
	return QVariant();
}

bool SearchModel::removeRows(int row, int count, const QModelIndex&) {
	beginRemoveRows(QModelIndex(), row, row + count - 1);
	for (int i = 0; i < count; ++i) {
		m_data->removeSearch(m_sortedKeys[row].toStdString());
	}
	refreshImpl();
	endRemoveRows();
	return true;
}

void SearchModel::setDataBacking(Retro::GameData* data) {
	beginResetModel();
	m_data = data;
	refreshImpl();
	endResetModel();
}

void SearchModel::refresh() {
	beginResetModel();
	refreshImpl();
	endResetModel();
}

void SearchModel::refreshImpl() {
	m_sortedKeys.clear();
	if (!m_data) {
		return;
	}
	const auto& searches = m_data->listSearches();
	for (const auto& search : searches) {
		m_sortedKeys.append(QString::fromStdString(search));
	}
	m_sortedKeys.sort();
}
