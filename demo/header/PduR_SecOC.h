/*
 * PduR_SecOC.h
 *
 *  Created on: 2021��9��7��
 *      Author: zhao chenyang
 */

#ifndef PDUR_SECOC_H_
#define PDUR_SECOC_H_
#include "Std_Types.h"
#include "ComStack_Types.h"

//PDU�Ĵ�������
Std_ReturnType PduR_SecOCTransmit(PduIdType TxPduId , const PduInfoType* PduInfoPtr);

//�²�ͨ�Žӿ�ģ��ȷ��PDU�Ĵ������PDUʧ�ܡ�
void PduR_SecOCIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result);

void PduR_SecOCIfRxIndication(PduIdType RxPduId,const PduInfoType* PduInfoPtr);

#endif /* PDUR_SECOC_H_ */
