#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTableWidget>
#include <QTextBrowser>

#include <iostream>
#include <stdio.h>
#include <lib/pci.h>
#include <3rdparty/libcpuid/libcpuid/libcpuid.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_tableWidget_up_clicked(const QModelIndex &index);

private:
    void initDisplay();
    void resetDisplay();

    void display_CPUInfo_on_tableWidget_up();
    void display_PCIInfo_on_tableWidget_up();

    void display_CPUInfo_on_tableWidget_down();
    void display_PCIInfo_on_tableWidget_down();

private:
    Ui::MainWindow *ui;
    int flag_treeClicked;
};

enum treeClicked{
        kernelModule,
        FileSystem,
        Users,
        Processor,
        GraphicProcessor,
        Monitors,
        MemoryDevice,
        PCIDevice,
        USBDevice,
        InputDevice,
        Storage,
        InterFaces
};


#endif // MAINWINDOW_H
