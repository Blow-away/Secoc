/*
 * Com.c
 *
 *  Created on: 2021年9月6日
 *      Author: zhao chenyang
 */


#include "Com.h"
#include "PduR_Com.h"
#include <stdio.h>

uint8 pdus[2][8] = {
		1,1,1,1,1,1,0,0,
		2,2,2,2,2,2,0,0
};

PduInfoType temp;


void send_message(PduIdType TxPduId)
{
    temp.SduDataPtr = pdus;
    temp.SduLength = 6; // 暂取6
    Std_ReturnType result = PduR_ComTransmit(TxPduId, &temp);
}

void Com_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
   //打印result
}

void Com_RxIndication(PduIdType RxPduId, const PduInfoType *PduInfoPtr)
{
    // 表示收到数据
	for(int i=0;i<PduInfoPtr->SduLength;++i){
		printf("%d ",PduInfoPtr->SduDataPtr[i]);
	}
}
