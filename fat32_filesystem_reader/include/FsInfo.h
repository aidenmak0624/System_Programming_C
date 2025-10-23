#ifndef FSINFO_H
#define FSINFO_H

#include <inttypes.h>
#pragma pack(push)
#pragma pack(1)
struct FSInfo{
    uint32_t lead_sig;
    uint8_t reserved1[480];
    uint32_t signature;
    uint32_t free_count;
    uint32_t next_free;
    uint8_t reserved2[12];
    uint32_t trail_signature;
};
#pragma pack(pop)

typedef struct FSInfo FSI;

#endif