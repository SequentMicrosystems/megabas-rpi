import smbus2
import time
import math

BUS_NO = 1  # change this in case of using different SBC 7 for ROCK 4 SE
TRIACS_VAL_ADD = 0
TRIACS_SET_ADD = 1
TRIACS_CLR_ADD = 2
DRY_CONTACT_VAL_ADD = 3

U0_10_OUT_VAL1_ADD = 4
U0_10_OUT_VAL2_ADD = 6
U0_10_OUT_VAL3_ADD = 8
U0_10_OUT_VAL4_ADD = 10
U0_10_IN_VAL1_ADD = 12
U0_10_IN_VAL2_ADD = 14
U0_10_IN_VAL3_ADD = 16
U0_10_IN_VAL4_ADD = 18
U0_10_IN_VAL5_ADD = 20
U0_10_IN_VAL6_ADD = 22
U0_10_IN_VAL7_ADD = 24
U0_10_IN_VAL8_ADD = 26
R_1K_CH1 = 28
R_1K_CH2 = 30
R_1K_CH3 = 32
R_1K_CH4 = 34
R_1K_CH5 = 36
R_1K_CH6 = 38
R_1K_CH7 = 40
R_1K_CH8 = 42
R_10K_CH1 = 44
R_10K_CH2 = 46
R_10K_CH3 = 48
R_10K_CH4 = 50
R_10K_CH5 = 52
R_10K_CH6 = 54
R_10K_CH7 = 56
R_10K_CH8 = 58
CAL_VAL_ADD = 60
CAL_CH_ADD = 62  # 60
CAL_CMD_ADD = 63
CAL_STAT_ADD = 64
# TEMPERATURE_ADD = 64
MODBUS_SETINGS_ADD = 65  # = (0)
MODBUS_ID_OFFSET_ADD = 69

RTC_YEAR_ADD = 70
RTC_MONTH_ADD = 71
RTC_DAY_ADD = 72
RTC_HOUR_ADD = 73
RTC_MINUTE_ADD = 74
RTC_SECOND_ADD = 75

RTC_SET_YEAR_ADD = 76
RTC_SET_MONTH_ADD = 77
RTC_SET_DAY_ADD = 78
RTC_SET_HOUR_ADD = 79
RTC_SET_MINUTE_ADD = 80
RTC_SET_SECOND_ADD = 81
RTC_CMD_ADD = 82

I2C_MEM_WDT_RESET_ADD = 83
I2C_MEM_WDT_INTERVAL_SET_ADD = 84
I2C_MEM_WDT_INTERVAL_GET_ADD = I2C_MEM_WDT_INTERVAL_SET_ADD + 2
I2C_MEM_WDT_INIT_INTERVAL_SET_ADD = I2C_MEM_WDT_INTERVAL_GET_ADD + 2
I2C_MEM_WDT_INIT_INTERVAL_GET_ADD = I2C_MEM_WDT_INIT_INTERVAL_SET_ADD + 2
I2C_MEM_WDT_RESET_COUNT_ADD = I2C_MEM_WDT_INIT_INTERVAL_GET_ADD + 2
I2C_MEM_WDT_CLEAR_RESET_COUNT_ADD = I2C_MEM_WDT_RESET_COUNT_ADD + 2
I2C_MEM_WDT_POWER_OFF_INTERVAL_SET_ADD = I2C_MEM_WDT_CLEAR_RESET_COUNT_ADD + 1
I2C_MEM_WDT_POWER_OFF_INTERVAL_GET_ADD = I2C_MEM_WDT_POWER_OFF_INTERVAL_SET_ADD + 4
I2C_MEM_DRY_CONTACT_RISING_ENABLE = I2C_MEM_WDT_POWER_OFF_INTERVAL_GET_ADD + 4
I2C_MEM_DRY_CONTACT_FALLING_ENABLE = I2C_MEM_DRY_CONTACT_RISING_ENABLE + 1
I2C_MEM_DRY_CONTACT_CH_CONT_RESET = I2C_MEM_DRY_CONTACT_FALLING_ENABLE

DIAG_TEMPERATURE_MEM_ADD = 0x72  # = (0)
DIAG_24V_MEM_ADD = 0x73
DIAG_24V_MEM_ADD1 = 0x74
DIAG_5V_MEM_ADD = 0x75
DIAG_5V_MEM_ADD1 = 0x76
CAN_REC_MPS_MEM_ADD = 0x77
REVISION_HW_MAJOR_MEM_ADD = 0x78  # = (0)
REVISION_HW_MINOR_MEM_ADD = 0x79
REVISION_MAJOR_MEM_ADD = 0x7a
REVISION_MINOR_MEM_ADD = 0x7b
BUILD_DAY_MEM_ADD = 0x7c
BUILD_MOTH_MEM_ADD = 0x7d
BUILD_YEAR_MEM_ADD = 0x7e
BOARD_TYPE_MEM_ADD = 0x7f
I2C_MEM_DRY_CONTACT_COUNTERS = 0x80
RELOAD_KEY = 0xca
U_OUT_CH_MAX = 4
U_IN_CH_MAX = 8
CONTACT_CH_MAX = 8
R_IN_CH_MAX = 8
TRIAC_CH_MAX = 4
HW_ADD = 0x48
WDT_MAX_POWER_OFF_INTERVAL = 4147200  # 48 days


