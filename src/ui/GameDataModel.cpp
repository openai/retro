#include "GameDataModel.h"

#include <QFont>
#include <QFontDatabase>
#include <QPalette>

#include "data.h"

using namespace Retro;

GameDataModel::GameDataModel(GameData* data)
	: m_data(data) {
	refresh();
}

int GameDataModel::rowCount(const QModelIndex&) const {
	return m_variableCache.size();
}

int GameDataModel::columnCount(const QModelIndex&) const {
	return 5;
}

QVariant GameDataModel::headerData(int section, Qt::Orientation orientation, int role) const {
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
		return tr("Value");
	case 2:
		return tr("Address");
	case 3:
		return tr("Type");
	case 4:
		return tr("Mask");
	}
	return QVariant();
}

QVariant GameDataModel::data(const QModelIndex& index, int role) const {
	std::string variable = m_variableCache[index.row()].toStdString();
	const auto& var = m_data->getVariable(variable);
	if (role == Qt::TextAlignmentRole) {
		switch (index.column()) {
		case 0:
		default:
			return static_cast<int>(Qt::AlignLeading | Qt::AlignVCenter);
		case 1:
		case 2:
		case 3:
			return static_cast<int>(Qt::AlignTrailing | Qt::AlignVCenter);
		case 4:
			return static_cast<int>(var.mask != UINT64_MAX ? Qt::AlignTrailing : Qt::AlignHCenter) | Qt::AlignVCenter;
		}
	}
	switch (index.column()) {
	case 2:
		if (role == Qt::FontRole) {
			QFont font(QFontDatabase::systemFont(QFontDatabase::FixedFont));
			font.setPointSizeF(QFont().pointSizeF());
			return font;
		}
		break;
	case 4:
		if (var.mask == UINT64_MAX) {
			if (role == Qt::ForegroundRole) {
				return QPalette().brush(QPalette::Disabled, QPalette::WindowText);
			}
			if (role == Qt::FontRole) {
				QFont font;
				font.setItalic(true);
				return font;
			}
		}
		break;
	}
	if (role != Qt::DisplayRole && role != Qt::EditRole) {
		return QVariant();
	}

	switch (index.column()) {
	case 0:
		return m_variableCache[index.row()];
	case 1:
		try {
			return static_cast<qint64>(m_data->lookupValue(variable));
		} catch (...) {
			return tr("error");
		}
	case 2:
		return QString::number(var.address, 16);
	case 3:
		return QString(var.type.type);
	case 4:
		return var.mask != UINT64_MAX ? QString::number(var.mask, 2) : tr("None");
	}
	return QVariant();
}

bool GameDataModel::setData(const QModelIndex& index, const QVariant& value, int role) {
	if (role != Qt::EditRole) {
		return false;
	}

	std::string variable = m_variableCache[index.row()].toStdString();
	const auto& var = m_data->getVariable(variable);

	if (data(index, role) == value) {
		return true;
	}

	bool ok;
	qint64 intVal;
	quint64 uintVal;
	switch (index.column()) {
	case 0:
		m_data->removeVariable(variable);
		m_data->setVariable(value.toString().toStdString(), var);
		m_variableCache[index.row()] = value.toString();
		break;
	case 1:
		try {
			m_data->lookupValue(variable) = value.toLongLong();
		} catch (...) {
		}
		break;
	case 2:
		uintVal = value.toString().toULongLong(&ok, 16);
		if (ok) {
			m_data->setVariable(variable, Variable{ var.type, uintVal, var.mask });
		}
		break;
	case 3:
		try {
			if (value.toString().length() >= 3) {
				m_data->setVariable(variable, Variable{ value.toString().toStdString(), var.address, var.mask });
			}
		} catch (...) {
		}
		break;
	case 4:
		intVal = value.toString().toLongLong(&ok, 2);
		m_data->setVariable(variable, Variable{ var.type, var.address, ok ? intVal : UINT64_MAX });
		break;
	default:
		return false;
	}
	emit dataChanged(index, index);
	return true;
}

Qt::ItemFlags GameDataModel::flags(const QModelIndex& index) const {
	return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool GameDataModel::removeRows(int row, int count, const QModelIndex&) {
	beginRemoveRows(QModelIndex(), row, row + count - 1);
	for (int i = 0; i < count; ++i) {
		m_data->removeVariable(m_variableCache[row].toStdString());
	}
	refreshImpl();
	endRemoveRows();
	return true;
}

void GameDataModel::setDataBacking(Retro::GameData* data) {
	beginResetModel();
	m_data = data;
	refreshImpl();
	endResetModel();
}

void GameDataModel::refresh() {
	beginResetModel();
	refreshImpl();
	endResetModel();
}

void GameDataModel::refreshImpl() {
	m_variableCache.clear();
	if (!m_data) {
		return;
	}
	for (const auto& var : m_data->listVariables()) {
		m_variableCache.append(QString::fromStdString(var.first));
	}
	m_variableCache.sort();
}
