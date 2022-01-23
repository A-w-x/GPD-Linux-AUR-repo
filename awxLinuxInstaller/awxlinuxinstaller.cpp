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
#include <QFile>
#include <QTextStream>
#include <QCheckBox>
#include <QDir>
#include <QStringList>
#include <crypt.h>

#include "awxlinuxinstaller.h"
#include "./ui_awxlinuxinstaller.h"
#include "zoneinfo.h"

void AwxLinuxInstaller::connectSignals() {
    QObject::connect(&utls, &AwxLinux::Utils::processOutputReady, this, &AwxLinuxInstaller::onProcessFinished);
    QObject::connect(ui->refreshPartListBtn, &QPushButton::clicked, this, &AwxLinuxInstaller::refreshPartitionsList);
    QObject::connect(ui->tabWidget, &QTabWidget::currentChanged, this, &AwxLinuxInstaller::onTabChanged);
    QObject::connect(ui->regionCombo, &QComboBox::currentTextChanged, this, &AwxLinuxInstaller::onRegionComboChange);
    QObject::connect(ui->gpartedRun, &QPushButton::clicked, this, &AwxLinuxInstaller::onGpartedBtnClicked);
    QObject::connect(ui->installBtn, &QPushButton::clicked, this, &AwxLinuxInstaller::onInstallBtnClicked);
}

AwxLinuxInstaller::AwxLinuxInstaller(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AwxLinuxInstaller)
{
    ui->setupUi(this);
    connectSignals();
    fillRegionCombo();
    ui->tabWidget->setCurrentIndex(0);

    isInstallISO = QDir("/root/.config/openbox").exists();

    if (isInstallISO) {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->emuTab));
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->pcgamesTab));

    } else {
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->partitionTab));
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->userSetTab));
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->bootloaderTab));
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->emupcinfoTab));

        initEmulatorsTab();
        initPcGamesTab();
    }
}

AwxLinuxInstaller::~AwxLinuxInstaller()
{
    delete ui;
}

void AwxLinuxInstaller::refreshPartitionsList() {
    if (processRunning)
        return;

    ui->fdiskOut->setText("loading..");
    ui->efiPartCombo->setEnabled(false);
    ui->awxPartCombo->setEnabled(false);
    ui->refreshPartListBtn->setEnabled(false);

    utls.runFdiskAsync();

    processRunning = true;
}

void AwxLinuxInstaller::fillRegionCombo() {
    for (const QString &z: zonesList.keys())
        ui->regionCombo->addItem(z);
}

void AwxLinuxInstaller::initEmulatorsTab() {
    QGridLayout *l;
    int row = 0;
    int col = 0;

    l = (QGridLayout *)ui->emuOptsGroup->layout();

    for (const QString &emu: emulatorsOptMap.keys()) {
        QCheckBox *ckb = new QCheckBox(ui->emuTab);

        ckb->setText(emu);
        ckb->setObjectName(emulatorsOptMap[emu]);
        l->addWidget(ckb, row, col);

        row = col+1 >= 3 ? row+1 : row;
        col = col+1 >= 3 ? 0 : col+1;
    }

    row = col = 0;
    l = (QGridLayout *)ui->emuStdGroup->layout();

    for (const QString &emu: emulatorsStdMap.keys()) {
        QCheckBox *ckb = new QCheckBox(ui->emuTab);

        ckb->setText(emu);
        ckb->setObjectName(emulatorsStdMap[emu]);
        l->addWidget(ckb, row, col);

        row = col+1 >= 3 ? row+1 : row;
        col = col+1 >= 3 ? 0 : col+1;
    }
}

void AwxLinuxInstaller::initPcGamesTab() {
    QGridLayout *l = (QGridLayout *)ui->pcgamesGroup->layout();
    int row = 0;
    int col = 0;

    for (const QString &pc: pcGamesMap.keys()) {
        QCheckBox *ckb = new QCheckBox(ui->emuTab);

        ckb->setText(pc);
        ckb->setObjectName(pcGamesMap[pc]);
        l->addWidget(ckb, row, col);

        row = col+1 >= 3 ? row+1 : row;
        col = col+1 >= 3 ? 0 : col+1;
    }
}

