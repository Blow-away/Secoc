/*
 * PduR_CanIf.c
 *
 *  Created on: 2021��9��7��
 *      Author: zhao chenyang
 */
#include "PduR_CanIf.h"
#include "SecOC.h"

void PduR_CanIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result){
	SecOC_TxConfirmation(TxPduId, result);
}
void PduR_CanIfRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr){
	SecOC_RxIndication(RxPduId,PduInfoPtr);
}
