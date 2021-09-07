/*
 * Com.c
 *
 *  Created on: 2021��9��6��
 *      Author: zhao chenyang
 */


#include "Com.h"
#include "PduR_Com.h"

uint8 pdus[2][8] = {
		1,1,1,1,1,1,0,0,
		2,2,2,2,2,2,0,0
};

PduInfoType temp;


void send_message(PduIdType TxPduId)
{
    temp.SduDataPtr = pdus;
    temp.SduLength = 6; // ��ȡ6
    Std_ReturnType result = PduR_ComTransmit(TxPduId, &temp);
}

void Com_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
   //��ӡresult
}

void Com_RxIndication(PduIdType RxPduId, const PduInfoType *PduInfoPtr)
{
    // ��ʾ�յ�����
}