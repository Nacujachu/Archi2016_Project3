#ifndef VAR_H_INCLUDED
#define VAR_H_INCLUDED
#include<stdio.h>
#define Mask_op 0b11111100000000000000000000000000
#define Mask_rs 0b00000011111000000000000000000000
#define Mask_rt 0b00000000000111110000000000000000
#define Mask_rd 0b00000000000000001111100000000000
#define Mask_sh 0b00000000000000000000011111000000
#define Mask_fn 0b00000000000000000000000000111111
#define Mask_Im 0b00000000000000001111111111111111
#define Mask_ad 0b00000011111111111111111111111111
#define Shift_op 26
#define Shift_rs 21
#define Shift_rt 16
#define Shitf_rd 11
#define Shift_sh 6
#define Shift_fn 0
//R type
#define add 0x20
#define addu 0x21
#define sub 0x22
#define and 0x24
#define or 0x25
#define xor 0x26
#define nor 0x27
#define nand 0x28
#define slt 0x2A
#define sll 0x00
#define srl 0x02
#define sra 0x03
#define jr 0x08
//I type
#define addi 0x08
#define addiu 0x09
#define lw 0x23
#define lh 0x21
#define lhu 0x25
#define lb 0x20
#define lbu 0x24
#define sw 0x2B
#define sh 0x29
#define sb 0x28
#define lui 0x0F
#define andi 0x0C
#define ori 0x0D
#define nori 0x0E
#define slti 0x0A
#define beq 0x04
#define bne 0x05
#define bgtz 0x07
//J type && special
#define j 0x02
#define jal 0x03
#define halt 0x3F
#define Sp 29

typedef struct VPN_PPN_MAP
{
    unsigned int Vpn;
    unsigned int Ppn;
    int Valid;
} VPN_PPN_MAP;
typedef struct TLB_entry
{
    unsigned int VPN , PPN;
    int Valid;
    int Last_cycle;
} TLB_entry;
typedef struct PTE_entry
{
    unsigned int PPN;
    int Valid;
} PTE_entry;
typedef struct Cache_entry
{
    unsigned int Tag ;
    int MRU , Valid;
} Cache_entry;
typedef struct Mem_entry
{
    int Last_cycle;
    int Valid;
} Mem_entry;

int I_disk_size , D_disk_size;
int I_mem_size , D_mem_size;
int I_page_size , D_page_size;
int I_cache_size , D_cache_size;
int I_block_size , D_block_size;
int I_set , D_set;
int I_PTE_number , D_PTE_number;
int I_TLB_number , D_TLB_number;
int I_MEM_number , D_MEM_number;
int I_CAH_row , D_CAH_row;
int I_CAH_col , D_CAH_col;

VPN_PPN_MAP I_V_P_Map[1024];
Mem_entry I_MEM_table[1024];
Mem_entry D_MEM_table[1024];
PTE_entry I_PTE_table[1024];
PTE_entry D_PTE_table[1024];
TLB_entry I_TLB_table[1024];
TLB_entry D_TLB_table[1024];
Cache_entry I_Cache[1024][1024];
Cache_entry D_Cache[1024][1024];

unsigned int Pc;
unsigned int PPNcur;
unsigned int Cycle;
unsigned char I_disk[1024] , D_disk[1024];
unsigned int Reg[32];
unsigned int ins[256];
int I_tlb_hit , D_tlb_hit , I_tlb_miss , D_tlb_miss;
int I_pte_hit , D_pte_hit , I_pte_miss , D_pte_miss;
int I_cah_hit , D_cah_hit , I_cah_miss , D_cah_miss;
FILE *Ibin , *Dbin , *Report , *Snapshot;

#endif // VAR_H_INCLUDED
