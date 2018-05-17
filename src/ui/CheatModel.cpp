#include "CheatModel.h"

using namespace Retro;

CheatModel::CheatModel(EmulatorController* controller)
	: m_controller(controller) {
}

int CheatModel::rowCount(const QModelIndex&) const {
	return m_cheats.size();
}

int CheatModel::columnCount(const QModelIndex&) const {
	return 3;
}

QVariant CheatModel::headerData(int section, Qt::Orientation orientation, int role) const {
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
		return tr("Cheat");
	case 2:
		return tr("Enabled");
	}
	return QVariant();
}

QVariant CheatModel::data(const QModelIndex& index, int role) const {
	if (role == Qt::CheckStateRole && index.column() == 2) {
		return m_cheats[index.row()].enabled ? Qt::Checked : Qt::Unchecked;
	}
	if (role != Qt::DisplayRole && role != Qt::EditRole) {
		return QVariant();
	}

	switch (index.column()) {
	case 0:
		return m_cheats[index.row()].name;
	case 1:
		return m_cheats[index.row()].value;
	}
	return QVariant();
}

bool CheatModel::setData(const QModelIndex& index, const QVariant& value, int role) {
	if (role != Qt::EditRole && role != Qt::CheckStateRole) {
		return false;
	}

	switch (index.column()) {
	case 2:
		m_cheats[index.row()].enabled = value == Qt::Checked;
		m_controller->enableCheat(index.row(), value == Qt::Checked);
		return true;
	}
	return false;
}

Qt::ItemFlags CheatModel::flags(const QModelIndex& index) const {
	Qt::ItemFlags flags = QAbstractTableModel::flags(index);
	if (index.column() == 2) {
		flags |= Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
	}
	return flags;
}

bool CheatModel::removeRows(int row, int count, const QModelIndex&) {
	beginRemoveRows(QModelIndex(), row, row + count - 1);
	for (int i = 0; i < count; ++i) {
		m_controller->removeCheat(row);
	}
	refreshImpl();
	endRemoveRows();
	return true;
}

void CheatModel::setEmulator(EmulatorController* controller) {
	beginResetModel();
	m_controller = controller;
	refreshImpl();
	endResetModel();
}

void CheatModel::addCheat(const Cheat& cheat) {
	beginInsertRows(QModelIndex(), m_cheats.size(), m_cheats.size());
	m_controller->addCheat(cheat);
	refreshImpl();
	endInsertRows();
}

void CheatModel::refresh() {
	beginResetModel();
	refreshImpl();
	endResetModel();
}

void CheatModel::refreshImpl() {
	m_cheats = m_controller->cheats();
}
