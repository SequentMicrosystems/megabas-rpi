/*
 * rtd.c:
 *	Command-line interface to the Raspberry
 *	Pi's MEGAS-RTD board.
 *	Copyright (c) 2016-2020 Sequent Microsystem
 *	<http://www.sequentmicrosystem.com>
 ***********************************************************************
 *	Author: Alexandru Burcea
 ***********************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "megabas.h"
#include "comm.h"
#include "thread.h"

#define VERSION_BASE	(int)1
#define VERSION_MAJOR	(int)2
#define VERSION_MINOR	(int)2

#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */

char *warranty =
	"	       Copyright (c) 2016-2020 Sequent Microsystems\n"
		"                                                             \n"
		"		This program is free software; you can redistribute it and/or modify\n"
		"		it under the terms of the GNU Leser General Public License as published\n"
		"		by the Free Software Foundation, either version 3 of the License, or\n"
		"		(at your option) any later version.\n"
		"                                    \n"
		"		This program is distributed in the hope that it will be useful,\n"
		"		but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
		"		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
		"		GNU Lesser General Public License for more details.\n"
		"			\n"
		"		You should have received a copy of the GNU Lesser General Public License\n"
		"		along with this program. If not, see <http://www.gnu.org/licenses/>.";

void usage(void)
{
	int i = 0;
	while (gCmdArray[i] != NULL)
	{
		if (gCmdArray[i]->name != NULL)
		{
			if (strlen(gCmdArray[i]->usage1) > 2)
			{
				printf("%s", gCmdArray[i]->usage1);
			}
			if (strlen(gCmdArray[i]->usage2) > 2)
			{
				printf("%s", gCmdArray[i]->usage2);
			}
		}
		i++;
	}
	printf("Where: <id> = Board level id = 0..7\n");
	printf("Type megabas -h <command> for more help\n");
}

int doBoardInit(int stack)
{
	int dev = 0;
	int add = 0;
	uint8_t buff[8];

	if ( (stack < 0) || (stack > 7))
	{
		printf("Invalid stack level [0..7]!");
		return ERROR;
	}
	add = stack + SLAVE_OWN_ADDRESS_BASE;
	dev = i2cSetup(add);
	if (dev == -1)
	{
		return ERROR;
	}
	if (ERROR == i2cMem8Read(dev, I2C_REVISION_MAJOR_MEM_ADD, buff, 1))
	{
		printf("MEGA-BAS id %d not detected\n", stack);
		return ERROR;
	}
	return dev;
}

int boardCheck(int stack)
{
	int dev = 0;
	int add = 0;
	uint8_t buff[8];

	if ( (stack < 0) || (stack > 7))
	{
		printf("Invalid stack level [0..7]!");
		return ERROR;
	}
	add = stack + SLAVE_OWN_ADDRESS_BASE;
	dev = i2cSetup(add);
	if (dev == -1)
	{
		return ERROR;
	}
	if (ERROR == i2cMem8Read(dev, I2C_REVISION_MAJOR_MEM_ADD, buff, 1))
	{

		return ERROR;
	}
	return OK;
}
static void doHelp(int argc, char *argv[]);
const CliCmdType CMD_HELP =
	{
		"-h",
		1,
		&doHelp,
		"\t-h		Display the list of command options or one command option details\n",
		"\tUsage:		megabas -h    Display command options list\n",
		"\tUsage:		megabas -h <param>   Display help for <param> command option\n",
		"\tExample:		megabas -h rread    Display help for \"rread\" command option\n"};

static void doHelp(int argc, char *argv[])
{
	int i = 0;
	if (argc == 3)
	{
		while (NULL != gCmdArray[i])
		{
			if (gCmdArray[i]->name != NULL)
			{
				if (strcasecmp(argv[2], gCmdArray[i]->name) == 0)
				{
					printf("%s%s%s%s", gCmdArray[i]->help, gCmdArray[i]->usage1,
						gCmdArray[i]->usage2, gCmdArray[i]->example);
					break;
				}
			}
			i++;
		}
		if (NULL == gCmdArray[i])
		{
			printf("Option \"%s\" not found\n", argv[2]);
			i = 0;
			while (NULL != gCmdArray[i])
			{
				if (gCmdArray[i]->name != NULL)
				{
					printf("%s", gCmdArray[i]->help);
					break;
				}
				i++;
			}
		}
	}
	else
	{
		i = 0;
		while (NULL != gCmdArray[i])
		{
			if (gCmdArray[i]->name != NULL)
			{
				printf("%s", gCmdArray[i]->help);
			}
			i++;
		}
	}
}

static void doVersion(int argc, char *argv[]);
const CliCmdType CMD_VERSION =
{
	"-v",
	1,
	&doVersion,
	"\t-v		Display the megabas command version number\n",
	"\tUsage:		megabas -v\n",
	"",
	"\tExample:		megabas -v  Display the version number\n"};

static void doVersion(int argc, char *argv[])
{
	UNUSED(argc);
	UNUSED(argv);
	printf("megabas v%d.%d.%d Copyright (c) 2016 - 2020 Sequent Microsystems\n",
	VERSION_BASE, VERSION_MAJOR, VERSION_MINOR);
	printf("\nThis is free software with ABSOLUTELY NO WARRANTY.\n");
	printf("For details type: megabas -warranty\n");

}

static void doWarranty(int argc, char* argv[]);
const CliCmdType CMD_WAR =
{
	"-warranty",
	1,
	&doWarranty,
	"\t-warranty	Display the warranty\n",
	"\tUsage:		megabas -warranty\n",
	"",
	"\tExample:		megabas -warranty  Display the warranty text\n"};

static void doWarranty(int argc UNU, char* argv[] UNU)
{
	printf("%s\n", warranty);
}

static void doList(int argc, char *argv[]);
const CliCmdType CMD_LIST =
	{
		"-list",
		1,
		&doList,
		"\t-list:		List all megabas boards connected\n\t\t\treturn the # of boards and stack level for every board\n",
		"\tUsage:		megabas -list\n",
		"",
		"\tExample:		megabas -list display: 1,0 \n"};

static void doList(int argc, char *argv[])
{
	int ids[8];
	int i;
	int cnt = 0;

	UNUSED(argc);
	UNUSED(argv);

	for (i = 0; i < 8; i++)
	{
		if (boardCheck(i) == OK)
		{
			ids[cnt] = i;
			cnt++;
		}
	}
	printf("%d board(s) detected\n", cnt);
	if (cnt > 0)
	{
		printf("Id:");
	}
	while (cnt > 0)
	{
		cnt--;
		printf(" %d", ids[cnt]);
	}
	printf("\n");
}

