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

void AwxLinuxInstaller::initUI() {
    ui->awxPartBtrfsOpts->setVisible(false);
    ui->homePartBtrfsOpts->setVisible(false);
    ui->homePartCombo->setEnabled(false);
    ui->homePartFormatCombo->setEnabled(false);
    ui->shgamesPartCombo->setEnabled(false);
}

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
    initUI();
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
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->deskTab));
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(ui->emupcinfoTab));

        initEmulatorsTab();
        initPcGamesTab();
    }
}

AwxLinuxInstaller::~AwxLinuxInstaller() {
    delete ui;
}

void AwxLinuxInstaller::refreshPartitionsList() {
    if (processRunning)
        return;

    ui->fdiskOut->setText("loading..");
    ui->efiPartCombo->setEnabled(false);
    ui->awxPartCombo->setEnabled(false);
    ui->homePartCombo->setEnabled(false);
    ui->shgamesPartCombo->setEnabled(false);
    ui->refreshPartListBtn->setEnabled(false);

    utls.runFdiskAsync();

    processRunning = true;
}

void AwxLinuxInstaller::fillRegionCombo() {
    QList<QString> zlist = zonesList.keys();

    for (const QString &z: zlist)
        ui->regionCombo->addItem(z);
}

void AwxLinuxInstaller::initEmulatorsTab() {
    QList<QString> emuOptList = emulatorsOptMap.keys();
    QList<QString> emuStdList = emulatorsStdMap.keys();
    int row = 0, col = 0;
    QGridLayout *l;

    l = (QGridLayout *)ui->emuOptsGroup->layout();

    for (const QString &emu: emuOptList) {
        QCheckBox *ckb = new QCheckBox(ui->emuTab);

        ckb->setText(emu);
        ckb->setObjectName(emulatorsOptMap[emu]);
        l->addWidget(ckb, row, col);

        row = col+1 >= 3 ? row+1 : row;
        col = col+1 >= 3 ? 0 : col+1;
    }

    row = col = 0;
    l = (QGridLayout *)ui->emuStdGroup->layout();

    for (const QString &emu: emuStdList) {
        QCheckBox *ckb = new QCheckBox(ui->emuTab);

        ckb->setText(emu);
        ckb->setObjectName(emulatorsStdMap[emu]);
        l->addWidget(ckb, row, col);

        row = col+1 >= 3 ? row+1 : row;
        col = col+1 >= 3 ? 0 : col+1;
    }
}

void AwxLinuxInstaller::initPcGamesTab() {
    QList<QString> pcList = pcGamesMap.keys();
    QGridLayout *l = (QGridLayout *)ui->pcgamesGroup->layout();
    int row = 0;
    int col = 0;

    for (const QString &pc: pcList) {
        QCheckBox *ckb = new QCheckBox(ui->emuTab);

        ckb->setText(pc);
        ckb->setObjectName(pcGamesMap[pc]);
        l->addWidget(ckb, row, col);

        row = col+1 >= 3 ? row+1 : row;
        col = col+1 >= 3 ? 0 : col+1;
    }
}

QString AwxLinuxInstaller::getAwxBtrfsMountOpts() {
    QStringList mountOpts;

    if (ui->awxcomprChk->isChecked())
        mountOpts << "compress=zstd";

    if (!ui->awxcowChk->isChecked())
        mountOpts << "nodatacow";

    return mountOpts.join(',');
}

QString AwxLinuxInstaller::getHomeBtrfsMountOpts() {
    QStringList mountOpts;

    if (ui->homecomprChk->isChecked())
        mountOpts << "compress=zstd";

    if (!ui->homecowChk->isChecked())
        mountOpts << "nodatacow";

    return mountOpts.join(',');
}

