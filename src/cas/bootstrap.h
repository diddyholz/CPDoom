#ifndef CAS_H
#define CAS_H

int CAS_HardwareSetup(void) __attribute__((section(".bootstrap")));
void CAS_ExitAndCleanup(void);

#endif