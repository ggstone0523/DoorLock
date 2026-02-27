#include <gui/screen1_screen/Screen1View.hpp>

extern "C" {
#include "task.h"

extern EventGroupHandle_t DoorLockEventGroup;
extern QueueHandle_t NumberKeyMessageQueue;
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
	xQueueSend(NumberKeyMessageQueue, (void*)&character, 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_two()
{
	static const uint8_t character = '2';
	xQueueSend(NumberKeyMessageQueue, (void*)&character, 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_three()
{
	static const uint8_t character = '3';
	xQueueSend(NumberKeyMessageQueue, (void*)&character, 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_four()
{
	static const uint8_t character = '4';
	xQueueSend(NumberKeyMessageQueue, (void*)&character, 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_five()
{
	static const uint8_t character = '5';
	xQueueSend(NumberKeyMessageQueue, (void*)&character, 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_six()
{
	static const uint8_t character = '6';
	xQueueSend(NumberKeyMessageQueue, (void*)&character, 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_seven()
{
	static const uint8_t character = '7';
	xQueueSend(NumberKeyMessageQueue, (void*)&character, 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_eight()
{
	static const uint8_t character = '8';
	xQueueSend(NumberKeyMessageQueue, (void*)&character, 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_nine()
{
	static const uint8_t character = '9';
	xQueueSend(NumberKeyMessageQueue, (void*)&character, 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_star()
{
	static const uint8_t character = '*';
	xQueueSend(NumberKeyMessageQueue, (void*)&character, 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_zero()
{
	static const uint8_t character = '0';
	xQueueSend(NumberKeyMessageQueue, (void*)&character, 0);
//     // Override and implement this function in Screen1
}

void Screen1View::click_sharp()
{
	static const uint8_t character = '#';
	xQueueSend(NumberKeyMessageQueue, (void*)&character, 0);
//     // Override and implement this function in Screen1
}