static void doBoard(int argc, char *argv[]);
const CliCmdType CMD_BOARD =
{
	"board",
	2,
	&doBoard,
	"\tboard		Display the board status and firmware version number\n",
	"\tUsage:		megabas <stack> board\n",
	"",
	"\tExample:		megabas 0 board  Display vcc, temperature, firmware version \n"};

static void doBoard(int argc, char *argv[])
{
	int dev = -1;
	u8 buff[5];
	int resp = 0;
	int temperature = 25;
	float vIn = 24;
	float vRasp = 5;

	if (argc != 3)
	{
		printf("Invalid arguments number type \"megabas -h\" for details\n");
		exit(1);
	}
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}
	resp = i2cMem8Read(dev, I2C_DIAG_TEMPERATURE_MEM_ADD, buff, 5);
	if (FAIL == resp)
	{
		printf("Fail to read board info!\n");
		exit(1);
	}
	temperature = buff[0];
	memcpy(&resp, &buff[1], 2);
	vIn = (float)resp / 1000; //read in milivolts

	memcpy(&resp, &buff[3], 2);
	vRasp = (float)resp / 1000; //read in milivolts	

	resp = i2cMem8Read(dev, I2C_REVISION_MAJOR_MEM_ADD, buff, 2);
	if (FAIL == resp)
	{
		printf("Fail to read board info!\n");
		exit(1);
	}
	printf(
		"Firmware ver %02d.%02d, CPU temperature %d C, Power source %0.2f V, Raspberry %0.2f V\n",
		(int)buff[0], (int)buff[1], temperature, vIn, vRasp);
}

int triacChSet(int dev, u8 channel, OutStateEnumType state)
{
	int resp = 0;
	u8 buff[2];

	if ( (channel < CHANNEL_NR_MIN) || (channel > TRIAC_CH_NR_MAX))
	{
		printf("Invalid triac nr!\n");
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_TRIACS_VAL_ADD, buff, 1))
	{
		return FAIL;
	}

	switch (state)
	{
	case OFF:
		buff[0] &= ~ (1 << (channel - 1));
		resp = i2cMem8Write(dev, I2C_TRIACS_VAL_ADD, buff, 1);
		break;
	case ON:
		buff[0] |= 1 << (channel - 1);
		resp = i2cMem8Write(dev, I2C_TRIACS_VAL_ADD, buff, 1);
		break;
	default:
		printf("Invalid triac state!\n");
		return ERROR;
		break;
	}
	return resp;
}

int triacChGet(int dev, u8 channel, OutStateEnumType* state)
{
	u8 buff[2];

	if (NULL == state)
	{
		return ERROR;
	}

	if ( (channel < CHANNEL_NR_MIN) || (channel > TRIAC_CH_NR_MAX))
	{
		printf("Invalid triac nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_TRIACS_VAL_ADD, buff, 1))
	{
		return ERROR;
	}

	if (buff[0] & (1 << (channel - 1)))
	{
		*state = ON;
	}
	else
	{
		*state = OFF;
	}
	return OK;
}

int triacSet(int dev, int val)
{
	u8 buff[2];

	buff[0] = 0x0f & val;

	return i2cMem8Write(dev, I2C_TRIACS_VAL_ADD, buff, 1);
}

int triacGet(int dev, int* val)
{
	u8 buff[2];

	if (NULL == val)
	{
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_TRIACS_VAL_ADD, buff, 1))
	{
		return ERROR;
	}
	*val = buff[0];
	return OK;
}

static void doTriacWrite(int argc, char *argv[]);
const CliCmdType CMD_TRIAC_WRITE =
{
	"trwr",
	2,
	&doTriacWrite,
	"\ttrwr:		Set triacs (AC switch) On/Off\n",
	"\tUsage:		megabas <id> trwr <channel> <on/off>\n",
	"\tUsage:		megabas <id> trwr <value>\n",
	"\tExample:		megabas 0 trwr 2 1; Set Triac #2 on Board #0 On\n"};

static void doTriacWrite(int argc, char *argv[])
{
	int pin = 0;
	OutStateEnumType state = STATE_COUNT;
	int val = 0;
	int dev = 0;
	OutStateEnumType stateR = STATE_COUNT;
	int valR = 0;
	int retry = 0;

	if ( (argc != 5) && (argc != 4))
	{
		printf("%s", CMD_TRIAC_WRITE.usage1);
		printf("%s", CMD_TRIAC_WRITE.usage2);
		exit(1);
	}

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}
	if (argc == 5)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > TRIAC_CH_NR_MAX))
		{
			printf("Triac channel number value out of range\n");
			exit(1);
		}

		/**/if ( (strcasecmp(argv[4], "up") == 0)
			|| (strcasecmp(argv[4], "on") == 0))
			state = ON;
		else if ( (strcasecmp(argv[4], "down") == 0)
			|| (strcasecmp(argv[4], "off") == 0))
			state = OFF;
		else
		{
			if ( (atoi(argv[4]) >= STATE_COUNT) || (atoi(argv[4]) < 0))
			{
				printf("Invalid triac state!\n");
				exit(1);
			}
			state = (OutStateEnumType)atoi(argv[4]);
		}

		retry = RETRY_TIMES;

		while ( (retry > 0) && (stateR != state))
		{
			if (OK != triacChSet(dev, pin, state))
			{
				printf("Fail to write triac\n");
				exit(1);
			}
			if (OK != triacChGet(dev, pin, &stateR))
			{
				printf("Fail to read triac\n");
				exit(1);
			}
			retry--;
		}
#ifdef DEBUG_I
		if(retry < RETRY_TIMES)
		{
			printf("retry %d times\n", 3-retry);
		}
#endif
		if (retry == 0)
		{
			printf("Fail to write triac\n");
			exit(1);
		}
	}
	else
	{
		val = atoi(argv[3]);
		if (val < 0 || val > 0x0f)
		{
			printf("Invalid triac value\n");
			exit(1);
		}

		retry = RETRY_TIMES;
		valR = -1;
		while ( (retry > 0) && (valR != val))
		{

			if (OK != triacSet(dev, val))
			{
				printf("Fail to write triac!\n");
				exit(1);
			}
			if (OK != triacGet(dev, &valR))
			{
				printf("Fail to read triac!\n");
				exit(1);
			}
		}
		if (retry == 0)
		{
			printf("Fail to write triac!\n");
			exit(1);
		}
	}
}

static void doTriacRead(int argc, char *argv[]);
const CliCmdType CMD_TRIAC_READ =
{
	"trrd",
	2,
	&doTriacRead,
	"\ttrrd:		Read triacs (AC switch) status\n",
	"\tUsage:		megabas <id> trrd <channel>\n",
	"\tUsage:		megabas <id> trrd\n",
	"\tExample:		megabas 0 trrd 2; Read Status of Triac #2 on Board #0\n"};