def c2(val):
    if val > 32768:
        val = val - 65536
    return val


def checkStack(stack):
    if stack < 0 or stack > 7:
        raise ValueError('Invalid stack level [0..7]')


def checkCh(ch, max):
    if ch < 1 or ch > max:
        raise ValueError('Channel out of range [1..' + str(max) + ']')


def checkOwbSns(ch):
    if ch < 1 or ch > 16:
        raise ValueError('One Wire Bus sensor number out of range [1..16]')


def getVer(stack):
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    fw_maj = bus.read_byte_data(HW_ADD + stack, REVISION_MAJOR_MEM_ADD)
    fw_min = bus.read_byte_data(HW_ADD + stack, REVISION_MINOR_MEM_ADD)
    ret = " Fw " + str(fw_maj) + "." + str(fw_min)
    # print(" Hardware "+str(hw_maj)+"."+str(hw_min) + " Firmware "+str(fw_maj)+"."+str(fw_min))
    bus.close()
    return ret


def setUOut(stack, ch, val):
    checkCh(ch, U_OUT_CH_MAX)
    checkStack(stack)
    if val < 0 or val > 10:
        raise ValueError('Invalid value')

    bus = smbus2.SMBus(BUS_NO)
    bus.write_word_data(HW_ADD + stack, U0_10_OUT_VAL1_ADD + (2 * (ch - 1)), int(val * 1000))
    bus.close()
    return 1


def getUOut(stack, ch):
    checkCh(ch, U_OUT_CH_MAX)
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    val = bus.read_word_data(HW_ADD + stack, U0_10_OUT_VAL1_ADD + (2 * (ch - 1)))
    bus.close()
    return c2(val) / 1000.0


def getUIn(stack, ch):
    checkCh(ch, U_IN_CH_MAX)
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    val = bus.read_word_data(HW_ADD + stack, U0_10_IN_VAL1_ADD + (2 * (ch - 1)))
    bus.close()
    return c2(val) / 1000.0


def getRIn1K(stack, ch):
    checkCh(ch, R_IN_CH_MAX)
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    val = bus.read_word_data(HW_ADD + stack, R_1K_CH1 + (2 * (ch - 1)))
    bus.close()
    return val / 1000.0


def getRIn10K(stack, ch):
    checkCh(ch, R_IN_CH_MAX)
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    val = bus.read_word_data(HW_ADD + stack, R_10K_CH1 + (2 * (ch - 1)))
    bus.close()
    return val / 1000.0


def getTriacs(stack):
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    val = bus.read_byte_data(HW_ADD + stack, TRIACS_VAL_ADD)
    bus.close()
    return val


def getTriac(stack, ch):
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    val = bus.read_byte_data(HW_ADD + stack, TRIACS_VAL_ADD)
    bus.close()
    mask = (1 << (ch - 1))
    return val & mask


def setTriacs(stack, val):
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    try:
        bus.write_byte_data(HW_ADD + stack, TRIACS_VAL_ADD, val)
    except:
        bus.close()
        return -1
    bus.close()
    return 1


def setTriac(stack, ch, val):
    checkCh(ch, TRIAC_CH_MAX)
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    if val != 0:
        bus.write_byte_data(HW_ADD + stack, TRIACS_SET_ADD, ch)
    else:
        bus.write_byte_data(HW_ADD + stack, TRIACS_CLR_ADD, ch)
    bus.close()
    return 1


def togleTriac(stack, ch, delay, count):
    checkCh(ch, TRIAC_CH_MAX)
    checkStack(stack)
    for i in range(count):
        setTriac(stack, ch, 1)
        time.sleep(delay)
        setTriac(stack, ch, 0)
        time.sleep(delay)


def getContact(stack):
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    val = bus.read_byte_data(HW_ADD + stack, DRY_CONTACT_VAL_ADD)
    bus.close()
    return val


def getContactCh(stack, ch):
    checkCh(ch, CONTACT_CH_MAX)
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    val = bus.read_byte_data(HW_ADD + stack, DRY_CONTACT_VAL_ADD)
    bus.close()
    mask = 1 << (ch - 1)
    if val & mask:
        return 1
    return 0


