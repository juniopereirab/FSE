#include <stdio.h>
#include <unistd.h>

#include "gpio.h"
#include "mainMenu.h"
#include "alarm.h"
#include "tcpClient.h"
#include "application.h"

DevicesOut _devOut;
DevicesIn _devIn;

void dataInit() {
  _devOut.lsT01 = 0;
  _devOut.lsT02 = 0;
  _devOut.lcT = 0;
  _devOut.acT = 0;
  _devOut.asp = 0;
  _devOut.ls101 = 0;
  _devOut.ls102 = 0;
  _devOut.lc1 = 0;
  _devOut.ac1 = 0;
  _devOut.alarm = 0;
  _devOut.alarmPlaying = 0;

  _devIn.spT = 0;
  _devIn.sfT = 0;
  _devIn.sjT01 = 0;
  _devIn.sjT02 = 0;
  _devIn.spoT = 0;
  _devIn.scIn = 0;
  _devIn.scOut = 0;
  _devIn.sp1 = 0;
  _devIn.sf1 = 0;
  _devIn.sj101 = 0;
  _devIn.sj102 = 0;
  _devIn.peopleQuantity = 0;

  Data data = currentData();
  data.devOut = _devOut;
  printData(data);
  printDevicesIn(_devIn);
}

void devices_in_handler(int command) {
  if (command == SP_T) {
      if(_devIn.spT) {
        _devIn.spT = 0;
      }
      else {
          _devIn.spT = 1;
      }
  }

  else if (command == LC_T) {
    Data dev10sData = currentData();
    if(_devOut.lcT){
        _devOut.lcT = 0;
    }
    else {
        _devOut.lcT = 1;
    }
    dev10sData.devOut = _devOut;
    printData(dev10sData);
  }

  else if (command == SF_T) {
    Data data = currentData();

    if (_devIn.sfT == 0) {
      _devIn.sfT = 1;
      _devOut.alarm = 1;
      data.devOut = _devOut;
      printData(data);
    } else {
      _devIn.sfT = 0;
      _devOut.alarm = 0;
      alarmOff();
      alarmHandler();
      data.devOut = _devOut;
      printData(data);
    }
  }

  else if (command == ASP) {
    Data devOutFire = currentData();
    if(_devOut.asp){
        _devOut.asp = 0;
    }
    else {
        _devOut.asp = 1;
    }
    devOutFire.devOut = _devOut;
    printData(devOutFire);
  }

  else if (command == SJ_T01) {
    if(_devIn.sjT01) {
        _devIn.sjT01 = 0;
    }
    else {
        _devIn.sjT01 = 1;
    }
  }

  else if (command == SJ_T02) {
    if(_devIn.sjT02){
        _devIn.sjT02 = 0;
    }
    else {
        _devIn.sjT02 = 1;
    }
  }

  else if (command == SPo_T) {
    if(_devIn.spoT){
        _devIn.spoT = 0;
    }
    else {
        _devIn.spoT = 1;
    }
  }

  else if (command == SC_IN) {
    if (_devIn.peopleQuantity < 100)
      _devIn.peopleQuantity += 1;
  }

  else if (command == SC_OUT) {
    if (_devIn.peopleQuantity > 0)
      _devIn.peopleQuantity -= 1;
  }

  else if (command == SP_1) {
    if(_devIn.sp1){
        _devIn.sp1 = 0;
    }
    else {
        _devIn.sp1 = 1;
    }
  }

  else if (command == LC_1) {
    Data dev10sData = currentData();
    if(_devOut.lc1){
        _devOut.lc1 = 0;
    }
    else {
        _devOut.lc1 = 1;
    }
    dev10sData.devOut = _devOut;
    printData(dev10sData);
  }

  else if (command == SF_1) {
    Data data = currentData();

    if (_devIn.sf1 == 0) {
      _devIn.sf1 = 1;
      _devOut.alarm = 1;
      data.devOut = _devOut;
      printData(data);
    } else {
      _devIn.sf1 = 0;
      _devOut.alarm = 0;
      alarmOff();
      alarmHandler();
      data.devOut = _devOut;
      printData(data);
    }
  }

  else if (command == SJ_101) {
    if(_devIn.sj101) {
        _devIn.sj101 = 0;
    } 
    else {
        _devIn.sj101 = 1;
    }
  }

  else if (command == SJ_102) {
      if(_devIn.sj102) {
          _devIn.sj102 = 0;
      }
      else {
          _devIn.sj102 = 1;
      }
  }

  if (_devIn.spT == 1 || _devIn.sfT == 1 || _devIn.sjT01 == 1 ||
    _devIn.sjT02 == 1 || _devIn.spoT == 1 || _devIn.sp1 == 1 ||
    _devIn.sf1 == 1 || _devIn.sj101 == 1 || _devIn.sj102 == 1 ) {
    alarmHandler();
  }
  else {
    Data data = currentData();
    _devOut.alarmPlaying = 0;
    alarmOff();
    data.devOut = _devOut;
    printData(data);
  }
  
  printDevicesIn(_devIn);
}

void storeDevicesOutUpdate(DevicesOut devOutUpdated) {
  if (devOutUpdated.alarm == 0) {
    devOutUpdated.alarmPlaying = 0;
    alarmOff();
  }

  if (devOutUpdated.alarm == 1 && (
    _devIn.spT == 1 || _devIn.sjT01 == 1 || _devIn.sjT02 == 1 ||
    _devIn.spoT == 1 || _devIn.sp1 == 1 || _devIn.sj101 == 1 ||
    _devIn.sj102 == 1)) {
    devOutUpdated.alarmPlaying = 1;
  }

  Data data = currentData();
  data.devOut = devOutUpdated;
  print_data(data);

  int res = 1;
  if (data.devOut.lsT01 != _devOut.lsT01) {
    res = sendCommand(LS_T01, data.devOut.lsT01, 0);
  }

  else if (data.devOut.lsT02 != _devOut.lsT02) {
    res = sendCommand(LS_T02, data.devOut.lsT02, 0);
  }

  else if (data.devOut.lcT != _devOut.lcT) {
    res = sendCommand(LC_T, data.devOut.lcT, 0);
  }

  else if (data.devOut.acT != _devOut.acT) {
    res = sendCommand(AC_T, data.devOut.acT, 0);
  }

  else if (data.devOut.asp != _devOut.asp) {
    res = sendCommand(ASP, data.devOut.asp, 0);
  }

  else if (data.devOut.ls101 != _devOut.ls101) {
    res = sendCommand(LS_101, data.devOut.ls101, 1);
  }

  else if (data.devOut.ls102 != _devOut.ls102) {
    res = sendCommand(LS_102, data.devOut.ls102, 1);
  }

  else if (data.devOut.lc1 != _devOut.lc1) {
    res = sendCommand(LC_1, data.devOut.lc1, 1);
  }

  else if (data.devOut.ac1 != _devOut.ac1) {
    res = sendCommand(AC_1, data.devOut.ac1, 1);
  }

  if (res == 1) {
    _devOut = devOutUpdated;
  }
}

DevicesOut recoverDevicesOutData() {
  return _devOut;
}