static void doTriacRead(int argc, char *argv[])
{
	int pin = 0;
	int val = 0;
	int dev = 0;
	OutStateEnumType state = STATE_COUNT;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > TRIAC_CH_NR_MAX))
		{
			printf("Triac channel number value out of range!\n");
			exit(1);
		}

		if (OK != triacChGet(dev, pin, &state))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		if (state != 0)
		{
			printf("1\n");
		}
		else
		{
			printf("0\n");
		}
	}
	else if (argc == 3)
	{
		if (OK != triacGet(dev, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("%d\n", val);
	}
	else
	{
		printf("%s", CMD_TRIAC_READ.usage1);
		printf("%s", CMD_TRIAC_READ.usage2);
		exit(1);
	}
}

static void doTriacTest(int argc, char* argv[]);
const CliCmdType CMD_TEST =
{
	"trtest",
	2,
	&doTriacTest,
	"\ttrtest:		Turn ON and OFF the triacs until press a key\n",
	"\tUsage:		megabas <id> trtest\n",
	"",
	"\tExample:		megabas 0 trtest\n"};

static void doTriacTest(int argc, char* argv[])
{
	int dev = 0;
	int i = 0;
	int retry = 0;
	int trVal;
	int valR;
	int triacResult = 0;
	FILE* file = NULL;
	const u8 triacOrder[4] =
	{
		1,
		2,
		3,
		4, };

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}
	if (argc == 4)
	{
		file = fopen(argv[3], "w");
		if (!file)
		{
			printf("Fail to open result file\n");
			//return -1;
		}
	}
//triac test****************************
	if (strcasecmp(argv[2], "trtest") == 0)
	{
		trVal = 0;
		printf(
			"Are all triacs and LEDs turning on and off in sequence?\nPress y for Yes or any key for No....");
		startThread();
		while (triacResult == 0)
		{
			for (i = 0; i < 4; i++)
			{
				triacResult = checkThreadResult();
				if (triacResult != 0)
				{
					break;
				}
				valR = 0;
				trVal = (u8)1 << (triacOrder[i] - 1);

				retry = RETRY_TIMES;
				while ( (retry > 0) && ( (valR & trVal) == 0))
				{
					if (OK != triacChSet(dev, triacOrder[i], ON))
					{
						retry = 0;
						break;
					}

					if (OK != triacGet(dev, &valR))
					{
						retry = 0;
					}
				}
				if (retry == 0)
				{
					printf("Fail to write triac\n");
					if (file)
						fclose(file);
					exit(1);
				}
				busyWait(150);
			}

			for (i = 0; i < 4; i++)
			{
				triacResult = checkThreadResult();
				if (triacResult != 0)
				{
					break;
				}
				valR = 0xff;
				trVal = (u8)1 << (triacOrder[i] - 1);
				retry = RETRY_TIMES;
				while ( (retry > 0) && ( (valR & trVal) != 0))
				{
					if (OK != triacChSet(dev, triacOrder[i], OFF))
					{
						retry = 0;
					}
					if (OK != triacGet(dev, &valR))
					{
						retry = 0;
					}
				}
				if (retry == 0)
				{
					printf("Fail to write triac!\n");
					if (file)
						fclose(file);
					exit(1);
				}
				busyWait(150);
			}
		}
	}
	else
	{
		usage();
		exit(1);
	}
	if (triacResult == YES)
	{
		if (file)
		{
			fprintf(file, "Triac Test ............................ PASS\n");
		}
		else
		{
			printf("Triac Test ............................ PASS\n");
		}
	}
	else
	{
		if (file)
		{
			fprintf(file, "Triac Test ............................ FAIL!\n");
		}
		else
		{
			printf("Triac Test ............................ FAIL!\n");
		}
	}
	if (file)
	{
		fclose(file);
	}
	triacSet(dev, 0);
}

