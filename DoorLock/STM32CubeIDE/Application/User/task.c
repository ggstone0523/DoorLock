/*
 * task.c
 *
 *  Created on: Feb 20, 2026
 *      Author: yusunchoi
 */
#include "stm32f4xx_hal.h"
#include "task.h"

EventGroupHandle_t TaskModeEventGroup;
EventGroupHandle_t MotorEventGroup;
EventGroupHandle_t SoundEventGroup;
EventGroupHandle_t LockEventGroup;

QueueHandle_t NumberKeyMessageQueue;
QueueHandle_t MotorStatusMessageQueue;


extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;


void WriteUserData(UserData *data)
{
    HAL_FLASH_Unlock();

    FLASH_Erase_Sector(FLASH_SECTOR_11, VOLTAGE_RANGE_3);

    uint32_t *ptr = (uint32_t*)data;
    for(int i = 0; i < sizeof(UserData)/4; i++)
    {
        HAL_FLASH_Program(TYPEPROGRAM_WORD,
                          USER_FLASH_ADDR + (i*4),
                          ptr[i]);
    }

    HAL_FLASH_Lock();
}


void SoundBuzzer()
{
	const TickType_t vBuzzerTime = pdMS_TO_TICKS(100);

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	vTaskDelay(vBuzzerTime);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
	vTaskDelay(vBuzzerTime);
}

int DebounceEvent(uint32_t* baseTick)
{
	uint32_t currTick = *baseTick;
	uint32_t tickDiff = 0;

	currTick = HAL_GetTick();
	tickDiff = (currTick > *baseTick ? currTick - *baseTick : *baseTick - currTick);
	*baseTick = currTick;
	if(tickDiff <= 500)
		return 1;
	else
		return 0;
}

void NumberKeyTask(void *argument)
{
  uint8_t ucRxData;
  EventBits_t uxBits;
  size_t xReceivedBytes;
  const TickType_t xBlockTime = pdMS_TO_TICKS(200);
  uint8_t passwordLen = 4;
  uint8_t tempPassword[8] = {'\0'};
  uint8_t index = 0;
  uint8_t mode = 0; // 0: default mode, 1: password change mode
  uint8_t isLock = 0; // 0: non lock, 1: lock
  UserData data = {0x12345678, "1234"};
  volatile UserData *flashData = (UserData*)USER_FLASH_ADDR;

  if(flashData->magic != 0x12345678)
  {
	  WriteUserData(&data);
  }

  for(;;)
  {
	  uxBits = xEventGroupWaitBits(
				  TaskModeEventGroup,
				  TASKMODE_PASSWORDRESETSTART,
				  pdTRUE,
				  pdFALSE,
				  xBlockTime);
	  if((uxBits & TASKMODE_PASSWORDRESETSTART) != 0)
	  {
		  index = 0;
	  	  mode = 1;
	  }

	  uxBits = xEventGroupWaitBits(
			  	  LockEventGroup,
				  LOCK_UP | LOCK_DOWN,
				  pdTRUE,
				  pdFALSE,
				  xBlockTime);
	  if((uxBits & LOCK_DOWN) != 0)
		  isLock = 0;
	  else if((uxBits & LOCK_UP) != 0)
		  isLock = 1;

	  xReceivedBytes = xQueueReceive(NumberKeyMessageQueue, (void*)&ucRxData, xBlockTime);

	  if(xReceivedBytes <= 0)
		  continue;

	  if(isLock == 1)
	  {
		  xEventGroupSetBits(SoundEventGroup, SOUND_FAIL);
		  continue;
	  }

	  if(mode == 0)
	  {
		  if('*' == ucRxData && index == passwordLen)
		  {
		      xEventGroupSetBits(MotorEventGroup, MOTOR_CONTROL);
			  xEventGroupSetBits(SoundEventGroup, SOUND_SUCCESS);
			  index = 0;
		  }
		  else if('*' == ucRxData || index == passwordLen)
		  {
			  xEventGroupSetBits(SoundEventGroup, SOUND_FAIL);
			  index = 0;
		  }
		  else if(flashData->password[index] == ucRxData)
		  {
			  xEventGroupSetBits(SoundEventGroup, SOUND_CLICK_NUMBER_KEY);
			  ++index;
		  }
		  else
		  {
			  xEventGroupSetBits(SoundEventGroup, SOUND_CLICK_NUMBER_KEY);
			  index = 0;
		  }
	  }
	  else if(mode == 1)
	  {
		  if('#' == ucRxData && (index >= 4 && index <= 8))
		  {
			  passwordLen = index;
			  for(int i = 0; i < passwordLen; ++i)
				  data.password[i] = tempPassword[i];
			  for(int i = passwordLen; i < 8; ++i)
				  data.password[i] = '\0';
			  WriteUserData(&data);
			  index = 0;
			  mode = 0;
			  xEventGroupSetBits(SoundEventGroup, SOUND_SUCCESS);
		  }
		  else if(('#' == ucRxData && index < 4) || index > 8 || '*' == ucRxData)
		  {
			  index = 0;
			  mode = 0;
			  xEventGroupSetBits(SoundEventGroup, SOUND_FAIL);
		  }
		  else
		  {
			  tempPassword[index] = ucRxData;
			  ++index;
			  xEventGroupSetBits(SoundEventGroup, SOUND_CLICK_NUMBER_KEY);
		  }
	  }
  }
}

