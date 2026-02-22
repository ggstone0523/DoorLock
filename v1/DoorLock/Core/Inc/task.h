/*
 * task.h
 *
 *  Created on: Feb 20, 2026
 *      Author: yusunchoi
 */

#ifndef APPLICATION_USER_TASK_H_
#define APPLICATION_USER_TASK_H_

#include "FreeRTOS.h"
#include "event_groups.h"
#include "message_buffer.h"

#define TASKMODE_CLOSEDOOR (1 << 0)
#define TASKMODE_PASSWORDRESET (1 << 1)
#define TASKMODE_PASSWORDRESETSTART (1 << 2)
#define TASKMODE_OPENDOOR (1 << 3)
#define TASKMODE_LOCKDOOR (1 << 4)
#define TASKMODE_LOCKDOOR_DOWN (1 << 5)

#define MOTOR_CONTROL (1 << 0)
#define MOTOR_OPEN_STATUS (1 << 1)
#define MOTOR_CLOSE_STATUS (1 << 2)
#define MOTOR_GET_STATUS (1 << 3)

#define SOUND_CLICK_NUMBER_KEY (1 << 0)
#define SOUND_SUCCESS (1 << 1)
#define SOUND_FAIL (1 << 2)
#define SOUND_ERROR (1 << 3)
#define SOUND_ONE (1 << 4)

#define LOCK_UP (1 << 0)
#define LOCK_DOWN (1 << 1)

void SoundBuzzer();
int DebounceEvent(uint32_t* baseTick);

void NumberKeyTask(void *argument);
void SoundTask(void *argument);
void MotorTask(void *argument);
void CloseDoorTask(void *argument);
void PasswordResetTask(void *argument);
void OpenDoorTask(void *argument);
void LockDoorTask(void *argument);


#endif /* APPLICATION_USER_TASK_H_ */