def getContactCounter(stack, ch):
    checkCh(ch, CONTACT_CH_MAX)
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    try:
        buff = bus.read_i2c_block_data(HW_ADD + stack, I2C_MEM_DRY_CONTACT_COUNTERS + (ch - 1) * 4, 4)
        val = buff[0] + (buff[1] << 8) + (buff[2] << 16) + (buff[3] << 24)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    bus.close()
    return val


def getContactCountEdge(stack, ch):
    checkCh(ch, CONTACT_CH_MAX)
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    try:
        rising = bus.read_byte_data(HW_ADD + stack, I2C_MEM_DRY_CONTACT_RISING_ENABLE)
        falling = bus.read_byte_data(HW_ADD + stack, I2C_MEM_DRY_CONTACT_FALLING_ENABLE)
    except Exception as e:
        bus.close()
        raise Error(e)
    bus.close()
    val = 0
    if (rising & (1 << (ch - 1))) != 0:
        val += 1
    if (falling & (1 << (ch - 1))) != 0:
        val += 2
    return val


def setContactCountEdge(stack, ch, edge):
    checkCh(ch, CONTACT_CH_MAX)
    checkStack(stack)
    if edge < 0 or edge > 3:
        raise ValueError('Invalid edge type, 0 - none(disable counting), 1 - rising, 2 - falling, 3 - both')
    bus = smbus2.SMBus(BUS_NO)
    try:
        rising = bus.read_byte_data(HW_ADD + stack, I2C_MEM_DRY_CONTACT_RISING_ENABLE)
        falling = bus.read_byte_data(HW_ADD + stack, I2C_MEM_DRY_CONTACT_FALLING_ENABLE)
    except Exception as e:
        bus.close()
        raise Error(e)
    if edge == 0:
        rising = rising & (~(1 << (ch - 1)))
        falling = falling & (~(1 << (ch - 1)))
    else:
        if (edge & 1) != 0:
            rising = rising | (1 << (ch - 1))
        if (edge & 2) != 0:
            falling = falling | (1 << (ch - 1))
    try:
        bus.write_byte_data(HW_ADD + stack, I2C_MEM_DRY_CONTACT_RISING_ENABLE, rising)
        bus.write_byte_data(HW_ADD + stack, I2C_MEM_DRY_CONTACT_FALLING_ENABLE, falling)
    except Exception as e:
        bus.close()
        raise Error(e)
    bus.close()


def getInVolt(stack):
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    val = bus.read_word_data(HW_ADD + stack, DIAG_24V_MEM_ADD)
    bus.close()
    return val / 1000.0


def getRaspVolt(stack):
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    val = bus.read_word_data(HW_ADD + stack, DIAG_5V_MEM_ADD)
    bus.close()
    return val / 1000.0


def getCpuTemp(stack):
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    val = bus.read_byte_data(HW_ADD + stack, DIAG_TEMPERATURE_MEM_ADD)
    bus.close()
    return val


# watchdog functions


def wdtGetPeriod(stack):
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    try:
        val = bus.read_word_data(HW_ADD + stack, I2C_MEM_WDT_INTERVAL_GET_ADD)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    bus.close()
    return val


def wdtSetPeriod(stack, val):
    ret = 1
    checkStack(stack)
    if val < 10 or val > 65000:
        raise ValueError('Invalid interval value [10..65000]')
    bus = smbus2.SMBus(BUS_NO)
    try:
        bus.write_word_data(HW_ADD + stack, I2C_MEM_WDT_INTERVAL_SET_ADD, val)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    bus.close()
    return ret


def wdtReload(stack):
    ret = 1
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    try:
        bus.write_byte_data(HW_ADD + stack, I2C_MEM_WDT_RESET_ADD, RELOAD_KEY)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    bus.close()
    return ret


def wdtSetDefaultPeriod(stack, val):
    ret = 1
    checkStack(stack)
    if val < 10 or val > 64999:
        raise ValueError('Invalid interval value [10..64999]')
    bus = smbus2.SMBus(BUS_NO)
    if 10 < val < 65000:
        try:
            bus.write_word_data(HW_ADD + stack, I2C_MEM_WDT_INIT_INTERVAL_SET_ADD, val)
        except:
            bus.close()
            raise ValueError(e)
    else:
        ret = -1
    return ret


def wdtGetDefaultPeriod(stack):
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    try:
        val = bus.read_word_data(HW_ADD + stack, I2C_MEM_WDT_INIT_INTERVAL_GET_ADD)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    return val