QString AwxLinuxInstaller::genSummary(const installData &data) {
    QString summaryStr;
    QTextStream summary {&summaryStr};

    summary << "Summary\n\n\n" <<
               "EFI partition (mounted as /boot/efi): " << data.efiPart << "\n\n" <<
               "AwxLinux partition: " << data.awxPart << "\n\n" <<
               "AwxLinux partition format: " << (data.awxPartFormat.compare("-") == 0 ? "custom":data.awxPartFormat) << "\n\n";

    if (data.awxPartFormat.compare("btrfs") == 0) {
        summary << "AwxLinux partition compression: " << utls.boolString(ui->awxcomprChk->isChecked()) << "\n\n" <<
                   "AwxLinux partition Copy On Write: " << utls.boolString(ui->awxcowChk->isChecked()) << "\n\n" ;
    }

    if (ui->homePartChk->isChecked()) {
        summary << "Home partition: " << data.homePart << "\n\n" <<
                   "Home partition format: " << data.homePartFormat << "\n\n";

        if (data.homePartFormat.compare("btrfs") == 0) {
            summary << "Home partition compression: " << utls.boolString(ui->homecomprChk->isChecked()) << "\n\n" <<
                       "Home partition Copy On Write: " << utls.boolString(ui->homecowChk->isChecked()) << "\n\n" ;
        }
    }

    if (ui->shgamesChk->isChecked()) {
        summary << "Shared games partition: " << data.shgamesPart << "\n\n";

        if (ui->shgamesFormChk->isChecked())
            summary << "Wipe shared games partition: " << utls.boolString(ui->shgamesFormChk->isChecked()) << "\n\n";
    }

    summary << "Username: " << data.usrname << "\n\n" <<
               "Device name: " << data.deviceName << "\n\n" <<
               "Timezone: " << data.timezone << "\n\n" <<
               "Install AwxLinux Xfce: " << data.instXfce << "\n\n" <<
               "Install GRUB: " << data.instGrub << "\n\n";

    if (data.swapSz > 0)
        summary << "Swap size: " << data.swapSz << "GB\n\n";

    summary << "Do you want to install AwxLinux now?\n\n";

    return summary.readAll();
}

void AwxLinuxInstaller::genInstallScript(const installData &data) {
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
               "awxpart=\"" << data.awxPart << "\"\n" <<
               "pformat=\"" << data.awxPartFormat << "\"\n" <<
               "btrfsmopts=\"" << data.awxPartBtrfsOpts << "\"\n" <<
               "homepart=\"" << data.homePart << "\"\n" <<
               "homeformat=\"" << data.homePartFormat << "\"\n" <<
               "homebtrfsmopts=\"" << data.homePartBtrfsOpts << "\"\n" <<
               "efipart=\"" << data.efiPart << "\"\n" <<
               "shgamespart=\"" << data.shgamesPart << "\"\n" <<
               "formatshgames=\"" << data.shgamesPartWipe << "\"\n" <<
               "timezone=\"" << data.timezone << "\"\n" <<
               "deviceName='" << data.deviceName << "'\n" <<
               "username='" << data.usrname << "'\n" <<
               "upwd='" << data.encUpwd << "'\n" <<
               "rpwd='" << data.encRpwd << "'\n" <<
               "swap=" << data.swapSz << "\n" <<
               "instxfce=\"" << data.instXfce << "\"\n" <<
               "instGrub=\"" << data.instGrub << "\"\n" <<
               instScript.readAll();

    instScript.close();
    finstScript.close();

    finstScript.setPermissions(QFile::ExeOwner | QFile::ReadGroup | QFile::ReadOther | QFile::ReadOwner | QFile::WriteOwner);
}

bool AwxLinuxInstaller::areValidSettings(const installData &data, const QString &upwd, const QString &ucpwd,
                                         const QString &rpwd, const QString &rcpwd) {
    QString err;

    if (data.awxPart == "")
        err = "AwxLinux device cannot be empty";
    else if (data.efiPart == "")
        err = "EFI device cannot be empty";
    else if (ui->homePartChk->isChecked() && data.homePart == "")
        err = "You enabled Home partition but no device is selected.";
    else if (ui->shgamesChk->isChecked() && data.shgamesPart == "")
        err = "You enabled a shared games partition but no device is selected.";
    else if (upwd == "" || ucpwd == "" || rpwd == "" || rcpwd == "")
        err = "Password field cannot be empty";
    else if (data.usrname == "")
        err = "Username field cannot be empty";
    else if (data.deviceName == "")
        err = "Device name field cannot be empty";
    else if (upwd.compare(ucpwd) != 0)
        err = "User passwords do not match";
    else if (rpwd.compare(rcpwd) != 0)
        err = "Root passwords do not match";

    if (!err.isEmpty()) {
        QMessageBox::critical(this, "Invalid settings", err);
        return false;
    }

    return true;
}

