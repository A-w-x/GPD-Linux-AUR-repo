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
#include <QApplication>
#include <proc/readproc.h>

#include "awxlinuxinstaller.h"

bool isInstallerAlreadyRunning() {
    PROCTAB* proctab = openproc(PROC_FILLSTAT | PROC_FILLSTATUS);
    bool running = false;
    proc_t proc;

    memset(&proc, 0, sizeof(proc_t));

    while (readproc(proctab, &proc) != nullptr) {
        QString procName(proc.cmd);
        bool isPowerMax = procName.compare("awxLinuxInstall") == 0;

        if (isPowerMax && running)
            return true;
        else if (isPowerMax)
            running = true;
    }

    closeproc(proctab);
    return false;
}

int main(int argc, char *argv[]) {
    if (isInstallerAlreadyRunning())
        return 0;

    QApplication a(argc, argv);
    AwxLinuxInstaller w;

    w.setFixedHeight(690);
    w.show();

    return a.exec();
}
