#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include "main.h"
#define sizeofBuff 128
 
typedef struct
{
    uint8_t *buffer;
    uint8_t sizeBuff;
    uint8_t head;
    uint8_t tail;

} ringBuffer_t;

void putChar(USART_TypeDef * uart , uint8_t c);

uint8_t getChar(USART_TypeDef * uart);

void uart_tx(USART_TypeDef * uart,uint8_t *data, uint8_t size);

void ringBufferInit(ringBuffer_t *ringbuff, uint8_t *tempbuff, uint32_t size);

uint8_t getByteToWriteToRingBuffer (ringBuffer_t *ringbuff);

uint8_t getByteFromRingBufferAvailableToRead (ringBuffer_t *ringbuff);

uint8_t readFromRingBuffer(ringBuffer_t *ringbuff);

void WriteToRingBuffer(ringBuffer_t *ringbuff, USART_TypeDef * uart);
void Get_string (ringBuffer_t *ringbuff, char *buffer);
extern ringBuffer_t ringBuff;
extern uint8_t temp [sizeofBuff];
int wait_until (ringBuffer_t *ringbuff, char *string, char*buffertostore);
/*************  variable for ringBuffer   *************/  

#endif //END OF RING BUFFER
