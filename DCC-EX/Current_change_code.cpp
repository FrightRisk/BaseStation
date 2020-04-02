/*
EXAMPLES:
sprintf(c,"a %d %d %d",data.address,data.subAddress,data.tStatus);
printf("Hi %c %d %s", 'c', 10, "there!");
CommManager::printf("<H %d %d>", data.id, data.tStatus);
CommManager::printf("<H %d %d %d %d>", tt->data.id, tt->data.address, tt->data.subAddress, tt->data.tStatus);
CommManager::printf("<p1 %s>", name);
*/

// **** CurrentMonitor.h ****
class MotorBoard {
public:
	MotorBoard(int sensePin, int enablePin, MOTOR_BOARD_TYPE type, const char *name);
	void check();
	void powerOn(bool announce=true);
	void powerOff(bool announce=true, bool overCurrent=false);
	int getLastRead();
	// const char *getLastRead(); fnd ??
	void showStatus();
	const char *getName() {
		return name;
	}
private:
	int sensePin;
	int enablePin;
	const char *name;
	float current;
	float reading;        // fnd added
	bool triggered;
	long int lastCheckTime;
	int triggerMilliamps; // fnd renamed from triggerValue
	int maxMilliAmps;     // fnd added
};

// **** SerialCommand.cpp ****
  case 'c':
    if(strlen(com) == 1) {
      // NEWCURRENT going to need something like this:
      CommManager::printf("<a %d>", boards[0]->getLastRead());
	  // CommManager::printf("<a %s>, boards[0]->getLastRead());
    } else {
        for(int i = 0; i < MAX_MOTOR_BOARDS; i++) {
	  if(boards[i] != NULL && strcasecmp(boards[i]->getName(), com+2) == 0) {
            // fnd - must change the below line
	    CommManager::printf("<a %s %d>", boards[i]->getName(), boards[i]->getLastRead());
	    return;
	  }
          CommManager::printf("<X>");
     }
     break;
	 
// **** CurrentMonitor.cpp ****
int MotorBoard::getLastRead() {  \
// ****** Why is this a function? Is it scope? So we can call it from serialcommand.cpp?
// or is it just factoring because it is called several times?
// NEWCURRENT
// Maintaining compatibility with JMRI for now so must return "reading" which is the raw pin value from
// the current sense pin. When we issue a PR for JMRI, we can change it to the line below.
//	return reading, current, triggerMilliamps, maxMilliAmps
	return reading;
}

//Change the above function to something like this? But C can't return multiple values. Would need pointers or
// to parse a string
const char MotorBoard::getLastRead() {
	return ("%d %d %d %d"), reading, current, triggerMilliamps, maxMilliamps);
}

// **** CommInterface.cpp ****
void CommManager::printf(const char *fmt, ...) {
	char buf[256] = {0};
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	for(int i = 0; i < nextInterface; i++) {
		if(interfaces[i] != NULL) {
			interfaces[i]->send(buf);
		}
	}
}
