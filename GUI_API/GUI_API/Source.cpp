#include "Rendering.h"
#include "Direction.h"
#include "Validation.h"
#include "Communication.h"

int main() {

	//--------Oprettelse af konstnate variabler---------
	//Skal være der:
	const int width = 479;                         //I pixels
	const int height = 272;
	int boxWidth = 5;                             //bredden på borderen
	int maxNumberOfRow = 3;                  //Maks antal objekter på 1 række
	int gyro = 0;                                  //Skal først bruges senere - hvis det bliver brugbart. - ikke const
	const int colorOfBorder = 0x00F700D2; //0xFFF700D2;          //Starte med 0xFF og SKAL bestå af 6 cifre her efter
	const int colorOfInnerBox = 0xFFFFFFFF;        //Standart color is black
	const int colorOfText = 0xFF000000;            //#hexColor #google
	const int colorOfBackground = 0xFFFFFFFF;      //FFFFFF : WHITE - 000000 : BLACK 

												   //Efter antal sider.
	int keypad = 10;                   //Flere intercases kan tilføjes
	int mainmenuOff = 3;              //Gør dem ikke const, men isted klasserne konst?
	int mainmenuOn = 2;
	int wrongCode = 1;
	int rightCode = 1;
	int simuleringOff = 1;
	int simuleringOn = 1;
	int startScreen = 5;
	int sleepMode = 1;
	int serverFail = 1;


	//----------oprettelse af engangs klasser-----------
	Rendering           calc(width, height, &boxWidth, &maxNumberOfRow, gyro);           //Bliver først deconstructet når programmet afsluttes
	Direction           direction;
	Validation          vali;
	Communication       COM(1);


	//-------------- oprettelse af tekst ---------------       
	char *keypadText[] =
	{
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9"
	};

	char *mainmenuOffText[] =
	{
		"Simulering (automatisk)",
		"Simulering (manuel)",
		"Exit"
	};

	char *mainmenuOnText[] =
	{
		"Simulering Off",
		"Exit"
	};

	char *wrongCodeText[] =
	{
		"WRONG   CODE"
	};

	char *rightCodeText[] =
	{
		"RIGHT   CODE"
	};

	char *simuleringOffText[] =
	{
		"SIMULERING OFF"
	};

	char *simuleringOnText[] =
	{
		"SIMULERING ON"
	};

	char *startScreenText[] =
	{
		"",
		"Welcome to",
		"Group 10's",
		"Smarthome!!",
		""
	};

	char *sleepModeText[] =
	{
		""
	};

	char *serverFailText[] =
	{
		"CAN'T CONNECT TO THE SERVER =("
	};


	//--------------Rendering and create--------------                                             //----------------------setup-----------------------
	calc.createUI(&keypad, keypadText, 2);                                                             //There will not be a fic
	calc.createUI(&mainmenuOff, mainmenuOffText);
	calc.createUI(&mainmenuOn, mainmenuOnText);
	calc.createUI(&simuleringOff, simuleringOffText);
	calc.createUI(&simuleringOn, simuleringOnText);
	calc.createUI(&wrongCode, wrongCodeText);
	calc.createUI(&rightCode, rightCodeText);
	calc.createUI(&startScreen, startScreenText, 5);
	calc.createUI(&sleepMode, sleepModeText, 1);
	calc.createUI(&serverFail, serverFailText);


	//------------------------------- PROGRAM -------------------------------       //without Direction lib

	int dir = 0;                //0. keypad - 1. rightCode - 2. wrongCode - 3. mainmenuOFF - 4. mainmenuOn - 5. simulationOn - 6. simulationOff - 7. sleep - 8. communication
	int UIoptimation = 0;       //9. serverFail.
	int sleep = 0;              //If sleep hits 100 it will log out.
	float waitTime = 2;
	int sendInfo;


	calc.setBorderColor(colorOfBorder);
	calc.setBackgroundColor(0xFF000000);
	calc.setTextColor(0xFF257F18);
	calc.setInnerBoxColor(0xFF000000);

	while (true)                         //in the end of every cyclus, if will check if there is response from the database
	{

		if (dir == 0)
		{
			if (UIoptimation == 0)
			{
				calc.setTextColor(0xFF000000);
				calc.setBackgroundColor(0xFFFFFFFF);
				calc.setBorderColor(colorOfBorder);
				calc.setInnerBoxColor(0xFFFFFFFF);
				calc.showUI(&keypad);

				UIoptimation++;
			}

			int touch = calc.getTouch(1);

			if (touch > 0)
			{
				int rightOrWrong = vali.setValue(touch);         //We have a wait on 0.2 seconds another place

				sleep = 0;

				if (rightOrWrong == 1)
				{
					dir = 1;
					UIoptimation = 0;
					//calc.editUIText(&keypad, "--", 10);         //This one will not edit the boxes after the function is finished
				}
				else if (rightOrWrong == 2)
				{
					dir = 2;
					UIoptimation = 0;
					//calc.editUIText(&keypad, "--", 10);
				}
			}
		}


		else if (dir == 1)
		{

			calc.setTextColor(0xFF000000);
			calc.setBackgroundColor(0xFFFFFFFF);
			calc.setBorderColor(0xFFFFFFFF);
			calc.setInnerBoxColor(0xFFFFFFFF);
			calc.showUI(&rightCode);
			wait(waitTime);                     //Would be better using the power to check at the same time.
			UIoptimation = 0;

			sleep = 0;
			sendInfo = 10;
			dir = 8;    //Call the server function here, later on;
		}


		else if (dir == 2)
		{

			calc.setTextColor(0xFF000000);
			calc.setBackgroundColor(0xFFFFFFFF);
			calc.setBorderColor(0xFFFFFFFF);
			calc.setInnerBoxColor(0xFFFFFFFF);
			calc.showUI(&wrongCode);
			wait(waitTime);
			UIoptimation = 0;

			sleep = 0;
			dir = 0;
		}

		else if (dir == 3)
		{
			if (UIoptimation == 0)
			{
				calc.setTextColor(0xFF000000);
				calc.setBackgroundColor(0xFFFFFFFF);
				calc.setBorderColor(colorOfBorder);
				calc.setInnerBoxColor(0xFFFFFFFF);
				calc.showUI(&mainmenuOn);

				UIoptimation++;
			}

			if (calc.getTouch(0))
			{
				sleep = 0;
				UIoptimation = 0;

				if (calc.getTouch(0) == 1)
				{
					dir = 8;    //Call the server function here, later on. 
					dir = 6;
				}
				else if (calc.getTouch(0) == 2)
				{
					dir = 0;
				}
			}
		}

		else if (dir == 4)
		{
			if (UIoptimation == 0)
			{
				calc.setTextColor(0xFF000000);
				calc.setBackgroundColor(0xFFFFFFFF);
				calc.setBorderColor(colorOfBorder);
				calc.setInnerBoxColor(0xFFFFFFFF);
				calc.showUI(&mainmenuOff);

				UIoptimation++;
			}

			if (calc.getTouch(0))
			{
				sleep = 0;
				UIoptimation = 0;

				if (calc.getTouch(0) == 1)
				{
					dir = 8;    //Call the server function here, later on. 
					dir = 5;
				}
				else if (calc.getTouch(0) == 2)
				{
					dir = 8;
					dir = 5;
				}
				else if (calc.getTouch(0) == 3)
				{
					dir = 0;
				}
			}
		}

		else if (dir == 5)
		{

			calc.setTextColor(0xFF000000);
			calc.setBackgroundColor(0xFFFFFFFF);
			calc.setBorderColor(0xFFFFFFFF);
			calc.setInnerBoxColor(0xFFFFFFFF);
			calc.showUI(&simuleringOn);
			wait(waitTime);
			UIoptimation = 0;

			sleep = 0;
			dir = 0;
		}

		else if (dir == 6)
		{

			calc.setTextColor(0xFF000000);
			calc.setBackgroundColor(0xFFFFFFFF);
			calc.setBorderColor(0xFFFFFFFF);
			calc.setInnerBoxColor(0xFFFFFFFF);
			calc.showUI(&simuleringOff);
			wait(waitTime);
			UIoptimation = 0;

			sleep = 0;
			dir = 0;
		}
		else if (dir == 7)
		{
			if (UIoptimation == 0)
			{
				calc.setTextColor(0xFF000000);
				calc.setBackgroundColor(0xFF000000);
				calc.setBorderColor(0xFF000000);
				calc.setInnerBoxColor(0xFF000000);
				calc.showUI(&sleepMode);
			}

			if (calc.getTouch(0))
			{
				UIoptimation = 0;

				sleep = 0;
				dir = 0;
			}
		}

		//wait(0.1);
		//sleep++;

		if (sleep >= 50)
		{
			//sleep--;
			dir = 7;
		}

		if (dir == 8)
		{
			COM.sendInformation(sendInfo);

			int info = COM.getInformation();

			if (info)
			{
				sleep = 0;
				UIoptimation = 0;

				if (info == 1)
				{
					dir = 3;
				}
				else if (info == 2)
				{
					dir = 4;
				}
			}
		}


	}
}