#include <fstream>
#include <iostream>
#include "libusb.h"
#include <string>
using namespace std;
void printdev(libusb_device *dev,  ofstream& out){
    libusb_device_descriptor desc;
    libusb_get_device_descriptor(dev, &desc);
    int dclass = (int)desc.bDeviceClass;
    if (dclass == 0){
        libusb_config_descriptor *config;
        libusb_get_config_descriptor(dev, 0, &config);
        const libusb_interface *inter;
        const libusb_interface_descriptor *interdesc;
        for(int i = 0; i < (int)config->bNumInterfaces; i++){
            inter = &config->interface[i];
            for(int j = 0; j < inter->num_altsetting; j++) {
                dclass = (int)(&inter->altsetting[j])->bInterfaceClass;
            }
        }
    }

    out << dclass << ";" << (int)desc.idVendor << ";" << (int)desc.idProduct << ";";   
    if ((int)desc.bDeviceClass == 239){
        out << (int)desc.bDeviceSubClass << "/" << (int)desc.bDeviceProtocol << ";";
    } else {
        out << ";";
    }
}

void get_serial_numbers(libusb_device *dev, ofstream& out){
    libusb_device_descriptor desc;
    libusb_get_device_descriptor(dev, &desc);
    libusb_device_handle *handle;
    libusb_open(dev, &handle);
    if (handle == nullptr) return;
    auto* data = new uint8_t [33]();
    libusb_get_string_descriptor_ascii(handle, desc.iSerialNumber, data, 31);
    data[32] = '\0';
    out << data << ";\n";
    delete[] data;
    libusb_close(handle);
}
int main(){
    libusb_device **devs;
    libusb_context *ctx = NULL;
    ssize_t cnt;
    ssize_t i;
    int r;
    r = libusb_init(&ctx);
    cnt = libusb_get_device_list(ctx, &devs);
    ofstream out;
    out.open("out.csv");
    out << "Класс устройства;Идентификатор производителя;Идентификатор изделия; Подкласс/протокол;\n";
    for(i = 0; i < cnt; i++) {
        printdev(devs[i], out);
        get_serial_numbers(devs[i], out);
    }
    libusb_free_device_list(devs, 1);
    libusb_exit(ctx);
    return 0;
}
