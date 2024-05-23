#ifndef CAS_H
#define CAS_H

int CAS_HardwareSetup(void) __attribute__((section(".bootstrap.text")));
void CAS_ExitAndCleanup(void);

#endif