void AwxLinuxInstaller::genInstallScript(const QString &usrName, const QString &deviceName,
                                         const QString &timezone, const QString &efiPart,
                                         const QString &awxPart, qint32 swapSz, bool instGrub,
                                         const QString &upwd, const QString &rpwd) {
    QFile instScript {"/root/awxinstall.part"};
    QFile finstScript {"/usr/local/bin/awxinstall"};
    QTextStream fstream {&finstScript};

    instScript.open(QIODevice::ReadOnly | QIODevice::Text);
    finstScript.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);

    if (!instScript.isOpen()) {
        QMessageBox::critical(this, "Error", "Unable to open .part file");
        return;

    } else if (!finstScript.isOpen()) {
        QMessageBox::critical(this, "Error", "Unable to open output file");
        return;
    }

    fstream << "#!/bin/bash\n" <<
               "awxpart=\"" << awxPart << "\"\n" <<
               "efipart=\"" << efiPart << "\"\n" <<
               "timezone=\"" << timezone << "\"\n" <<
               "deviceName='" << deviceName << "'\n" <<
               "username='" << usrName << "'\n" <<
               "upwd='" << upwd << "'\n" <<
               "rpwd='" << rpwd << "'\n" <<
               "swap=" << swapSz << "\n" <<
               "instGrub=\"" << (instGrub ? "true":"false") << "\"\n" <<
               instScript.readAll();

    instScript.close();
    finstScript.close();

    finstScript.setPermissions(QFile::ExeOwner | QFile::ReadGroup | QFile::ReadOther | QFile::ReadOwner | QFile::WriteOwner);
}

void AwxLinuxInstaller::installOS() {
    QString upwd = ui->upwd->text();
    QString ucpwd = ui->ucpwd->text();
    QString rpwd = ui->rpwd->text();
    QString rcpwd = ui->rcpwd->text();
    QString region = ui->regionCombo->currentText();
    QString zone = ui->zoneCombo->currentText();
    QString efiPart = ui->efiPartCombo->currentText();
    QString awxPart = ui->awxPartCombo->currentText();
    QString usrname = ui->usrname->text().trimmed();
    QString deviceName = ui->devicename->text().simplified().remove(' ');
    qint32 swapSz = ui->swapFileSize->value();
    qint32 instGrub = ui->instGrubCombo->currentIndex();
    QMessageBox::StandardButton msgBoxRet;
    QString summaryStr;
    QString timezone;
    QString encUpwd;
    QString encRpwd;
    QTextStream summary {&summaryStr};

    if (awxPart == "") {
        QMessageBox::critical(this, "Invalid install partition", "AwxLinux partition cannot be empty");
        return;
    }

    if (efiPart == "") {
        QMessageBox::critical(this, "Invalid EFI partition", "EFI partition cannot be empty");
        return;
    }

    if (upwd == "" || ucpwd == "" || rpwd == "" || rcpwd == "") {
        QMessageBox::critical(this, "Invalid passwords", "Password field cannot be empty");
        return;
    }

    if (usrname == "") {
        QMessageBox::critical(this, "Invalid username", "Username field cannot be empty");
        return;
    }

    if (deviceName == "") {
        QMessageBox::critical(this, "Invalid device name", "Device name field cannot be empty");
        return;
    }

    if (upwd.compare(ucpwd) != 0) {
        QMessageBox::critical(this, "Invalid user password", "User passwords do not match");
        return;
    }

    if (rpwd.compare(rcpwd) != 0) {
        QMessageBox::critical(this, "Invalid root password", "Root passwords do not match");
        return;
    }

    timezone = region + (zone != "" ? ("/" + zone) : "" );
    encUpwd = QString::fromLatin1( crypt( upwd.toUtf8(), utls.make_salt( 16 ).toUtf8() ) );
    encRpwd = QString::fromLatin1( crypt( rpwd.toUtf8(), utls.make_salt( 16 ).toUtf8() ) );

    summary << "Summary\n\n\n" <<
               "EFI partition (mounted as /boot/efi): " << efiPart << "\n\n" <<
               "AwxLinux install partition: " << awxPart << "\n\n" <<
               "Username: " << usrname << "\n\n" <<
               "Device name: " << deviceName << "\n\n" <<
               "Timezone: " << timezone << "\n\n" <<
               "Install GRUB: " << (instGrub == 0 ? "Yes":"No") << "\n\n";

    if (swapSz > 0)
        summary << "Swap size: " << swapSz << "GB\n\n";

    summary << "Do you want to install AwxLinux now?\n\n";

    msgBoxRet = QMessageBox::question(this, "Summary", summary.readAll(), QMessageBox::Yes | QMessageBox::No);

    if (msgBoxRet == QMessageBox::No)
        return;

    genInstallScript(usrname, deviceName, timezone, efiPart, awxPart, swapSz, instGrub == 0, encUpwd, encRpwd);

    instProc.start("lxterminal", QStringList() << "-e" << "awxinstall");
}

