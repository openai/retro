#include "SearchResultsModel.h"

#include <QFontDatabase>

using namespace Retro;

int SearchResultsModel::rowCount(const QModelIndex&) const {
	return m_results ? m_results->size() : 0;
}

int SearchResultsModel::columnCount(const QModelIndex&) const {
	return 3;
}

QVariant SearchResultsModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (orientation != Qt::Horizontal) {
		return QVariant();
	}
	if (role != Qt::DisplayRole) {
		return QVariant();
	}
	switch (section) {
	case 0:
		return tr("Value");
	case 1:
		return tr("Address");
	case 2:
		return tr("Type");
	}
	return QVariant();
}

QVariant SearchResultsModel::data(const QModelIndex& index, int role) const {
	if (role == Qt::TextAlignmentRole) {
		switch (index.column()) {
		case 2:
		default:
			return static_cast<int>(Qt::AlignLeading | Qt::AlignVCenter);
		case 0:
		case 1:
			return static_cast<int>(Qt::AlignTrailing | Qt::AlignVCenter);
		}
	}
	if (index.column() == 1 && role == Qt::FontRole) {
		QFont font(QFontDatabase::systemFont(QFontDatabase::FixedFont));
		font.setPointSizeF(QFont().pointSizeF());
		return font;
	}

	if (role != Qt::DisplayRole && role != Qt::EditRole) {
		return QVariant();
	}

	switch (index.column()) {
	case 0:
		return static_cast<qint64>(m_data->lookupValue((*m_results)[index.row()]));
	case 1:
		return QString::number((*m_results)[index.row()].address, 16);
	case 2:
		return (*m_results)[index.row()].type.type;
	}
	return QVariant();
}

bool SearchResultsModel::setData(const QModelIndex& index, const QVariant& value, int role) {
	if (role != Qt::EditRole) {
		return false;
	}

	switch (index.column()) {
	case 0:
		m_data->lookupValue((*m_results)[index.row()]) = value.toLongLong();
		return true;
	}
	return false;
}

Qt::ItemFlags SearchResultsModel::flags(const QModelIndex& index) const {
	Qt::ItemFlags flags = QAbstractTableModel::flags(index);
	if (index.column() == 0) {
		flags |= Qt::ItemIsEditable;
	}
	return flags;
}

void SearchResultsModel::setDataBacking(Retro::GameData* data) {
	beginResetModel();
	m_data = data;
	refreshImpl();
	endResetModel();
}

void SearchResultsModel::setVariable(const QString& name) {
	beginResetModel();
	m_search = m_data->getSearch(name.toStdString());
	m_variable = name;
	refreshImpl();
	endResetModel();
}

void SearchResultsModel::refresh() {
	beginResetModel();
	refreshImpl();
	endResetModel();
}

void SearchResultsModel::refreshImpl() {
	m_results = nullptr;
	if (m_data && m_search) {
		m_results = &m_search->typedResults();
	}
}