int contactChGet(int dev, u8 channel, OutStateEnumType* state)
{
	u8 buff[2];

	if (NULL == state)
	{
		return ERROR;
	}

	if ( (channel < CHANNEL_NR_MIN) || (channel > CONTACT_CH_NR_MAX))
	{
		printf("Invalid DRY CONTACT nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_DRY_CONTACT_VAL_ADD, buff, 1))
	{
		return ERROR;
	}

	if (buff[0] & (1 << (channel - 1)))
	{
		*state = ON;
	}
	else
	{
		*state = OFF;
	}
	return OK;
}

int contactGet(int dev, int* val)
{
	u8 buff[2];

	if (NULL == val)
	{
		return ERROR;
	}
	if (FAIL == i2cMem8Read(dev, I2C_DRY_CONTACT_VAL_ADD, buff, 1))
	{
		return ERROR;
	}
	*val = buff[0];
	return OK;
}

int contactCountGet(int dev, u8 ch, u32* val)
{
	u8 buff[4];

	if (NULL == val)
	{
		return ERROR;
	}
	if ( (ch < CHANNEL_NR_MIN) || (ch > CONTACT_CH_NR_MAX))
	{
		printf("Invalid DRY CONTACT nr!\n");
		return ERROR;
	}
	if (FAIL
		== i2cMem8Read(dev, I2C_MEM_DRY_CONTACT_CONTORS + 4 * (ch - 1), buff, 4))
	{
		return ERROR;
	}
	memcpy(val, buff, 4);
	return OK;
}

int contactCountReset(int dev, u8 channel)
{
	u8 buff[4];

	if ( (channel < CHANNEL_NR_MIN) || (channel > CONTACT_CH_NR_MAX))
	{
		printf("Invalid DRY CONTACT nr!\n");
		return ERROR;
	}
	buff[0] = channel;
	if (FAIL == i2cMem8Write(dev, I2C_MEM_DRY_CONTACT_CH_CONT_RESET, buff, 1))
	{
		printf("Fail to reset contor! \n");
		return ERROR;
	}
	return OK;
}

int contactCountRisingSet(int dev, u8 channel, u8 state)
{
	u8 buff[4];

	if ( (channel < CHANNEL_NR_MIN) || (channel > CONTACT_CH_NR_MAX))
	{
		printf("Invalid DRY CONTACT nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_DRY_CONTACT_RISING_ENABLE, buff, 1))
	{
		return ERROR;
	}
	if (state != 0)
	{
		buff[0] |= 1 << (channel - 1);
	}
	else
	{
		buff[0] &= ~ (1 << (channel - 1));
	}
	if (FAIL == i2cMem8Write(dev, I2C_MEM_DRY_CONTACT_RISING_ENABLE, buff, 1))
	{
		return ERROR;
	}
	return OK;
}

int contactCountFallingSet(int dev, u8 channel, u8 state)
{
	u8 buff[4];

	if ( (channel < CHANNEL_NR_MIN) || (channel > CONTACT_CH_NR_MAX))
	{
		printf("Invalid DRY CONTACT nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_DRY_CONTACT_FALLING_ENABLE, buff, 1))
	{
		return ERROR;
	}
	if (state != 0)
	{
		buff[0] |= 1 << (channel - 1);
	}
	else
	{
		buff[0] &= ~ (1 << (channel - 1));
	}
	if (FAIL == i2cMem8Write(dev, I2C_MEM_DRY_CONTACT_FALLING_ENABLE, buff, 1))
	{
		return ERROR;
	}
	return OK;
}

int contactCountRisingGet(int dev, u8 channel, u8* state)
{
	u8 buff[4];

	if (NULL == state)
	{
		return ERROR;
	}
	if ( (channel < CHANNEL_NR_MIN) || (channel > CONTACT_CH_NR_MAX))
	{
		printf("Invalid DRY CONTACT nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_DRY_CONTACT_RISING_ENABLE, buff, 1))
	{
		return ERROR;
	}
	if ( (buff[0] & (1 << (channel - 1))) != 0)
	{
		*state = 1;
	}
	else
	{
		*state = 0;
	}
	return OK;
}

int contactCountFallingGet(int dev, u8 channel, u8* state)
{
	u8 buff[4];

	if (NULL == state)
	{
		return ERROR;
	}
	if ( (channel < CHANNEL_NR_MIN) || (channel > CONTACT_CH_NR_MAX))
	{
		printf("Invalid DRY CONTACT nr!\n");
		return ERROR;
	}

	if (FAIL == i2cMem8Read(dev, I2C_MEM_DRY_CONTACT_FALLING_ENABLE, buff, 1))
	{
		return ERROR;
	}
	if ( (buff[0] & (1 << (channel - 1))) != 0)
	{
		*state = 1;
	}
	else
	{
		*state = 0;
	}
	return OK;
}

static void doContactRead(int argc, char *argv[]);
const CliCmdType CMD_CONTACT_READ =
	{
		"contactrd",
		2,
		&doContactRead,
		"\tcontactrd:	Read dry contact status\n\t\t\tWarning: For this measurements to be valid place the jumper in position \"1K\" \n",
		"\tUsage:		megabas <id> contactrd <channel>\n",
		"\tUsage:		megabas <id> contactrd\n",
		"\tExample:		megabas 0 contactrd 2; Read Status of Dry contact pin #2 on Board #0\n"};

static void doContactRead(int argc, char *argv[])
{
	int pin = 0;
	int val = 0;
	int dev = 0;
	OutStateEnumType state = STATE_COUNT;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		pin = atoi(argv[3]);
		if ( (pin < CHANNEL_NR_MIN) || (pin > CONTACT_CH_NR_MAX))
		{
			printf("Dry contact number value out of range!\n");
			exit(1);
		}

		if (OK != contactChGet(dev, pin, &state))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		if (state != 0)
		{
			printf("1\n");
		}
		else
		{
			printf("0\n");
		}
	}
	else if (argc == 3)
	{
		if (OK != contactGet(dev, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("%d\n", val);
	}
	else
	{
		printf("%s", CMD_CONTACT_READ.usage1);
		printf("%s", CMD_CONTACT_READ.usage2);
		exit(1);
	}
}

static void doCountRead(int argc, char *argv[]);
const CliCmdType CMD_COUNTER_READ =
	{
		"countrd",
		2,
		&doCountRead,
		"\tcountrd:	Read dry contact transitions count\n\t\t\tWarning: For this measurements to be valid place the jumper in position \"1K\" \n",
		"\tUsage:		megabas <id> countrd <channel>\n",
		"",
		"\tExample:		megabas 0 countrd 2; Read transitions count of dry contact pin #2 on Board #0\n"};

static void doCountRead(int argc, char *argv[])
{
	u8 pin = 0;
	u32 val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		pin = (u8)atoi(argv[3]);

		if (OK != contactCountGet(dev, pin, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("%u\n", (unsigned int)val);
	}
	else
	{
		printf("%s", CMD_COUNTER_READ.usage1);
		exit(1);
	}
}

static void doCountReset(int argc, char *argv[]);
const CliCmdType CMD_COUNTER_RST =
	{
		"countrst",
		2,
		&doCountReset,
		"\tcountrst:	Reset dry contact transitions count\n",
		"\tUsage:		megabas <id> countrst <channel>\n",
		"",
		"\tExample:		megabas 0 countrst 2; Reset transitions count of dry contact pin #2 on Board #0\n"};

static void doCountReset(int argc, char *argv[])
{
	u8 pin = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		pin = (u8)atoi(argv[3]);

		if (OK != contactCountReset(dev, pin))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("done\n");
	}
	else
	{
		printf("%s", CMD_COUNTER_RST.usage1);
		exit(1);
	}
}

static void doEdgeRead(int argc, char *argv[]);
const CliCmdType CMD_EDGE_READ =
	{
		"edgerd",
		2,
		&doEdgeRead,
		"\tedgerd:		Read dry contact transitions type, ret 0 - disable, 1 - rising, 2 - falling, 3 - both\n",
		"\tUsage:		megabas <id> edgerd <channel> \n",
		"",
		"\tExample:		megabas 0 edgerd 2; Read transitions type of dry contact pin #2 on Board #0\n"};

static void doEdgeRead(int argc, char *argv[])
{
	u8 pin = 0;
	u8 rising = 0;
	u8 falling = 0;
	int dev = 0;
	

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		pin = (u8)atoi(argv[3]);

		if (OK != contactCountRisingGet(dev, pin, &rising))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		if (OK != contactCountFallingGet(dev, pin, &falling))
		{
			printf("Fail to read!\n");
			exit(1);
		}
		printf("%d\n", (int)(rising + falling * 2));
	}
	else
	{
		printf("%s", CMD_EDGE_READ.usage1);
		exit(1);
	}
}

static void doEdgeWrite(int argc, char *argv[]);
const CliCmdType CMD_EDGE_WRITE =
	{
		"edgewr",
		2,
		&doEdgeWrite,
		"\tedgewr:		Write dry contact transitions type: 0 - disable, 1 - rising, 2 - falling, 3 - both\n",
		"\tUsage:		megabas <id> edgewr <channel> <val>\n",
		"",
		"\tExample:		megabas 0 edgewr 2 1; Set transitions type of dry contact pin #2 on Board #0 to rising\n"};

static void doEdgeWrite(int argc, char *argv[])
{
	u8 pin = 0;
	u8 rising = 0;
	u8 falling = 0;
	int dev = 0;
	
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 5)
	{
		pin = (u8)atoi(argv[3]);
		
		if((1 & atoi(argv[4])) != 0)
		{
			rising = 1;
		}
		if((2 & atoi(argv[4])) != 0)
				{
					falling = 1;
				}
		if (OK != contactCountRisingSet(dev, pin, rising))
		{
			printf("Fail to write!\n");
			exit(1);
		}
		if (OK != contactCountFallingSet(dev, pin, falling))
		{
			printf("Fail to write!\n");
			exit(1);
		}
		
	}
	else
	{
		printf("%s", CMD_EDGE_WRITE.usage1);
		exit(1);
	}
}

int dacGet(int dev, int ch, float* val)
{
	u8 buff[2] =
	{
		0,
		0};
	u16 raw = 0;

	if ( (ch < CHANNEL_NR_MIN) || (ch > DAC_CH_NR_MAX))
	{
		printf("DAC channel out of range!\n");
		return ERROR;
	}
	if (OK != i2cMem8Read(dev, I2C_U0_10_OUT_VAL1_ADD + 2 * (ch - 1), buff, 2))
	{
		printf("Fail to read!\n");
		return ERROR;
	}
	memcpy(&raw, buff, 2);
	*val = (float)raw / 1000;
	return OK;
}

int dacSet(int dev, int ch, float val)
{
	u8 buff[2] =
	{
		0,
		0};
	u16 raw = 0;

	if ( (ch < CHANNEL_NR_MIN) || (ch > DAC_CH_NR_MAX))
	{
		printf("DAC channel out of range!\n");
		return ERROR;
	}
	if (val < 0)
	{
		val = 0;
	}
	if (val > 10)
	{
		val = 10;
	}
	raw = (u16)ceil(val * 1000); //transform to milivolts
	memcpy(buff, &raw, 2);
	if (OK != i2cMem8Write(dev, I2C_U0_10_OUT_VAL1_ADD + 2 * (ch - 1), buff, 2))
	{
		printf("Fail to write!\n");
		return ERROR;
	}
	return OK;
}

static void doDacRead(int argc, char *argv[]);
const CliCmdType CMD_DAC_READ =
	{
		"dacrd",
		2,
		&doDacRead,
		"\tdacrd:		Read DAC voltage value (0 - 10V)\n",
		"\tUsage:		megabas <id> dacrd <channel>\n",
		"",
		"\tExample:		megabas 0 dacrd 2; Read the voltage on 0-10V out channel #2 on Board #0\n"};

static void doDacRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > DAC_CH_NR_MAX))
		{
			printf("DAC channel out of range!\n");
			exit(1);
		}

		if (OK != dacGet(dev, ch, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}

		printf("%0.3f\n", val);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_DAC_READ.usage1);
		exit(1);
	}
}

static void doDacWrite(int argc, char *argv[]);
const CliCmdType CMD_DAC_WRITE =
	{
		"dacwr",
		2,
		&doDacWrite,
		"\tdacwr:		Write DAC output voltage value (0..10V)\n",
		"\tUsage:		megabas <id> dacwr <channel> <value>\n",
		"",
		"\tExample:		megabas 0 dacwr 2 2.5; Write 2.5V to 0-10V out channel #2 on Board #0\n"};

static void doDacWrite(int argc, char *argv[])
{
	int ch = 0;
	int dev = 0;
	float volt = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > DAC_CH_NR_MAX))
		{
			printf("DAC channel out of range!\n");
			exit(1);
		}
		volt = atof(argv[4]);
		if (volt < 0 || volt > 10)
		{
			printf("Invalid DAC voltage value, must be 0..10 \n");
			exit(1);
		}

		if (OK != dacSet(dev, ch, volt))
		{
			printf("Fail to write!\n");
			exit(1);
		}
		printf("done\n");
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_DAC_WRITE.usage1);
		exit(1);
	}
}