def wdtSetOffInterval(stack, val):
    ret = 1
    checkStack(stack)
    if 10 > val or val > WDT_MAX_POWER_OFF_INTERVAL:
        raise ValueError('Invalid interval value [2..4147200]')
    bus = smbus2.SMBus(BUS_NO)
    buff = [0, 0, 0, 0]
    buff[0] = 0xff & val
    buff[1] = 0xff & (val >> 8)
    buff[2] = 0xff & (val >> 16)
    buff[3] = 0xff & (val >> 24)
    try:
        bus.write_i2c_block_data(HW_ADD + stack, I2C_MEM_WDT_POWER_OFF_INTERVAL_SET_ADD, buff)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    bus.close()
    return ret


def wdtGetOffInterval(stack):
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    try:
        buff = bus.read_i2c_block_data(HW_ADD + stack, I2C_MEM_WDT_POWER_OFF_INTERVAL_GET_ADD, 4)
        val = buff[0] + (buff[1] << 8) + (buff[2] << 16) + (buff[3] << 24)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    return val


def wdtGetResetCount(stack):
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    try:
        val = bus.read_word_data(HW_ADD + stack, I2C_MEM_WDT_RESET_COUNT_ADD)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    return val


I2C_RTC_YEAR_ADD = 70
I2C_RTC_MONTH_ADD = 71
I2C_RTC_DAY_ADD = 72
I2C_RTC_HOUR_ADD = 73
I2C_RTC_MINUTE_ADD = 74
I2C_RTC_SECOND_ADD = 75
I2C_RTC_SET_YEAR_ADD = 76
I2C_RTC_SET_MONTH_ADD = 77
I2C_RTC_SET_DAY_ADD = 78
I2C_RTC_SET_HOUR_ADD = 79
I2C_RTC_SET_MINUTE_ADD = 80
I2C_RTC_SET_SECOND_ADD = 81
I2C_RTC_CMD_ADD = 82


def rtcGet(stack):
    global I2C_RTC_YEAR_ADD
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    try:
        buff = bus.read_i2c_block_data(HW_ADD + stack, I2C_RTC_YEAR_ADD, 6)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    bus.close()
    t = (2000 + buff[0], buff[1], buff[2], buff[3], buff[4], buff[5])
    return t


def rtcSet(stack, y, mo, d, h, m, s):
    if y > 2000:
        y -= 2000
    if y < 0 or y > 255:
        raise ValueError("Invalid year!")
    if mo > 12 or mo < 1:
        raise ValueError("Invalid month!")
    if d < 1 or d > 31:
        raise ValueError("Invalid day!")
    if h < 0 or h > 23:
        raise ValueError("Invalid hour!")
    if m < 0 or m > 59:
        raise ValueError("Invalid minute!")
    if s < 0 or s > 59:
        raise ValueError("Invalid seconds!")
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    buff = [int(y), int(mo), int(d), int(h), int(m), int(s), 0xaa]
    try:
        bus.write_i2c_block_data(HW_ADD + stack, I2C_RTC_SET_YEAR_ADD, buff)
    except Exception as e:
        bus.close()
        raise ValueError(e)
    bus.close()


I2C_MEM_1WB_DEV = 161
I2C_MEM_1WB_TEMP_ALL = 162
I2C_MEM_1WB_START_SEARCH = 173
I2C_MEM_1WB_T1 = 174
I2C_MEM_1WB_ROM_CODE_IDX = 206
I2C_MEM_1WB_ROM_CODE = 207
OWB_TEMP_SIZE_B = 2
OWM_ROM_CODE_SIZE_B = 8

def owbScan(stack):
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    buf = 0xaa
    try:
        bus.write_byte_data(HW_ADD + stack, I2C_MEM_1WB_START_SEARCH, buf)
    except Exception as e:
        bus.close()
        raise Exception("Fail to write with exception " + str(e))
    bus.close()


def owbGetSensorNo(stack):
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    try:
        no = bus.read_byte_data(HW_ADD + stack, I2C_MEM_1WB_DEV)
    except Exception as e:
        bus.close()
        raise Exception("Fail to read with exception " + str(e))
    bus.close()
    return no


def owbGetTemp(stack, sensor):
    checkOwbSns(sensor)
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    try:
        temp = bus.read_word_data(HW_ADD + stack, I2C_MEM_1WB_T1 + OWB_TEMP_SIZE_B * (sensor - 1))
    except Exception as e:
        bus.close()
        raise Exception("Fail to read with exception " + str(e))
    bus.close()
    return temp / 100.0


def owbGetRomCode(stack, sensor):
    checkOwbSns(sensor)
    checkStack(stack)
    bus = smbus2.SMBus(BUS_NO)
    try:
        bus.write_byte_data(HW_ADD + stack, I2C_MEM_1WB_ROM_CODE_IDX, sensor - 1)  # Select the sensor
        buff = bus.read_i2c_block_data(HW_ADD + stack, I2C_MEM_1WB_ROM_CODE, 8)
    except Exception as e:
        bus.close()
        raise Exception("Fail to read with exception " + str(e))
    bus.close()
    return buff