void AwxLinuxInstaller::installOS() {
    installData data {
        .region = ui->regionCombo->currentText(),
        .zone = ui->zoneCombo->currentText(),
        .efiPart = ui->efiPartCombo->currentText(),
        .instGrub = utls.boolString(ui->instGrubCombo->currentIndex() == 0),
        .awxPart = ui->awxPartCombo->currentText(),
        .awxPartFormat = ui->awxPFormatCombo->currentIndex() == 2 ? "-":ui->awxPFormatCombo->currentText(),
        .homePart = ui->homePartCombo->currentText(),
        .homePartFormat = ui->homePartFormatCombo->currentIndex() == 0 ? "-":ui->homePartFormatCombo->currentText(),
        .usrname = ui->usrname->text().trimmed(),
        .deviceName = ui->devicename->text().simplified().remove(' '),
        .swapSz = ui->swapFileSize->value(),
        .instXfce = utls.boolString(ui->instxfceChk->isChecked()),
        .shgamesPart = ui->shgamesPartCombo->currentText(),
        .shgamesPartWipe = utls.boolString(ui->shgamesFormChk->isChecked())
    };
    QString upwd = ui->upwd->text();
    QString ucpwd = ui->ucpwd->text();
    QString rpwd = ui->rpwd->text();
    QString rcpwd = ui->rcpwd->text();
    QMessageBox::StandardButton msgBoxRet;


    if (!areValidSettings(data, upwd, ucpwd, rpwd, rcpwd))
        return;

    data.timezone = data.region + (data.zone != "" ? ("/" + data.zone) : "" );
    data.encUpwd = QString::fromLatin1( crypt( upwd.toUtf8(), utls.make_salt( 16 ).toUtf8() ) );
    data.encRpwd = QString::fromLatin1( crypt( rpwd.toUtf8(), utls.make_salt( 16 ).toUtf8() ) );

    if (data.awxPartFormat.compare("btrfs") == 0)
        data.awxPartBtrfsOpts = getAwxBtrfsMountOpts();

    if (ui->homePartChk->isChecked()) {
        if (data.homePartFormat.compare("btrfs") == 0)
            data.homePartBtrfsOpts = getHomeBtrfsMountOpts();
    }

    msgBoxRet = QMessageBox::question(this, "Summary", genSummary(data), QMessageBox::Yes | QMessageBox::No);
    if (msgBoxRet == QMessageBox::No)
        return;

    genInstallScript(data);
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

        if (ckb->isChecked())
            emusStrList << w->objectName();
    }

    for (const QWidget *w: emuStdList) {
        QCheckBox *ckb = (QCheckBox *)w;

        if (ckb->isChecked())
            emusStrList << w->objectName();
    }

    for (const QWidget *w: pcgamesList) {
        QCheckBox *ckb = (QCheckBox *)w;

        if (ckb->isChecked())
            pcGamesStrList << w->objectName();
    }

    fstream << "#!/bin/bash\n";

    if (!emusStrList.empty()) {
        fstream << "yay -S --noconfirm wolfssl cubeb cpp-httplib-compiled discord-rpc-git fmt libao wxgtk2 wxgtk3\n";

        if (emusStrList.contains("yuzu-mainline-bin")) {
            fstream << "yay -S --noconfirm yuzu-mainline-bin\n";
            emusStrList.removeAll("yuzu-mainline-bin");
        }

        if (emusStrList.contains("pcsx2-git"))
            fstream << "yay -S --noconfirm rapidyaml-git\n";

        if (emusStrList.contains("cemu")) {
            fstream << "sudo pacman -S --noconfirm wine wine-mono winetricks lutris vkd3d lib32-vkd3d\n" <<
                       "cmver=\"1.26.2\"\n" <<
                       "wget \"https://cemu.info/releases/cemu_$cmver.zip\"\n" <<
                       "unzip \"cemu_$cmver.zip\"\n" <<
                       "mv \"cemu_$cmver\" Cemu\n" <<
                       "wget https://lutris.nyc3.cdn.digitaloceanspaces.com/games/cemu/sharedFonts.tar.xz\n" <<
                       "tar -xvf sharedFonts.tar.xz --directory Cemu\n" <<
                       "cd Cemu\n" <<
                       "mkdir controllerProfiles\n" <<
                       "cd controllerProfiles\n" <<
                       "wget https://raw.githubusercontent.com/AccountOneOff/lutris_files/master/Xbox_Controller.txt\n" <<
                       "wget https://raw.githubusercontent.com/A-w-x/GPD-Linux-AUR-repo/main/config_files/emulators/cemu/controllerProfiles/gpd.txt\n" <<
                       "cd ../..\n" <<
                       "mkdir -p /opt/Cemu/drive_c\n" <<
                       "mv Cemu /opt/Cemu/drive_c\n" <<
                       "rm \"cemu_$cmver.zip\" sharedFonts.tar.xz\n" <<
                       "rm -r Cemu\n";

            emusStrList.removeAll("cemu");
        }

        if (emusStrList.contains("ryu")) {
            fstream << "ryuV=\"1.1.104\"\n" <<
                       "rm -r /opt/ryujinx\n" <<
                       "wget \"https://github.com/Ryujinx/release-channel-master/releases/download/$ryuV/ryujinx-$ryuV-linux_x64.tar.gz\"\n" <<
                       "tar -xvf \"ryujinx-$ryuV-linux_x64.tar.gz\"\n" <<
                       "mv publish ryujinx\n" <<
                       "mv ryujinx /opt\n" <<
                       "chmod +x /opt/ryujinx/Ryujinx\n" <<
                       "sudo ln -s /opt/ryujinx/Ryujinx /usr/bin/ryujinx\n" <<
                       "rm \"ryujinx-$ryuV-linux_x64.tar.gz\"\n" <<
                       "gtk-update-icon-cache\n";

            emusStrList.removeAll("ryu");
        }

        if (!emusStrList.empty())
            fstream << "sudo pacman -S --noconfirm " << emusStrList.join(" ") << "\n";
    }

    if (!pcGamesStrList.empty()) {
        fstream << "echo -e \"Installing pc games apps\\n\"\n" <<
                   "sudo pacman -S --noconfirm wine wine-mono winetricks lutris vkd3d lib32-vkd3d wqy-zenhei lib32-libcanberra\n" <<
                   "yay -S --noconfirm " << pcGamesStrList.join(" ") << "\n";

        if (pcGamesStrList.contains("steam")) { // steam big picture focus fix
            fstream << "echo \"export SDL_VIDEO_MINIMIZE_ON_FOCUS_LOSS=0\" >> \"$HOME/.bashrc\"\n" <<
                       "gtk-update-icon-cache\n";
        }
    }

    if (ui->steamtinkerChk->isChecked()) {
        fstream << "yay -S --noconfirm steamtinkerlaunch\n";
    }

    if (ui->instMangoChk->isChecked()) {
        fstream << "sudo pacman -S --noconfirm vulkan-tools mesa-demos breeze\n" <<
                   "yay -S --noconfirm vkbasalt goverlay-bin lib32-mangohud\n";
    }

    if (ui->xbcontrChk->isChecked()) { // add some tweaks from ChimeraOS
        fstream << "yay -S --noconfirm xpadneo-dkms-git\n" <<
                   "sudo sed -i 's/#MinConnectionInterval=/MinConnectionInterval=7/' /etc/bluetooth/main.conf\n" <<
                   "sudo sed -i 's/#MaxConnectionInterval=/MaxConnectionInterval=9/' /etc/bluetooth/main.conf\n" <<
                   "sudo sed -i 's/#ConnectionLatency=/ConnectionLatency=0/' /etc/bluetooth/main.conf\n" <<
                   "sudo sed -i 's/#JustWorksRepairing = never/JustWorksRepairing=confirm/' /etc/bluetooth/main.conf\n";
    }

    if (ui->steamtinkerChk->isChecked())
        fstream << "complete initial steam setup, then type 'steamtinkerlaunch compat add' to add tinker launch to steam play settings";

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
    ui->homePartCombo->clear();
    ui->shgamesPartCombo->clear();

    ui->efiPartCombo->addItem("");
    ui->awxPartCombo->addItem("");
    ui->homePartCombo->addItem("");
    ui->shgamesPartCombo->addItem("");

    for (const QString &part: partitions) {
        ui->efiPartCombo->addItem(part);
        ui->awxPartCombo->addItem(part);
        ui->homePartCombo->addItem(part);
        ui->shgamesPartCombo->addItem(part);
    }

    ui->efiPartCombo->setEnabled(true);
    ui->awxPartCombo->setEnabled(true);
    ui->homePartCombo->setEnabled(ui->homePartChk->isChecked());
    ui->shgamesPartCombo->setEnabled(true);
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

