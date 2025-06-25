#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initDisplay(); //display summary info page
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    resetDisplay(); //reset the info page every clicked

    if(item->text(column) == "Summary"){
        printf("Summary clicked\n");
        fflush(stdout);
        ui->tableWidget_down->show();
    }else if(item->text(column) == "Operating System"){
        printf("Operating System clicked\n");
        fflush(stdout);
        ui->tableWidget_down->show();
    }else if(item->text(column) == "Security"){
        printf("Security clicked\n");
        fflush(stdout);
        ui->tableWidget_down->show();
    }else if(item->text(column) == "Kernel Modules"){
        printf("Kernel Modules clicked\n");
        fflush(stdout);
        ui->tableWidget_down->show();
        ui->tableWidget_up->show();
    }else if(item->text(column) == "Boots"){
        printf("Boots clicked\n");
        fflush(stdout);
        ui->tableWidget_up->show();
    }else if(item->text(column) == "Memory Usage"){
        printf("Memory Usage clicked\n");
        fflush(stdout);
        ui->tableWidget_up->show();
    }else if(item->text(column) == "Filesystems"){
        printf("Filesystems clicked\n");
        fflush(stdout);
        ui->tableWidget_down->show();
        ui->tableWidget_up->show();
    }else if(item->text(column) == "Displays"){
        printf("Displays clicked\n");
        fflush(stdout);
        ui->tableWidget_down->show();
    }else if(item->text(column) == "Users"){
        printf("Users clicked\n");
        fflush(stdout);
        ui->tableWidget_down->show();
        ui->tableWidget_up->show();
    }else if(item->text(column) == "Groups"){
        printf("Groups clicked\n");
        fflush(stdout);
        ui->tableWidget_up->show();
    }else if(item->text(column) == "PCI Devices"){
        printf("PCI Devices clicked\n");
        fflush(stdout);
        flag_treeClicked = treeClicked::PCIDevice;
        ui->tableWidget_down->show();
        ui->tableWidget_up->show();
        display_PCIInfo_on_tableWidget_up();
    }else if(item->text(column) == "Processor"){
        printf("Processor clicked\n");
        fflush(stdout);
        flag_treeClicked = treeClicked::Processor;
        ui->tableWidget_down->show();
        ui->tableWidget_up->show();
        display_CPUInfo_on_tableWidget_up();
    }else{
        //display blank table
        printf("display blanktable\n");
        fflush(stdout);
    }
}

void MainWindow::initDisplay(){
    ui->tableWidget_up->verticalHeader()->hide();
    ui->tableWidget_up->setShowGrid(false);
    ui->tableWidget_up->setEditTriggers(QTableWidget::NoEditTriggers);
    ui->tableWidget_up->setSelectionBehavior(QTableWidget::SelectRows);

    ui->tableWidget_up->hide();
    ui->tableWidget_down->hide();
}

void MainWindow::resetDisplay(){
    ui->tableWidget_up->clear();
    ui->tableWidget_up->hide();
    ui->tableWidget_up->setRowCount(0);
    ui->tableWidget_up->setColumnCount(0);
    ui->tableWidget_down->clear();
    ui->tableWidget_down->hide();
    ui->tableWidget_down->setRowCount(0);
    ui->tableWidget_down->setColumnCount(0);
}


//void MainWindow::displaySummaryPage(){
//    //set table size and location

//    //display info on arranged table
//    displayCPUInfo();
//    displayPCIInfo();
//}