int adcGet(int dev, int ch, float* val)
{
	u8 buff[2] =
	{
		0,
		0};
	u16 raw = 0;

	if ( (ch < CHANNEL_NR_MIN) || (ch > ADC_CH_NR_MAX))
	{
		printf("ADC channel out of range!\n");
		return ERROR;
	}
	if (OK != i2cMem8Read(dev, I2C_U0_10_IN_VAL1_ADD + 2 * (ch - 1), buff, 2))
	{
		printf("Fail to read!\n");
		return ERROR;
	}
	memcpy(&raw, buff, 2);
	*val = (float)raw / 1000;
	return OK;
}

static void doAdcRead(int argc, char *argv[]);
const CliCmdType CMD_ADC_READ =
	{
		"adcrd",
		2,
		&doAdcRead,
		"\tadcrd:		Read ADC input voltage value (0 - 10V)\n\t\t\tWarning: For this measurements to be valid place the jumper in position \"0-10V\" \n",
		"\tUsage:		megabas <id> adcrd <channel>\n",
		"",
		"\tExample:		megabas 0 adcrd 2; Read the voltage input on 0-10V in channel #2 on Board #0\n"};

static void doAdcRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > ADC_CH_NR_MAX))
		{
			printf("ADC channel out of range!\n");
			exit(1);
		}

		if (OK != adcGet(dev, ch, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}

		printf("%0.3f\n", val);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_ADC_READ.usage1);
		exit(1);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
int r1kGet(int dev, int ch, float* val)
{
	u8 buff[2] =
	{
		0,
		0};
	u16 raw = 0;

	if ( (ch < CHANNEL_NR_MIN) || (ch > ADC_CH_NR_MAX))
	{
		printf("ADC channel out of range!\n");
		return ERROR;
	}
	if (OK != i2cMem8Read(dev, I2C_R_1K_CH1 + 2 * (ch - 1), buff, 2))
	{
		printf("Fail to read!\n");
		return ERROR;
	}
	memcpy(&raw, buff, 2);
	*val = (float)raw / 1000;
	return OK;
}

static void doR1kRead(int argc, char *argv[]);
const CliCmdType CMD_R1K_READ =
	{
		"r1krd",
		2,
		&doR1kRead,
		"\tr1krd:		Read resistor input  value (1k) in kiloOhms, Warning: You must palce the jumper in position \"1K\".\n\t\t\tReturn 30kOhm for resistor out of range\n",
		"\tUsage:		megabas <id> r1krd <channel>\n",
		"",
		"\tExample:	megabas 0 r1krd 2; Read the resistance input on channel #2 on Board #0\n"};

static void doR1kRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > ADC_CH_NR_MAX))
		{
			printf("Resistor read channel out of range!\n");
			exit(1);
		}

		if (OK != r1kGet(dev, ch, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}

		printf("%0.3f\n", val);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_ADC_READ.usage1);
		exit(1);
	}
}

