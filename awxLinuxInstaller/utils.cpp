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
#include <parted/parted.h>
#include <parted/device.h>
#include <algorithm>
#include <QCryptographicHash>

#include "utils.h"
#include "Entropy.h"

namespace AwxLinux {
    Utils::Utils() {
        QObject::connect(&proc, &QProcess::finished, this, &Utils::getFdiskOutp);
    }

    QList<QString> Utils::scanPartitions() {
        QList<QString> partList = QList<QString>();
        PedDevice *dev = nullptr;
        PedDisk *dsk = nullptr;

        ped_device_probe_all();

        dev = ped_device_get_next(dev);
        while (dev != nullptr) {
             dsk = ped_disk_new(dev);

             if (dsk != nullptr) {
                 PedPartition *parts = dsk->part_list;

                 while (parts != nullptr) {
                     if (parts->num > 0)
                         partList.append(ped_partition_get_path(parts));

                     parts = parts->next;
                 }
             }

             dev = ped_device_get_next(dev);
        }

        std::sort(partList.begin(), partList.end());
        return partList;
    }

    void Utils::runFdiskAsync() {
        proc.start("fdisk", QStringList() << "-l");
    }

    void Utils::getFdiskOutp() {
        emit processOutputReady(QString(proc.readAllStandardOutput()));
    }

    // Borrowed from Calamares:
    // https://github.com/calamares/calamares/blob/calamares/src/modules/users/SetPasswordJob.cpp
    QString Utils::make_salt(int length) {
        Q_ASSERT( length >= 8 );
        Q_ASSERT( length <= 128 );

        QString salt_string;
        CalamaresUtils::EntropySource source = CalamaresUtils::getPrintableEntropy( length, salt_string );

        if ( salt_string.length() != length )
            salt_string.truncate( length );

        salt_string.insert( 0, "$6$" );
        salt_string.append( '$' );
        return salt_string;
    }

    QString Utils::boolString(bool vbool) const {
        return vbool ? "true":"false";
    }
}
