#include "mbed.h"


//===============================================
//METHOD :  Communication
//DESCR. :  Connection to extern units
//===============================================
class Communication
{
public:
	Communication(int type);                        //Type of communication
	void sendInformation(int);
	int getInformation(void);                       //Last recieved information

private:
	//---------Privat funktioner---------
	void receiveInformation(void);
	void sendToValidation(void);

	//-------------Variables-------------
	int _rxInfo;
	bool _toogle;

};