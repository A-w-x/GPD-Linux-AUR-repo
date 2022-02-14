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
#ifndef AWXLINUXINSTALLER_H
#define AWXLINUXINSTALLER_H

#include <QMainWindow>
#include <QMessageBox>
#include <QMap>

#include "utils.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AwxLinuxInstaller; }
QT_END_NAMESPACE

class AwxLinuxInstaller : public QMainWindow
{
    Q_OBJECT

public:
    AwxLinuxInstaller(QWidget *parent = nullptr);
    ~AwxLinuxInstaller();

private:
    Ui::AwxLinuxInstaller *ui;
    AwxLinux::Utils utls;
    QProcess gpartedProc;
    QProcess instProc;
    bool processRunning = false;
    bool partitionsTabInit = false;
    bool isInstallISO = false;
    QMap<QString, QString> emulatorsOptMap = {
        {"Citra-canary", "citra-canary-git"},
        {"Dolphin", "dolphin-emu-git dolphin-emu-nogui-git"},
        {"Duckstation", "duckstation-git"},
        {"Flycast", "flycast-git"},
        {"Mgba", "libmgba-git mgba-qt-git mgba-sdl-git"},
        {"PCSX2", "pcsx2-git"},
        {"ppsspp", "ppsspp-common-git ppsspp-git ppsspp-headless-git ppsspp-qt-git"},
        {"RPCS3", "rpcs3-git"},
        {"Xemu", "xemu-git"},
        {"PCem", "pcem-git"},
        {"Melonds", "melonds-git"}
    };
    QMap<QString, QString> emulatorsStdMap = {
        {"SCUMMVM", "scummvm"},
        {"Retroarch", "retroarch"},
        {"ryujinx", "ryu"},
        {"Cemu", "cemu"},
        {"Yuzu", "yuzu-mainline-bin"}
    };
    QMap<QString, QString> pcGamesMap = {
        {"Steam", "steam"},
        {"Heroic launcher", "heroic-games-launcher-bin"},
        {"Lutris", "lutris"},
        {"proton-ge", "proton-ge-custom-bin"},
        {"Gamescope", "gamescope"}
    };

    void connectSignals();
    void refreshPartitionsList();
    void fillRegionCombo();
    void initEmulatorsTab();
    void initPcGamesTab();
    void genInstallScript(const QString &usrName, const QString &deviceName,
                          const QString &timezone, const QString &efiPart,
                          const QString &awxPart, const QString &homePart,
                          qint32 swapSz, bool installXfce, bool instGrub,
                          const QString &upwd, const QString &rpwd);
    void installOS();
    void installApps();

public slots:
    void onProcessFinished(const QString &output);
    void onTabChanged(int idx);
    void onRegionComboChange(const QString &text);
    void onGpartedBtnClicked();
    void onInstallBtnClicked();
};
#endif // AWXLINUXINSTALLER_H