void SoundTask(void *argument)
{
  EventBits_t uxBits;
  const TickType_t xBlockTime = pdMS_TO_TICKS(200);

  for(;;)
  {
	  uxBits = xEventGroupWaitBits(
			  SoundEventGroup,
			  SOUND_CLICK_NUMBER_KEY | SOUND_SUCCESS | SOUND_FAIL | SOUND_ERROR | SOUND_ONE,
			  pdTRUE,
			  pdFALSE,
			  xBlockTime);
	  if((uxBits & (SOUND_CLICK_NUMBER_KEY | SOUND_ONE)) != 0)
	  {
		  SoundBuzzer();
	  }
	  if((uxBits & SOUND_SUCCESS) != 0)
	  {
		  SoundBuzzer();
		  SoundBuzzer();
	  }
	  if((uxBits & SOUND_FAIL) != 0)
	  {
		  SoundBuzzer();
		  SoundBuzzer();
		  SoundBuzzer();
	  }
	  if((uxBits & SOUND_ERROR) != 0)
	  {
		  SoundBuzzer();
		  SoundBuzzer();
		  SoundBuzzer();
		  SoundBuzzer();
	  }
  }
}

void MotorTask(void *argument)
{
  EventBits_t uxBits;
  uint8_t isMotorStatusLock = 0; // 0: open, 1: close
  const TickType_t xBlockTime = pdMS_TO_TICKS(200);
  const TickType_t xMotorActivateTime = pdMS_TO_TICKS(20);
  uint8_t numOfMotorStatusGetEvent = 0;

  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); // 2000 열린 상태
  for(;;)
  {	  uxBits = xEventGroupWaitBits(
		  MotorEventGroup,
		  MOTOR_CONTROL | MOTOR_OPEN_STATUS | MOTOR_CLOSE_STATUS | MOTOR_STATUS_GET_FOR_PASSWORDRESET | MOTOR_STATUS_GET_FOR_LOOKDOOR,
		  pdTRUE,
		  pdFALSE,
		  xBlockTime);
  	  if((uxBits & MOTOR_CONTROL) != 0)
  	  {
  		  if(isMotorStatusLock == 0)
  			  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 1000); // 닫힌 상태
  		  else
  			  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 2000); // 열린 상태
  		  vTaskDelay(xMotorActivateTime);
  		  isMotorStatusLock ^= 1;
  	  }
  	  if((uxBits & MOTOR_OPEN_STATUS) != 0)
  	  {
  		  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 2000); // 열린 상태
  		  vTaskDelay(xMotorActivateTime);
  		  isMotorStatusLock = 0;
  	  }
  	  if((uxBits & MOTOR_CLOSE_STATUS) != 0)
  	  {
  		  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 1000); // 닫힌 상태
  		  vTaskDelay(xMotorActivateTime);
  		  isMotorStatusLock = 1;
  	  }
  	  if((uxBits & MOTOR_STATUS_GET_FOR_PASSWORDRESET) != 0)
  		numOfMotorStatusGetEvent++;
  	  if((uxBits & MOTOR_STATUS_GET_FOR_LOOKDOOR) != 0)
  		numOfMotorStatusGetEvent++;

  	  while(numOfMotorStatusGetEvent > 0)
  	  {
  		numOfMotorStatusGetEvent--;
  		xQueueSend(MotorStatusMessageQueue, (void*)&isMotorStatusLock, 0);
  	  }
  }
}

void CloseDoorTask(void *argument)
{
	EventBits_t uxBits;
	const TickType_t xBlockTime = pdMS_TO_TICKS(200);
	uint32_t baseTick = HAL_GetTick();

	for(;;)
	{
		uxBits = xEventGroupWaitBits(
				  TaskModeEventGroup,
				  TASKMODE_CLOSEDOOR,
				  pdTRUE,
				  pdFALSE,
				  xBlockTime);
	  	if((uxBits & TASKMODE_CLOSEDOOR) != 0)
	  	{
			if(DebounceEvent(&baseTick) == 1)
				continue;
	  		xEventGroupSetBits(MotorEventGroup, MOTOR_CLOSE_STATUS);
	  		xEventGroupSetBits(SoundEventGroup, SOUND_SUCCESS);
	  	}
	}
}

