#include <gui/screen1_screen/Screen1View.hpp>

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "message_buffer.h"

extern EventGroupHandle_t DoorLockEventGroup;
extern MessageBufferHandle_t NumberKeyMessageBuffer;
}

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::click_one()
{
	static const uint8_t character = '1';
	xMessageBufferSend(NumberKeyMessageBuffer,
			(void*)&character,
			sizeof(character), 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_two()
{
	static const uint8_t character = '2';
	xMessageBufferSend(NumberKeyMessageBuffer,
			(void*)&character,
			sizeof(character), 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_three()
{
	static const uint8_t character = '3';
	xMessageBufferSend(NumberKeyMessageBuffer,
			(void*)&character,
			sizeof(character), 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_four()
{
	static const uint8_t character = '4';
	xMessageBufferSend(NumberKeyMessageBuffer,
			(void*)&character,
			sizeof(character), 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_five()
{
	static const uint8_t character = '5';
	xMessageBufferSend(NumberKeyMessageBuffer,
			(void*)&character,
			sizeof(character), 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_six()
{
	static const uint8_t character = '6';
	xMessageBufferSend(NumberKeyMessageBuffer,
			(void*)&character,
			sizeof(character), 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_seven()
{
	static const uint8_t character = '7';
	xMessageBufferSend(NumberKeyMessageBuffer,
			(void*)&character,
			sizeof(character), 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_eight()
{
	static const uint8_t character = '8';
	xMessageBufferSend(NumberKeyMessageBuffer,
			(void*)&character,
			sizeof(character), 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_nine()
{
	static const uint8_t character = '9';
	xMessageBufferSend(NumberKeyMessageBuffer,
			(void*)&character,
			sizeof(character), 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_star()
{
	static const uint8_t character = '*';
	xMessageBufferSend(NumberKeyMessageBuffer,
			(void*)&character,
			sizeof(character), 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_zero()
{
	static const uint8_t character = '0';
	xMessageBufferSend(NumberKeyMessageBuffer,
			(void*)&character,
			sizeof(character), 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_sharp()
{
	static const uint8_t character = '#';
	xMessageBufferSend(NumberKeyMessageBuffer,
			(void*)&character,
			sizeof(character), 0);
//     // Override and implement this function in Screen1
}
