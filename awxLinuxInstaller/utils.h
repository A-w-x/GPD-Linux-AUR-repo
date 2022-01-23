/*
AwxLinuxInstaller
Copyright (C) A-w-x 2022

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
#pragma once

#include <QString>
#include <QList>
#include <QObject>
#include <QProcess>

namespace AwxLinux {
    class Utils: public QObject {
    Q_OBJECT

    private:
        QProcess proc;

        void getFdiskOutp();

    public:
        Utils();

        QList<QString> scanPartitions();
        void runFdiskAsync();
        QString make_salt(int length);

    signals:
        void partitionsListScanDone(const QList<QString> &partitionsList);
        void processOutputReady(const QString &output);
    };


}
