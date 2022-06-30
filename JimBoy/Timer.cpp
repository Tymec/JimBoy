#include "Timer.h"

Timer::Timer(MemoryController* memoryController) : memoryController(memoryController) {
}

Timer::~Timer() {
}

void Timer::Update(unsigned cycles) {
	updateDivider(cycles);
	checkTimerClock();

	if (isClockEnabled()) {
		timerCounter -= cycles;

		if (timerCounter <= 0) {
			setTimerFreq();

			uint8_t tima = memoryController->cpuRead(TIMA);

			if (tima >= 255) {
				memoryController->cpuWrite(TIMA, memoryController->cpuRead(TMA));
				memoryController->requestInterrupt(TIMER);
			} else {
				memoryController->cpuWrite(TIMA, tima + 1);
			}
		}
	}
}

void Timer::updateDivider(unsigned cycles) {
	divCounter += cycles;

	if (divCounter >= 255) {
		divCounter = 0;

		memoryController->directWrite(0xFF04, getRegister(DIV) + 1);
	}
}

void Timer::checkTimerClock() {
	uint8_t newClock = getTimerClock();

	if (newClock != timerClock) 
		setTimerFreq();
}

bool Timer::isClockEnabled() {
	uint8_t tac = memoryController->cpuRead(TAC);
	return testBit(tac, 2);
}

uint8_t Timer::getTimerClock() {
	uint8_t tac = memoryController->cpuRead(TAC);
	tac &= 0x3;
	return tac;
}

void Timer::setTimerFreq() {
	uint8_t tac = getTimerClock();
	timerClock = tac;
	timerCounter = TimerClocks[tac];
}

uint8_t Timer::getRegister(TimerRegisters reg) {
	return memoryController->cpuRead(reg);
}