void PasswordResetTask(void *argument)
{
	EventBits_t uxBits;
	uint8_t ucRxData;
	const TickType_t xBlockTime = pdMS_TO_TICKS(200);
	const TickType_t xMessageTime = pdMS_TO_TICKS(200);
	uint32_t baseTick = HAL_GetTick();

	for(;;)
	{
		uxBits = xEventGroupWaitBits(
				  TaskModeEventGroup,
				  TASKMODE_PASSWORDRESET,
				  pdTRUE,
				  pdFALSE,
				  xBlockTime);
	  	if((uxBits & TASKMODE_PASSWORDRESET) != 0)
	  	{
			if(DebounceEvent(&baseTick) == 1)
				continue;
  			xEventGroupSetBits(MotorEventGroup, MOTOR_STATUS_GET_FOR_LOOKDOOR);
  			while(xQueueReceive(MotorStatusMessageQueue,(void*)&ucRxData, xMessageTime) == errQUEUE_EMPTY);
  			if(ucRxData == 0)
  			{
  				xEventGroupSetBits(TaskModeEventGroup, TASKMODE_PASSWORDRESETSTART);
  				xEventGroupSetBits(SoundEventGroup, SOUND_ONE);
  			}
  			else
  				xEventGroupSetBits(SoundEventGroup, SOUND_FAIL);
	  	}
	}
}

void OpenDoorTask(void *argument)
{
	EventBits_t uxBits;
	const TickType_t xBlockTime = pdMS_TO_TICKS(200);
	uint32_t baseTick = HAL_GetTick();

	for(;;)
	{
		uxBits = xEventGroupWaitBits(
				  TaskModeEventGroup,
				  TASKMODE_OPENDOOR,
				  pdTRUE,
				  pdFALSE,
				  xBlockTime);
	  	if((uxBits & TASKMODE_OPENDOOR) != 0)
	  	{
			if(DebounceEvent(&baseTick) == 1)
				continue;
	  		xEventGroupSetBits(MotorEventGroup, MOTOR_OPEN_STATUS);
	  		xEventGroupSetBits(SoundEventGroup, SOUND_SUCCESS);
	  		xEventGroupSetBits(TaskModeEventGroup, TASKMODE_LOCKDOOR_DOWN);
	  	}
	}
}

void LockDoorTask(void *argument)
{
	int isLockUp = 0; // 0: lock down, 1: lock up
	EventBits_t uxBits;
	uint8_t ucRxData;
	const TickType_t xBlockTime = pdMS_TO_TICKS(200);
	const TickType_t xMessageTime = pdMS_TO_TICKS(200);
	uint32_t baseTick = HAL_GetTick();

	for(;;)
	{
		uxBits = xEventGroupWaitBits(
				  TaskModeEventGroup,
				  TASKMODE_LOCKDOOR | TASKMODE_LOCKDOOR_DOWN,
				  pdTRUE,
				  pdFALSE,
				  xBlockTime);
	  	if((uxBits & TASKMODE_LOCKDOOR) != 0)
	  	{
			if(DebounceEvent(&baseTick) == 1)
				continue;
	  		if(isLockUp == 1)
	  		{
	  			xEventGroupSetBits(LockEventGroup, LOCK_DOWN);
	  			xEventGroupSetBits(SoundEventGroup, SOUND_SUCCESS);
	  			isLockUp = 0;
	  		}
	  		else
	  		{
	  			xEventGroupSetBits(MotorEventGroup, MOTOR_STATUS_GET_FOR_LOOKDOOR);
	  			while(xQueueReceive(MotorStatusMessageQueue,(void*)&ucRxData, xMessageTime) == errQUEUE_EMPTY);
	  			if(ucRxData == 1)
	  			{
	  				xEventGroupSetBits(LockEventGroup, LOCK_UP);
	  				xEventGroupSetBits(SoundEventGroup, SOUND_SUCCESS);
	  				isLockUp = 1;
	  			}
	  			else
	  				xEventGroupSetBits(SoundEventGroup, SOUND_FAIL);
	  		}
	  	}
	  	if((uxBits & TASKMODE_LOCKDOOR_DOWN) != 0)
	  	{
	  		xEventGroupSetBits(LockEventGroup, LOCK_DOWN);
	  		isLockUp = 0;
	  	}
	}
}

