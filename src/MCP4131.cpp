#include "Arduino.h"
#include "MCP4131.h"
#include <SPI.h>

int slaveSelectPin;

MCP4131::MCP4131(int slavePin) {
    slaveSelectPin = slavePin;
	pinMode(slavePin, OUTPUT);
}

void MCP4131::init() {
    SPI.begin();
}

byte MCP4131::readWiper() {
    return sendCommand(ADDRESS_WIPER0, COMMAND_READ, 255);
}

void MCP4131::writeWiper(unsigned int wiperValue) {  
    sendCommand(ADDRESS_WIPER0, COMMAND_WRITE, wiperValue);
}

void MCP4131::sendCommand(byte address, char command) {
    // Adjust SPI settings to fit MCP4131
    SPI.beginTransaction(SPISettings(250000, MSBFIRST, SPI_MODE0));

    // take the SS pin low to de-select the chip:
    enableChip();
    
    byte msb = (address << 4) | ((command << 2) | COMMAND_MASK);

    SPI.transfer(msb);

    // take the SS pin high to de-select the chip:
    disableChip();

    // Stop using the SPI bus to allow other chips to use SPI
    SPI.endTransaction();
}

byte MCP4131::sendCommand(byte address, char command, unsigned int data) {
    // Adjust SPI settings to fit MCP4131
    SPI.beginTransaction(SPISettings(250000, MSBFIRST, SPI_MODE0));

    // take the SS pin low to select the chip:
    enableChip();

    byte transferByte = (address << 4) | ((command << 2) | COMMAND_MASK);

    SPI.transfer(transferByte);
    byte result = SPI.transfer(data);

    // take the SS pin high to de-select the chip:
    disableChip();

    // Stop using the SPI bus to allow other chips to use SPI
    SPI.endTransaction();

    return result;
    
}

void MCP4131::decrementWiper() {
    sendCommand(ADDRESS_WIPER0, COMMAND_DECREMENT);
    }

void MCP4131::incrementWiper() {
    sendCommand(ADDRESS_WIPER0, COMMAND_INCREMENT);
}

void MCP4131::enableChip() {
    digitalWrite(slaveSelectPin, LOW);
}

void MCP4131::disableChip() {
    digitalWrite(slaveSelectPin, HIGH);
}