void AwxLinuxInstaller::installApps() {
    QList<QWidget *> emuOptList = ui->emuOptsGroup->findChildren<QWidget *>();
    QList<QWidget *> emuStdList = ui->emuStdGroup->findChildren<QWidget *>();
    QList<QWidget *> pcgamesList = ui->pcgamesGroup->findChildren<QWidget *>();
    QStringList emusStrList;
    QStringList pcGamesStrList;
    QFile instScript {"/opt/awxappsinstall"};
    QTextStream fstream {&instScript};

    instScript.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text);

    if (!instScript.isOpen()) {
        QMessageBox::critical(this, "Error", "Unable to create apps install script");
        return;
    }

    for (const QWidget *w: emuOptList) {
        QCheckBox *ckb = (QCheckBox *)w;

        if (ckb->checkState() == Qt::Checked)
            emusStrList << w->objectName();
    }

    for (const QWidget *w: emuStdList) {
        QCheckBox *ckb = (QCheckBox *)w;

        if (ckb->checkState() == Qt::Checked)
            emusStrList << w->objectName();
    }

    for (const QWidget *w: pcgamesList) {
        QCheckBox *ckb = (QCheckBox *)w;

        if (ckb->checkState() == Qt::Checked)
            pcGamesStrList << w->objectName();
    }

    fstream << "#!/bin/bash\n";

    if (!emusStrList.empty()) {
        fstream << "yay -S --noconfirm wolfssl cubeb cpp-httplib-compiled discord-rpc-git\n";

        if (emusStrList.contains("yuzu-mainline-bin")) {
            fstream << "yay -S --noconfirm yuzu-mainline-bin\n";
            emusStrList.removeAll("yuzu-mainline-bin");
        }

        if (emusStrList.contains("cemu")) {
            fstream << "sudo pacman -S --noconfirm wine wine-mono winetricks lutris vkd3d lib32-vkd3d\n" <<
                       "cmver=\"1.26.1\"\n" <<
                       "chver=\"1251c_0575\"\n" <<
                       "wget \"https://cemu.info/releases/cemu_$cmver.zip\"\n" <<
                       "wget \"https://files.sshnuke.net/cemuhook_$chver.zip\"\n" <<
                       "unzip \"cemu_$cmver.zip\"\n" <<
                       "mv \"cemu_$cmver\" Cemu\n" <<
                       "unzip \"cemuhook_$chver.zip\" -d Cemu\n" <<
                       "wget https://lutris.nyc3.cdn.digitaloceanspaces.com/games/cemu/sharedFonts.tar.xz\n" <<
                       "tar -xvf sharedFonts.tar.xz --directory Cemu\n" <<
                       "cd Cemu\n" <<
                       "wget https://raw.githubusercontent.com/dnmodder/cemu_lutris_files/master/cemuhook.ini\n" <<
                       "mkdir controllerProfiles\n" <<
                       "cd controllerProfiles\n" <<
                       "wget https://raw.githubusercontent.com/AccountOneOff/lutris_files/master/Xbox_Controller.txt\n" <<
                       "wget https://raw.githubusercontent.com/A-w-x/GPD-Linux-AUR-repo/main/config_files/emulators/cemu/controllerProfiles/gpd.txt\n" <<
                       "cd ../..\n" <<
                       "mkdir -p /opt/Cemu/drive_c\n" <<
                       "mv Cemu /opt/Cemu/drive_c\n" <<
                       "rm \"cemu_$cmver.zip\" \"cemuhook_$chver.zip\" sharedFonts.tar.xz\n" <<
                       "rm -r Cemu\n";

            emusStrList.removeAll("cemu");
        }

        if (emusStrList.contains("ryu")) {
            fstream << "ryuV=\"1.1.5\"\n" <<
                       "wget \"https://github.com/Ryujinx/release-channel-master/releases/download/$ryuV/ryujinx-$ryuV-linux_x64.tar.gz\"\n" <<
                       "tar -xvf \"ryujinx-$ryuV-linux_x64.tar.gz\"\n" <<
                       "mkdir /opt/ryujinx\n" <<
                       "mv publish ryujinx\n" <<
                       "mv ryujinx /opt\n" <<
                       "chmod +x /opt/ryujinx/Ryujinx\n" <<
                       "sudo ln -s /opt/ryujinx/Ryujinx /usr/bin/ryujinx\n" <<
                       "rm \"ryujinx-$ryuV-linux_x64.tar.gz\"\n" <<
                       "gtk-update-icon-cache\n";

            emusStrList.removeAll("ryu");
        }

        fstream << "sudo pacman -S --noconfirm " << emusStrList.join("") << "\n";
    }

    if (!pcGamesStrList.empty()) {
        fstream << "echo -e \"Installing pc games apps\\n\"\n" <<
                   "sudo pacman -S --noconfirm wine wine-mono winetricks lutris vkd3d lib32-vkd3d wqy-zenhei lib32-libcanberra\n" <<
                   "yay -S --noconfirm " << pcGamesStrList.join("") << "\n";

        if (pcGamesStrList.contains("steam")) { // steam big picture focus fix
            fstream << "echo \"export SDL_VIDEO_MINIMIZE_ON_FOCUS_LOSS=0\" >> \"$HOME/.bashrc\"\n" <<
                       "gtk-update-icon-cache\n";
        }
    }

    if (ui->runSteamBootChk->checkState() == Qt::Checked)
        fstream << "echo \"steam -bigpicture &\" >> \"$HOME/.xinitp2\"\n";

    if (ui->instMangoChk->checkState() == Qt::Checked) {
        fstream << "sudo pacman -S --noconfirm vulkan-tools mesa-demos\n" <<
                   "yay -S --noconfirm vkbasalt goverlay-bin lib32-mangohud\n";
    }

    fstream << "echo \"###########\"\n" <<
               "echo Done!\n" <<
               "echo \"##########\"\n"
               "read -p \"Press any key to exit\"\n" <<
               "rm /opt/awxappsinstall\n";

    instScript.close();
    instScript.setPermissions(QFile::ExeOwner | QFile::ReadGroup | QFile::ReadOther | QFile::ReadOwner | QFile::WriteOwner);

    instProc.start("xfce4-terminal", QStringList() << "-e" << "/opt/awxappsinstall");
}

