/*
Copyright 2022 The Foedag team

GPL License

Copyright (c) 2022 The Open-Source FPGA Foundation

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MPW1Loader.h"

#include <QSet>

namespace FOEDAG {

constexpr int PinNameIndex{5};

MPW1Loader::MPW1Loader(PackagePinsModel *model, QObject *parent)
    : PackagePinsLoader(model, parent) {}

std::pair<bool, QString> MPW1Loader::load(const QString &fileName) {
  const auto &[success, content] = getFileContent(fileName);
  if (!success) return std::make_pair(success, content);

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
  QStringList lines = content.split("\n", Qt::SkipEmptyParts);
#else
  QStringList lines = content.split("\n", QString::SkipEmptyParts);
#endif
  parseHeader(lines.takeFirst());
  PackagePinGroup group{};
  QVector<QString> tmp{Voltage2 + 1};
  QStringList oneLine = QStringList::fromVector(tmp);
  QSet<QString> uniquePins;
  for (const auto &line : lines) {
    QStringList data = line.split(",");
    if (data.count() <= PinNameIndex)
      return std::make_pair(false, "Wrong table data");
    if (uniquePins.contains(data.at(PinNameIndex))) continue;
    uniquePins.insert(data.at(PinNameIndex));

    oneLine[PinName] = data.at(PinNameIndex);
    group.pinData.append({oneLine});
  }
  m_model->append(group);  // append last
  m_model->initListModel();

  return std::make_pair(true, QString{});
}

}  // namespace FOEDAG
