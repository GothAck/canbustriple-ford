
#include "Middleware.h"


class FordData : public Middleware
{
  public:
    void tick();
    bool process( Message msg );
    FordData(SerialCommand *serialCommand);
    void commandHandler(byte* bytes, int length);
  private:
    SerialCommand* cmd;
};

FordData::FordData(SerialCommand *serialCommand) {
  cmd = serialCommand;
}

void FordData::tick(){}

void FordData::commandHandler(byte* bytes, int length){}

bool FordData::process( Message msg )
{
  bool did = false;
  
  switch (msg.busId) {
    case 1:
      switch (msg.frame_id) {
        case 0x201:
          {
            unsigned int rpm = (msg.frame_data[0] << 8) + msg.frame_data[1]; // revs per minute
            unsigned int delta = (msg.frame_data[2] << 8) + msg.frame_data[3];
            float speed_ = ((msg.frame_data[4] << 8) + msg.frame_data[5]) / 100; // km/h
            cmd->activeSerial->print(F("{\"engine_gas\": {\"rpm\":"));
            cmd->activeSerial->print(rpm, DEC);
            cmd->activeSerial->print(F(",\"weird_delta\":"));
            cmd->activeSerial->print(delta, DEC);            
            cmd->activeSerial->print(F(",\"speed\":"));
            cmd->activeSerial->print(speed_, 4);
            cmd->activeSerial->print(F(",\"accelerator\":"));
            cmd->activeSerial->print(msg.frame_data[6], DEC);
            cmd->activeSerial->print(F("}}\n"));
            did = true;
            break;
          }
        case 0x228:
          {
            cmd->activeSerial->print(F("{\"gear\": {\"0\":"));
            cmd->activeSerial->print(msg.frame_data[0], DEC);
            cmd->activeSerial->print(F(",\"1\":"));
            cmd->activeSerial->print(msg.frame_data[1], DEC);
            cmd->activeSerial->print(F("}}\n"));
//            did = true;
            break;
          }
        case 0x231:
          {
            cmd->activeSerial->print(F("{\"gear2\": {\"0\":"));
            cmd->activeSerial->print(msg.frame_data[0], DEC);
            cmd->activeSerial->print(F(",\"torque_demand\":"));
            cmd->activeSerial->print((msg.frame_data[1] << 8) + msg.frame_data[2], DEC);
            cmd->activeSerial->print(F("}}\n"));
//            did = true;
          }
        case 0x420:
          {
            int coolant = msg.frame_data[0] - 40;
            cmd->activeSerial->print(F("{\"engine_read\": {\"coolant_degc\":"));
            cmd->activeSerial->print(coolant, DEC);
            cmd->activeSerial->print(F(",\"inlet_kPa\":"));
            cmd->activeSerial->print(msg.frame_data[7], DEC);
            cmd->activeSerial->print(F("}}\n"));
            did = true;
            break;
          }
        case 0x433:
          {
            cmd->activeSerial->print(F("{\"doors\": {\"map\":"));
            cmd->activeSerial->print(msg.frame_data[0], DEC);
            cmd->activeSerial->print(F(",\"lock\":"));
            cmd->activeSerial->print(msg.frame_data[5], DEC);
            cmd->activeSerial->print(F("}}\n"));
            did = true;
            break;
          }
        case 0x4B0:
          {
            cmd->activeSerial->print(F("{\"wheel_speed\": {\"fl\":"));
            float speed_ = (((msg.frame_data[0] << 8) + msg.frame_data[1]) - 10000) / 100.0;
            cmd->activeSerial->print(speed_, 4);
            cmd->activeSerial->print(F(",\"fr\":"));
            speed_ = (((msg.frame_data[2] << 8) + msg.frame_data[3]) - 10000) / 100.0;
            cmd->activeSerial->print(speed_, 4);
            cmd->activeSerial->print(F(",\"rl\":"));
            speed_ = (((msg.frame_data[4] << 8) + msg.frame_data[5]) - 10000) / 100.0;
            cmd->activeSerial->print(F(",\"rr\":"));
            speed_ = (((msg.frame_data[6] << 8) + msg.frame_data[7]) - 10000) / 100.0;
            cmd->activeSerial->print(F("}}\n"));
            did = true;
            break;
          }
        case 0x4DA:
          {
            float column = (((msg.frame_data[0] << 8) + msg.frame_data[1]) - 32768) / 10.0;
            cmd->activeSerial->print(F("{\"steering\": {\"column\":"));
            cmd->activeSerial->print(column, 1);
            cmd->activeSerial->print(F("}}\n"));
            break;
          }
        case 0x4F2:
          {
            cmd->activeSerial->print(F("{\"odometer\": {\"range\":"));
            cmd->activeSerial->print(msg.frame_data[0], DEC);
            cmd->activeSerial->print(F(",\"val\":"));
            cmd->activeSerial->print((msg.frame_data[1] << 8) + msg.frame_data[2], DEC);
            cmd->activeSerial->print(F("}}\n"));
            break;
          }
      }
      break;
  }
  return !did;
}