void AwxLinuxInstaller::on_awxPFormatCombo_currentIndexChanged(int index) {
    ui->awxPartBtrfsOpts->setVisible(index == 1);
}

void AwxLinuxInstaller::on_homePartChk_stateChanged(int arg1) {
    ui->homePartCombo->setEnabled(arg1 == Qt::Checked);
    ui->homePartFormatCombo->setEnabled(arg1 == Qt::Checked);
}

void AwxLinuxInstaller::on_homePartFormatCombo_currentIndexChanged(int index) {
    ui->homePartBtrfsOpts->setVisible(index == 2);
}

void AwxLinuxInstaller::on_awxcowChk_stateChanged(int arg1) {
    ui->awxcomprChk->setEnabled(arg1 == Qt::Checked);

    if (ui->awxcomprChk->isChecked())
        ui->awxcomprChk->setChecked(arg1 == Qt::Checked);
}

void AwxLinuxInstaller::on_homecowChk_stateChanged(int arg1) {
    ui->homecomprChk->setEnabled(arg1 == Qt::Checked);

    if (ui->homecomprChk->isChecked())
        ui->homecomprChk->setChecked(arg1 == Qt::Checked);
}

void AwxLinuxInstaller::on_shgamesChk_stateChanged(int arg1) {
    ui->shgamesPartCombo->setEnabled(arg1 == Qt::Checked);
    ui->shgamesFormChk->setEnabled(arg1 == Qt::Checked);
}