int r10kGet(int dev, int ch, float* val)
{
	u8 buff[2] =
	{
		0,
		0};
	u16 raw = 0;

	if ( (ch < CHANNEL_NR_MIN) || (ch > ADC_CH_NR_MAX))
	{
		printf("ADC channel out of range!\n");
		return ERROR;
	}
	if (OK != i2cMem8Read(dev, I2C_R_10K_CH1 + 2 * (ch - 1), buff, 2))
	{
		printf("Fail to read!\n");
		return ERROR;
	}
	memcpy(&raw, buff, 2);
	*val = (float)raw / 1000;
	return OK;
}

static void doR10kRead(int argc, char *argv[]);
const CliCmdType CMD_R10K_READ =
	{
		"r10krd",
		2,
		&doR10kRead,
		"\tr10krd:		Read resistor input  value (10k) in kiloOhms, Warning: You must palce the jumper in position \"10K\".\n\t\t\tReturn 30kOhm for resistor out of range\n",
		"\tUsage:		megabas <id> r10krd <channel>\n",
		"",
		"\tExample:	megabas 0 r10krd 2; Read the resistance input on channel #2 on Board #0\n"};

static void doR10kRead(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > ADC_CH_NR_MAX))
		{
			printf("Resistor read channel out of range!\n");
			exit(1);
		}

		if (OK != r10kGet(dev, ch, &val))
		{
			printf("Fail to read!\n");
			exit(1);
		}

		printf("%0.3f\n", val);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_ADC_READ.usage1);
		exit(1);
	}
}

void getCalStat(int dev)
{
	u8 buff[2];

	busyWait(100);
	if (OK != i2cMem8Read(dev, I2C_MEM_CALIB_STATUS, buff, 1))
	{
		printf("Fail to read calibration status!\n");
		exit(1);
	}
	switch (buff[0])
	{
	case 0:
		printf("Calibration in progress\n");
		break;
	case 1:
		printf("Calibration done\n");
		break;
	case 2:
		printf("Calibration error!\n");
		break;
	default:
		printf("Unknown calibration status\n");
		break;
	}
}

static void doAdcCal(int argc, char *argv[]);
const CliCmdType CMD_ADC_CAL =
	{
		"adccal",
		2,
		&doAdcCal,
		"\tadccal:		Calibrate one ADC channel, the calibration must be done in 2 points at min 5V apart\n",
		"\tUsage:		megabas <id> adccal <channel> <value>\n",
		"",
		"\tExample:		megabas 0 adccal 2 0.5; Calibrate the voltage input on ADC channel #2 on Board #0 at 0.5V\n"};

static void doAdcCal(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;
	u8 buff[4] =
	{
		0,
		0};
	u16 raw = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > ADC_CH_NR_MAX))
		{
			printf("ADC channel out of range!\n");
			exit(1);
		}

		val = atof(argv[4]);
		if ( (val < 0) || (val > 10))
		{
			printf("ADC calibration value out of range!\n");
			exit(1);
		}
		raw = (u16)ceil(val * VOLT_TO_MILIVOLT);
		memcpy(buff, &raw, 2);
		buff[2] = ch + CAL_0_10V_IN_START_ID - 1;
		buff[3] = CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			exit(1);
		}

		getCalStat(dev);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_ADC_CAL.usage1);
		exit(1);
	}
}

static void doAdcCalRst(int argc, char *argv[]);
const CliCmdType CMD_ADC_CAL_RST =
	{
		"adccalrst",
		2,
		&doAdcCalRst,
		"\tadccalrst:	Reset the calibration for one ADC channel\n",
		"\tUsage:		megabas <id> adccalrst <channel>\n",
		"",
		"\tExample:		megabas 0 adccalrst 2 ; Reset the calibration on ADC channel #2 on Board #0 at factory default\n"};

static void doAdcCalRst(int argc, char *argv[])
{
	int ch = 0;

	int dev = 0;
	u8 buff[4] =
	{
		0,
		0,
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > ADC_CH_NR_MAX))
		{
			printf("ADC channel out of range!\n");
			exit(1);
		}

		buff[2] = ch + CAL_0_10V_IN_START_ID - 1;
		;
		buff[3] = RESET_CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			exit(1);
		}

		getCalStat(dev);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_ADC_CAL_RST.usage1);
		exit(1);
	}
}

static void doDacCal(int argc, char *argv[]);
const CliCmdType CMD_DAC_CAL =
	{
		"daccal",
		2,
		&doDacCal,
		"\tdaccal:		Calibrate one DAC channel, the calibration must be done in 2 points at min 5V apart\n",
		"\tUsage:		megabas <id> daccal <channel> <value>\n",
		"",
		"\tExample:		megabas 0 daccal 2 0.5; Calibrate the voltage outputs on DAC channel #2 on Board #0 at 0.5V\n"};

static void doDacCal(int argc, char *argv[])
{
	int ch = 0;
	float val = 0;
	int dev = 0;
	u8 buff[4] =
	{
		0,
		0};
	u16 raw = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 5)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > DAC_CH_NR_MAX))
		{
			printf("DAC channel out of range!\n");
			exit(1);
		}

		val = atof(argv[4]);
		if ( (val < 0) || (val > 10))
		{
			printf("DAC calibration value out of range!\n");
			exit(1);
		}
		raw = (u16)ceil(val * VOLT_TO_MILIVOLT);
		memcpy(buff, &raw, 2);
		buff[2] = ch + CAL_0_10V_OUT_START_ID - 1;
		buff[3] = CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			exit(1);
		}

		getCalStat(dev);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_DAC_CAL.usage1);
		exit(1);
	}
}

static void doDacCalRst(int argc, char *argv[]);
const CliCmdType CMD_DAC_CAL_RST =
	{
		"daccalrst",
		2,
		&doDacCalRst,
		"\tdaccalrst:	Reset calibration for one DAC channel\n",
		"\tUsage:		megabas <id> daccalrst <channel>\n",
		"",
		"\tExample:		megabas 0 daccalrst 2; Reset calibration data on DAC channel #2 on Board #0 at factory default\n"};

static void doDacCalRst(int argc, char *argv[])
{
	int ch = 0;

	int dev = 0;
	u8 buff[4] =
	{
		0,
		0,
		0,
		0};
	u16 raw = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		ch = atoi(argv[3]);
		if ( (ch < CHANNEL_NR_MIN) || (ch > DAC_CH_NR_MAX))
		{
			printf("DAC channel out of range!\n");
			exit(1);
		}

		memcpy(buff, &raw, 2);
		buff[2] = ch + CAL_0_10V_OUT_START_ID - 1;
		buff[3] = RESET_CALIBRATION_KEY;

		if (OK != i2cMem8Write(dev, I2C_MEM_CALIB_VALUE, buff, 4))
		{
			printf("Fail to write calibration data!\n");
			exit(1);
		}
		getCalStat(dev);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_DAC_CAL_RST.usage1);
		exit(1);
	}
}