#ifdef __cplusplus
extern "C"
{
#endif

void MainWindow::display_CPUInfo_on_tableWidget_up(){
    struct cpu_raw_data_t raw;
    struct cpu_id_t data;
    cpuid_present();
    if(cpuid_get_raw_data(&raw) < 0){
        printf("Error: %s\n", cpuid_error());
    }
    if(cpu_identify(&raw, &data) < 0){
        printf("Error: %s\n", cpuid_error());
    }

    int deviceCount = data.num_cores + 1;
    QString cpuDeviceData[deviceCount][4];
    ui->tableWidget_up->setRowCount(deviceCount);
    ui->tableWidget_up->setColumnCount(4);
    ui->tableWidget_up->setHorizontalHeaderLabels(QStringList() << "Device" << "Model" << "Socket:Core" << "Frequency");

    cpuDeviceData[0][0] = QString("all");
    cpuDeviceData[0][1] = QString("Summary");

    int cpuID = 0;
    for(int row = 1; row < deviceCount; ++ row){
        int col = 0;
        cpuDeviceData[row][col] = QString("cpu%1").arg(cpuID);
        cpuDeviceData[row][col + 1] = data.brand_str;
        cpuDeviceData[row][col + 2] = QString("%3 : %4").arg(system("grep 'physical id' /proc/cpuinfo | sort -u | awk {'print $4'}")).arg(cpuID);
        cpuDeviceData[row][col + 3] = QString("%5 MHz").arg(cpu_clock_measure(200, 0));
        ++cpuID;
    }

//transmit array stored data to table widget for displaying
    for(int row = 0; row < deviceCount; ++row){
        for(int col = 0; col < 4; ++col){
            QTableWidgetItem *item = new QTableWidgetItem(cpuDeviceData[row][col]);
            ui->tableWidget_up->setItem(row, col, item);
        }
    }
}

void MainWindow::display_PCIInfo_on_tableWidget_up(){
    printf("print PCIInfo\n");

    struct pci_access *pacc;
    struct pci_dev *dev;
    unsigned int c;
    char namebuf[1024], *name;

    int deviceCount = 0;
    int iterator_columns = 0;

    pacc = pci_alloc();		/* Get the pci_access structure */
    /* Set all options you want -- here we stick with the defaults */
    pci_init(pacc);		/* Initialize the PCI library */
    pci_scan_bus(pacc);		/* We want to get the list of devices */

    //scan the devices and stored the counts
    for(dev=pacc->devices; dev; dev=dev->next){
        deviceCount++;
    }

    QString pciDeviceData[deviceCount][7];
    ui->tableWidget_up->setRowCount(deviceCount);
    ui->tableWidget_up->setColumnCount(7);
    ui->tableWidget_up->setHorizontalHeaderLabels(QStringList() << "Name" << "Address" << "Vendor ID" << "Device ID" << "Class" << "IRQ" << "Base");

    for (dev=pacc->devices; dev; dev=dev->next)	/* Iterate over all devices */
      {
        pci_fill_info(dev, PCI_FILL_IDENT | PCI_FILL_BASES | PCI_FILL_CLASS);	/* Fill in header info we need */

        c = pci_read_byte(dev, PCI_INTERRUPT_PIN);				/* Read config register directly */
        name = pci_lookup_name(pacc, namebuf, sizeof(namebuf), PCI_LOOKUP_DEVICE, dev->vendor_id, dev->device_id);

        /*store the value into array*/
        pciDeviceData[iterator_columns][0] = name;
        pciDeviceData[iterator_columns][1] = QString("%1:%2:%3.%4").arg(dev->domain).arg(dev->bus).arg(dev->dev).arg(dev->func);
        pciDeviceData[iterator_columns][2] = QString("%5").arg(dev->vendor_id, 4, 16, QLatin1Char('O'));
        pciDeviceData[iterator_columns][3] = QString("%6").arg(dev->device_id, 4, 16, QLatin1Char('O'));
        pciDeviceData[iterator_columns][4] = QString("%7").arg(dev->device_class, 4, 16, QLatin1Char('O'));
        pciDeviceData[iterator_columns][5] = QString("%8 (pin %9)").arg(dev->irq).arg(c);
        pciDeviceData[iterator_columns][6] = QString("%10").arg((long)dev->base_addr[0]);

        ++iterator_columns;
      }

//transmit array stored data to table widget for displaying
    for(int row = 0; row < deviceCount; ++row){
        for(int col = 0; col < 7; ++col){
            QTableWidgetItem *item = new QTableWidgetItem(pciDeviceData[row][col]);
            ui->tableWidget_up->setItem(row, col, item);
        }
    }
    pci_cleanup(pacc);		/* Close everything */
}

#ifdef __cplusplus
}
#endif


void MainWindow::on_tableWidget_up_clicked(const QModelIndex &index)
{
            switch(flag_treeClicked){
                case treeClicked::kernelModule:
                    printf("tableWidget_Down_Messgae: KernelModule");
                    break;
                case treeClicked::FileSystem:
                    printf("tableWidget_Down_Messgae: FileSystem");
                    break;
                case treeClicked::Users:
                    printf("tableWidget_Down_Messgae: Users");
                    break;
                case treeClicked::Processor:
                    printf("tableWidget_Down_Messgae: Processor");
                    display_CPUInfo_on_tableWidget_down();
                    break;
                case treeClicked::GraphicProcessor:
                    break;
                case treeClicked::Monitors:
                    break;
                case treeClicked::MemoryDevice:
                    break;
                case treeClicked::PCIDevice:
                    printf("tableWidget_Down_Messgae: PCIDevice");
                    display_PCIInfo_on_tableWidget_down();
                    break;
                case treeClicked::USBDevice:
                    break;
                case treeClicked::InputDevice:
                    break;
                case treeClicked::Storage:
                    break;
                case treeClicked::InterFaces:
                    break;
                default:
                    break;
            }
}

void MainWindow::display_CPUInfo_on_tableWidget_down(){
    //set tableWidget col and row
    //set row fixed at 2

    //set col fixed at ?
    //seperate for 2 kinds of selection 1.all summary 2.others cores
}

void MainWindow::display_PCIInfo_on_tableWidget_down(){
    //set tableWidget col and row
    //set row fixed at 2

    //set col fixed at ?
    //seperate for 2 kinds of selection (detect if have PCI Express) 1.w/ PCI Express 2.w/o PCI Express

}