void AwxLinuxInstaller::onProcessFinished(const QString &output) {
    QList<QString> partitions = utls.scanPartitions();

    ui->fdiskOut->setText(output);
    ui->efiPartCombo->clear();
    ui->awxPartCombo->clear();

    ui->efiPartCombo->addItem("");
    ui->awxPartCombo->addItem("");

    for (const QString &part: partitions) {
        ui->efiPartCombo->addItem(part);
        ui->awxPartCombo->addItem(part);
    }

    ui->efiPartCombo->setEnabled(true);
    ui->awxPartCombo->setEnabled(true);
    ui->refreshPartListBtn->setEnabled(true);

    processRunning = false;
}

void AwxLinuxInstaller::onTabChanged(int idx) {
    switch (idx) {
        case 1: {
            if (!partitionsTabInit) {
                refreshPartitionsList();
                partitionsTabInit = true;
            }
        }
            break;
        default:
            break;
    }
}

void AwxLinuxInstaller::onRegionComboChange(const QString &text) {
    ui->zoneCombo->clear();

    if (zonesList[text].count() == 0)
        return;

    for (const QString &s: zonesList[text])
        ui->zoneCombo->addItem(s);
}

void AwxLinuxInstaller::onGpartedBtnClicked() {
    gpartedProc.start("gparted");
}

void AwxLinuxInstaller::onInstallBtnClicked() {
    if (isInstallISO)
        installOS();
    else
        installApps();
}