static void doWdtReload(int argc, char *argv[]);
const CliCmdType CMD_WDT_RELOAD =
	{
		"wdtr",
		2,
		&doWdtReload,
		"\twdtr:		Reload the watchdog timer and enable the watchdog if is disabled\n",
		"\tUsage:		megabas <id> wdtr\n",
		"",
		"\tExample:		megabas 0 wdtr; Reload the watchdog timer on Board #0 with the period \n"};

static void doWdtReload(int argc, char *argv[])
{
	int dev = 0;
	u8 buff[2] =
	{
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		buff[0] = WDT_RESET_SIGNATURE;
		if (OK != i2cMem8Write(dev, I2C_MEM_WDT_RESET_ADD, buff, 1))
		{
			printf("Fail to write watchdog reset key!\n");
			exit(1);
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_RELOAD.usage1);
		exit(1);
	}
}

static void doWdtSetPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_SET_PERIOD =
	{
		"wdtpwr",
		2,
		&doWdtSetPeriod,
		"\twdtpwr:		Set the watchdog period in seconds, \n\t\t\treload command must be issue in this interval to prevent Raspberry Pi power off\n",
		"\tUsage:		megabas <id> wdtpwr <val> \n",
		"",
		"\tExample:		megabas 0 wdtpwr 10; Set the watchdog timer period on Board #0 at 10 seconds \n"};

static void doWdtSetPeriod(int argc, char *argv[])
{
	int dev = 0;
	u16 period;
	u8 buff[2] =
	{
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		period = (u16)atoi(argv[3]);
		if (0 == period)
		{
			printf("Invalid period!\n");
			exit(1);
		}
		memcpy(buff, &period, 2);
		if (OK != i2cMem8Write(dev, I2C_MEM_WDT_INTERVAL_SET_ADD, buff, 2))
		{
			printf("Fail to write watchdog period!\n");
			exit(1);
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_SET_PERIOD.usage1);
		exit(1);
	}
}

static void doWdtGetPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_GET_PERIOD =
	{
		"wdtprd",
		2,
		&doWdtGetPeriod,
		"\twdtprd:		Get the watchdog period in seconds, \n\t\t\treload command must be issue in this interval to prevent Raspberry Pi power off\n",
		"\tUsage:		megabas <id> wdtprd \n",
		"",
		"\tExample:		megabas 0 wdtprd; Get the watchdog timer period on Board #0\n"};

static void doWdtGetPeriod(int argc, char *argv[])
{
	int dev = 0;
	u16 period;
	u8 buff[2] =
	{
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		if (OK != i2cMem8Read(dev, I2C_MEM_WDT_INTERVAL_GET_ADD, buff, 2))
		{
			printf("Fail to read watchdog period!\n");
			exit(1);
		}
		memcpy(&period, buff, 2);
		printf("%d\n", (int)period);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_GET_PERIOD.usage1);
		exit(1);
	}
}

static void doWdtSetInitPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_SET_INIT_PERIOD =
	{
		"wdtipwr",
		2,
		&doWdtSetInitPeriod,
		"\twdtipwr:	Set the watchdog initial period in seconds, \n\t\t\tThis period is loaded after power cycle, giving Raspberry time to boot\n",
		"\tUsage:		megabas <id> wdtipwr <val> \n",
		"",
		"\tExample:		megabas 0 wdtipwr 10; Set the watchdog timer initial period on Board #0 at 10 seconds \n"};

static void doWdtSetInitPeriod(int argc, char *argv[])
{
	int dev = 0;
	u16 period;
	u8 buff[2] =
	{
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		period = (u16)atoi(argv[3]);
		if (0 == period)
		{
			printf("Invalid period!\n");
			exit(1);
		}
		memcpy(buff, &period, 2);
		if (OK != i2cMem8Write(dev, I2C_MEM_WDT_INIT_INTERVAL_SET_ADD, buff, 2))
		{
			printf("Fail to write watchdog period!\n");
			exit(1);
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_SET_INIT_PERIOD.usage1);
		exit(1);
	}
}

static void doWdtGetInitPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_GET_INIT_PERIOD =
	{
		"wdtiprd",
		2,
		&doWdtGetInitPeriod,
		"\twdtiprd:	Get the watchdog initial period in seconds. \n\t\t\tThis period is loaded after power cycle, giving Raspberry time to boot\n",
		"\tUsage:		megabas <id> wdtiprd \n",
		"",
		"\tExample:		megabas 0 wdtiprd; Get the watchdog timer initial period on Board #0\n"};

static void doWdtGetInitPeriod(int argc, char *argv[])
{
	int dev = 0;
	u16 period;
	u8 buff[2] =
	{
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		if (OK != i2cMem8Read(dev, I2C_MEM_WDT_INIT_INTERVAL_GET_ADD, buff, 2))
		{
			printf("Fail to read watchdog period!\n");
			exit(1);
		}
		memcpy(&period, buff, 2);
		printf("%d\n", (int)period);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_GET_INIT_PERIOD.usage1);
		exit(1);
	}
}

static void doWdtSetOffPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_SET_OFF_PERIOD =
	{
		"wdtopwr",
		2,
		&doWdtSetOffPeriod,
		"\twdtopwr:	Set the watchdog off period in seconds (max 48 days). \n\t\t\tThis is the time that watchdog mantain Raspberry turned off \n",
		"\tUsage:		megabas <id> wdtopwr <val> \n",
		"",
		"\tExample:		megabas 0 wdtopwr 10; Set the watchdog off interval on Board #0 at 10 seconds \n"};

static void doWdtSetOffPeriod(int argc, char *argv[])
{
	int dev = 0;
	u32 period;
	u8 buff[4] =
	{
		0,
		0,
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 4)
	{
		period = (u32)atoi(argv[3]);
		if ( (0 == period) || (period > WDT_MAX_OFF_INTERVAL_S))
		{
			printf("Invalid period!\n");
			exit(1);
		}
		memcpy(buff, &period, 4);
		if (OK
			!= i2cMem8Write(dev, I2C_MEM_WDT_POWER_OFF_INTERVAL_SET_ADD, buff, 4))
		{
			printf("Fail to write watchdog period!\n");
			exit(1);
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_SET_OFF_PERIOD.usage1);
		exit(1);
	}
}

static void doWdtGetOffPeriod(int argc, char *argv[]);
const CliCmdType CMD_WDT_GET_OFF_PERIOD =
	{
		"wdtoprd",
		2,
		&doWdtGetOffPeriod,
		"\twdtoprd:	Get the watchdog off period in seconds (max 48 days) \n\t\t\tThis is the time that watchdog mantain Raspberry turned off \n",
		"\tUsage:		megabas <id> wdtoprd \n",
		"",
		"\tExample:		megabas 0 wdtoprd; Get the watchdog off period on Board #0\n"};

