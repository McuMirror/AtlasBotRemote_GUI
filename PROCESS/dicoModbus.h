/**
 * @file    dicoModbus.h
 * @brief   Dictionnaire Modbus
 * @author  S.BARBRY
 * @date    DECEMBRE 2015
 */
#ifndef _DICOMODBUS_H_
#define _DICOMODBUS_H_

/***************************** Read Only Registers *****************************/
#define MODBUS_READ_ONLY_REG_START_ADDR     0x100

#define MODBUS_SOFT_VERSION                 0x100
#define MODBUS_LEFT_SENSOR                  0x101
#define MODBUS_CENTRAL_SENSOR               0x102
#define MODBUS_RIGHT_SENSOR                 0x103

#define MODBUS_LEFT_MOTOR_SPEED             0x104
#define MODBUS_RIGHT_MOTOR_SPEED            0x105

#define MODBUS_CPU_LOAD                     0x106

#define MODBUS_READ_ONLY_REG_END_ADDR       0x106
#define MODBUS_READ_ONLY_REG_SIZE           (MODBUS_READ_ONLY_REG_END_ADDR - MODBUS_READ_ONLY_REG_START_ADDR + 1)

/**************************** Write Only Registers *****************************/
#define MODBUS_WRITE_ONLY_REG_START_ADDR    0x200

#define MODBUS_MOTOR_LEFT_CSG               0x200
#define MODBUS_MOTOR_RIGHT_CSG              0x201

#define MODBUS_WRITE_ONLY_REG_END_ADDR      0x201
#define MODBUS_WRITE_ONLY_REG_SIZE          (MODBUS_WRITE_ONLY_REG_END_ADDR - MODBUS_WRITE_ONLY_REG_START_ADDR + 1)

/***************************** Read/Write Registers ****************************/
#define MODBUS_READWRITE_REG_START_ADDR     0x300

#define MODBUS_MOTORS_MANU_ACCESS           0x300
#define MODBUS_MOTORS_LEFT_DUTY             0x301
#define MODBUS_MOTORS_RIGHT_DUTY            0x302

#define MODBUS_READWRITE_REG_END_ADDR       0x302
#define MODBUS_READWRITE_REG_SIZE           (MODBUS_READWRITE_REG_END_ADDR - MODBUS_READWRITE_REG_START_ADDR + 1)

/*************************** Configuration Registers ***************************/
#define MODBUS_CONFIGURATION_REG_START_ADDR 0x400

#define MODBUS_PID_LEFT_P                   0x400
#define MODBUS_PID_LEFT_I                   0x401
#define MODBUS_PID_RIGHT_P                  0x402
#define MODBUS_PID_RIGHT_I                  0x403

#define MODBUS_CONFIGURATION_REG_END_ADDR   0x403
#define MODBUS_CONFIGURATION_REG_SIZE       (MODBUS_CONFIGURATION_REG_END_ADDR - MODBUS_CONFIGURATION_REG_START_ADDR + 1)

/*******************************************************************************/

#endif /* _DICOMODBUS_H_ */
