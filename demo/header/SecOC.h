/*
 * SecOC.h
 *
 *  Created on: 2021��9��6��
 *      Author: zhao chenyang
 */

#ifndef SECOC_H_
#define SECOC_H_
#include "Std_Types.h"
#include "ComStack_Types.h"
#include "SecOC_Types.h"

extern uint8 apduTx[];
extern uint8 spduTx[];
extern uint8 apduRx[];
extern uint8 spduRx[];
void init();

Std_ReturnType SecOC_IfTransmit(PduIdType TxPduId, const PduInfoType *PduInfoPtr);
Std_ReturnType authenticate(SecOCintermediate_type*);
void SecOC_MainFunctionRx();
void SecOC_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);

void  SecOC_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
void SecOC_MainFunctionRx();

#endif /* SECOC_H_ */