static void doWdtGetOffPeriod(int argc, char *argv[])
{
	int dev = 0;
	u32 period;
	u8 buff[4] =
	{
		0,
		0,
		0,
		0};

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		if (OK
			!= i2cMem8Read(dev, I2C_MEM_WDT_POWER_OFF_INTERVAL_GET_ADD, buff, 4))
		{
			printf("Fail to read watchdog period!\n");
			exit(1);
		}
		memcpy(&period, buff, 4);
		printf("%d\n", (int)period);
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_WDT_GET_OFF_PERIOD.usage1);
		exit(1);
	}
}

int rs485Set(int dev, u8 mode, u32 baud, u8 stopB, u8 parity, u8 add)
{
	ModbusSetingsType settings;
	u8 buff[5];

	if (baud > 920600 || baud < 1200)
	{
		printf("Invalid RS485 Baudrate [1200, 920600]!\n");
		return ERROR;
	}
	if (mode > 1)
	{
		printf("Invalid RS485 mode : 0 = disable, 1= Modbus RTU (Slave)!\n");
		return ERROR;
	}
	if (stopB < 1 || stopB > 2)
	{
		printf("Invalid RS485 stop bits [1, 2]!\n");
		return ERROR;
	}
	if (parity > 2)
	{
		printf("Invalid RS485 parity 0 = none; 1 = even; 2 = odd! \n");
		return ERROR;
	}
	if (add < 1)
	{
		printf("Invalid MODBUS device address: [1, 255]!\n");
	}
	settings.mbBaud = baud;
	settings.mbType = mode;
	settings.mbParity = parity;
	settings.mbStopB = stopB;
	settings.add = add;

	memcpy(buff, &settings, sizeof(ModbusSetingsType));
	if (OK != i2cMem8Write(dev, I2C_MODBUS_SETINGS_ADD, buff, 5))
	{
		printf("Fail to write RS485 settings!\n");
		return ERROR;
	}
	return OK;
}

int rs485Get(int dev)
{
	ModbusSetingsType settings;
	u8 buff[5];

	if (OK != i2cMem8Read(dev, I2C_MODBUS_SETINGS_ADD, buff, 5))
	{
		printf("Fail to read RS485 settings!\n");
		return ERROR;
	}
	memcpy(&settings, buff, sizeof(ModbusSetingsType));
	printf("<mode> <baudrate> <stopbits> <parity> <add> %d %d %d %d %d\n",
		(int)settings.mbType, (int)settings.mbBaud, (int)settings.mbStopB,
		(int)settings.mbParity, (int)settings.add);
	return OK;
}

static void doRs485Read(int argc, char *argv[]);
const CliCmdType CMD_RS485_READ =
{
	"rs485rd",
	2,
	&doRs485Read,
	"\trs485rd:	Read the RS485 communication settings\n",
	"\tUsage:		megabas <id> rs485rd\n",
	"",
	"\tExample:		megabas 0 rs485rd; Read the RS485 settings on Board #0\n"};

static void doRs485Read(int argc, char *argv[])
{
	int dev = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 3)
	{
		if (OK != rs485Get(dev))
		{
			exit(1);
		}
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_RS485_READ.usage1);
		exit(1);
	}
}

static void doRs485Write(int argc, char *argv[]);
const CliCmdType CMD_RS485_WRITE =
	{
		"rs485wr",
		2,
		&doRs485Write,
		"\trs485wr:	Write the RS485 communication settings\n",
		"\tUsage:		megabas <id> rs485wr <mode> <baudrate> <stopBits> <parity> <slaveAddr>\n",
		"",
		"\tExample:		megabas 0 rs485wr 1 9600 1 0 1; Write the RS485 settings on Board #0 \n\t\t\t(mode = Modbus RTU; baudrate = 9600 bps; stop bits one; parity none; modbus slave address = 1)\n"};

static void doRs485Write(int argc, char *argv[])
{
	int dev = 0;
	u8 mode = 0;
	u32 baud = 1200;
	u8 stopB = 1;
	u8 parity = 0;
	u8 add = 0;

	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (argc == 8)
	{
		mode = 0xff & atoi(argv[3]);
		baud = atoi(argv[4]);
		stopB = 0xff & atoi(argv[5]);
		parity = 0xff & atoi(argv[6]);
		add = 0xff & atoi(argv[7]);
		if (OK != rs485Set(dev, mode, baud, stopB, parity, add))
		{
			exit(1);
		}
		printf("done\n");
	}
	else
	{
		printf("Invalid params number:\n %s", CMD_RS485_WRITE.usage1);
		exit(1);
	}
}

const CliCmdType* gCmdArray[] =
{
	&CMD_VERSION,
	&CMD_HELP,
	&CMD_WAR,
	&CMD_LIST,
	&CMD_BOARD,
	&CMD_TRIAC_WRITE,
	&CMD_TRIAC_READ,
	&CMD_TEST,
	&CMD_CONTACT_READ,
	&CMD_COUNTER_READ,
	&CMD_COUNTER_RST,
	&CMD_EDGE_READ,
	&CMD_EDGE_WRITE,
	&CMD_DAC_READ,
	&CMD_DAC_WRITE,
	&CMD_ADC_READ,
	&CMD_R1K_READ,
	&CMD_R10K_READ,
	&CMD_ADC_CAL,
	&CMD_ADC_CAL_RST,
	&CMD_DAC_CAL,
	&CMD_DAC_CAL_RST,
	&CMD_WDT_RELOAD,
	&CMD_WDT_SET_PERIOD,
	&CMD_WDT_GET_PERIOD,
	&CMD_WDT_SET_INIT_PERIOD,
	&CMD_WDT_GET_INIT_PERIOD,
	&CMD_WDT_SET_OFF_PERIOD,
	&CMD_WDT_GET_OFF_PERIOD,
	&CMD_RS485_READ,
	&CMD_RS485_WRITE,
	NULL}; //null terminated array of cli structure pointers

int main(int argc, char *argv[])
{
	int i = 0;

	if (argc == 1)
	{
		usage();
		return -1;
	}
	while (NULL != gCmdArray[i])
	{
		if ( (gCmdArray[i]->name != NULL) && (gCmdArray[i]->namePos < argc))
		{
			if (strcasecmp(argv[gCmdArray[i]->namePos], gCmdArray[i]->name) == 0)
			{
				gCmdArray[i]->pFunc(argc, argv);
				return 0;
			}
		}
		i++;
	}
	printf("Invalid command option\n");
	usage();

	return -1;
}
