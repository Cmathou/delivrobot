#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
//uint32_t value = 0;

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
  }
};

////////////////////// WriteStrings /////////////////////////////
class MyCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string value = pCharacteristic->getValue();
    if (value.length() > 0)
    {
      if (value.front() == '0' && value.length() == 12)
      {
        int x = 0;
        int y = 0;
        memcpy(&x, value.substr(4, 4).data(), 4);
        memcpy(&y, value.substr(8, 4).data(), 4);
        Serial.println("*********");
        Serial.print("[x;y] = [");
        Serial.print(x);
        Serial.print(";");
        Serial.print(y);
        Serial.println("]");
      }
      else
      {
        return;
      }
    }
    /*
      if (value.length() > 0) {
        valor = "";
        for (int i = 0; i < value.length(); i++){
          // Serial.print(value[i]); // Presenta value.
          valor = valor + value[i];
        }

        Serial.println("*********");
        Serial.print("valor = ");
        Serial.println(valor); // Presenta valor.
      }
      */
  }
};
///////////////////////////////////////////////////

void setup()
{
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("FallourJTM");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE |
          BLECharacteristic::PROPERTY_NOTIFY |
          BLECharacteristic::PROPERTY_INDICATE);

  // https://www.bluetooth.com/specifications/gatt/viewer?
  //   attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());
  // Esta línea es para el WriteStrings:
  pCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop()
{
  // notify changed value
  if (deviceConnected)
  { 
    //std::string value = pCharacteristic->getValue();
    // pCharacteristic->setValue((uint8_t*)&value, 4);
    //int doble = valor.toInt() * 2 ;
    //String doblado = (String) doble;

    //pCharacteristic->setValue(doblado.c_str()); // Pone el numero doble
    //pCharacteristic->notify();
    delay(5); // bluetooth stack will go into congestion, if too many packets are sent.
  }
  // disconnecting
  if (!deviceConnected && oldDeviceConnected)
  {
    delay(500);                  // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected)
  {
    // do stuff here on connecting
    Serial.println("Device connected");
    oldDeviceConnected = deviceConnected;
  }
}
