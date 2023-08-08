//
// Created by Dizarker on 08.10.2021.
//

#ifndef DYNAMIC_STAND_BASE_PROTOCOLFILTER_H
#define DYNAMIC_STAND_BASE_PROTOCOLFILTER_H

#ifdef __cplusplus
extern "C"
{
#endif

#define PROTOCOL_FILTER_BUFFER_SIZE                     512
#define PROTOCOL_FILTER_MAX_PROTOCOLS_COUNT             2

#define PROTOCOL_FILTER_ASCII_SYMBOL_MIN_VALUE          0x20
#define PROTOCOL_FILTER_ASCII_SYMBOL_MAX_VALUE          0x7a

typedef enum
{
    PROTOCOL_FILTER_BYTEORDER_BIG_ENDIAN,
    PROTOCOL_FILTER_BYTEORDER_LITTLE_ENDIAN
} protocolFilter_byteorder_t;

typedef enum
{
    PROTOCOL_FILTER_TYPE_BINARY,
    PROTOCOL_FILTER_TYPE_TEXT
} protocolFilter_protocol_type_t;

typedef enum
{
    PROTOCOL_FILTER_STATE_WAIT_FOR_START_CONDITION,
    PROTOCOL_FILTER_STATE_RECEIVING_LENGTH,
    PROTOCOL_FILTER_STATE_RECEIVING_MESSAGE_BODY,
    PROTOCOL_FILTER_STATE_RECEIVING_CRC,
    PROTOCOL_FILTER_STATE_WAITING_FOR_POSTFIX
} protocolFilter_state_t;

typedef struct
{
    protocolFilter_protocol_type_t type;
    const char *prefix;
    int prefixSize;
    const char *postfix;
    int postfixSize;
    int lengthOffset;
    int lengthExtra;
    int lengthSize;
    protocolFilter_byteorder_t byteorder;
    int crcSize;
    int crcStartOffset;
    int crcStopOffset;
    int (*protocolParser)(const char *data, size_t length);
} protocolFilter_proto_handler;

typedef struct
{
    char buffer[PROTOCOL_FILTER_BUFFER_SIZE];
    int protocolsNum;
    int protoLock;
    int lenRx;
    int lenToRx;
    int r7;
    protocolFilter_proto_handler protocol[PROTOCOL_FILTER_MAX_PROTOCOLS_COUNT];
    protocolFilter_state_t state;
} protocolFilter_t;

int protocolFilter_init (protocolFilter_t *protoF, int protocolsNum);
int protocolFilter_task (protocolFilter_t *protoF, char byte);

#ifdef __cplusplus
}
#endif

#endif //DYNAMIC_STAND_BASE_PROTOCOLFILTER_H
