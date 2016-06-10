#include <stdio.h>
#include <stdlib.h>

#include "var.h"
#include "data_io.h"
#include "CMP.h"
#include "D_CMP.h"
void debug_print(int debug,int Regdebug,int OP , int FN,int Rs , int Rt , int Rd,int Load)
{
    if(debug == 0)return ;

    if(OP == 0&&Regdebug==1)
    {
        printf("Cycle: %d  ",Cycle);
        switch(FN)
        {
        case add:
            printf("add\n");
            break;
        case addu:
            printf("addu\n");
            break;
        case sub:
            printf("sub\n");
            break;
        case and:
            printf("and\n");
            break;
        case or:
            printf("or\n");
            break;
        case xor:
            printf("xor\n");
            break;
        case nor:
            printf("nor\n");
            break;
        case nand:
            printf("nand\n");
            break;
        case slt:
            printf("slt\n");
            break;
        case sll:
            printf("sll\n");
            break;
        case srl:
            printf("srl\n");
            break;
        case sra:
            printf("sra\n");
            break;
        case jr:
            printf("jr\n");
            break;
        default:
            break;
        }
        printf("%x %x %x\n",Reg[Rd],Reg[Rt],Reg[Rs]);
    }
    else if (OP!=0)
    {
        printf("Cycle: %d  ",Cycle);
        switch(OP)
        {

        case addi:
            printf("addi\n");
            break;
        case addiu:
            printf("addiu\n");
            break;
        case lw:
            printf("lw\n");
            printf("Load: %d\n",Load);
            break;
        case lh:
            printf("lh\n");
            printf("Load: %x\n",Load);
            break;
        case lhu:
            printf("lhu\n");
            printf("Load: %d\n",Load);
            break;
        case lb:
            printf("lb\n");
            printf("Load: %d\n",Load);
            break;
        case sw :
            printf("sw\n");
            printf("Load: %d\n",Load);
            break;
        case sh:
            printf("sh\n");
            printf("Load: %d\n",Load);
            break;
        case sb:
            printf("sb\n");
            printf("Load: %d\n",Load);
            break;
        case lui:
            printf("lui\n");
            break;
        case andi:
            printf("andi\n");
            break;
        case ori:
            printf("ori\n");
            break;
        case slti:
            printf("slti\n");
            break;
        case beq:
            printf("beq\n");
            break;
        case bne:
            printf("bne\n");
            break;
        case bgtz:
            printf("bgtz\n");
            break;
        case jal:
            printf("jal\n");
            break;
        case j:
            printf("j\n");
            break;
        case halt:
            printf("halt\n");
            break;
        default:
            break;
        }
    }

}
int main(int argc , char *argv[])
{
    if(argc == 1)
    {
        I_mem_size = 64;
        D_mem_size = 32;
        I_page_size = 8;
        D_page_size = 16;
        I_cache_size = 16;
        I_block_size = 4;
        I_set = 4;
        D_cache_size = 16;
        D_block_size = 4;
        D_set = 1;
    }
    else
    {
        I_mem_size = atoi(argv[0+1]);
        D_mem_size = atoi(argv[1+1]);
        I_page_size = atoi(argv[2+1]);
        D_page_size = atoi(argv[3+1]);
        I_cache_size = atoi(argv[4+1]);
        I_block_size = atoi(argv[5+1]);
        I_set = atoi(argv[6+1]);
        D_cache_size = atoi(argv[7+1]);
        D_block_size = atoi(argv[8+1]);
        D_set = atoi(argv[9+1]);
    }

    I_PTE_number = 1024 / I_page_size;
    D_PTE_number = 1024 / D_page_size;
    I_TLB_number = I_PTE_number/4;
    D_TLB_number = D_PTE_number/4;
    I_MEM_number = I_mem_size / I_page_size;
    D_MEM_number = D_mem_size / D_page_size;
    I_CAH_col = I_set;
    D_CAH_col = D_set;
    I_CAH_row = I_cache_size / I_set / I_block_size;
    D_CAH_row = D_cache_size / D_set / D_block_size;

    initial();
    memory_initial();
    Take_snap();
    while(Cycle <= 500000)
    {
        unsigned int VM = Pc;
        unsigned int NX_ins = ins[Pc/4];
        unsigned int OP = (NX_ins&Mask_op)>>Shift_op;
        unsigned int RD = (NX_ins&Mask_rd)>>Shitf_rd;
        unsigned int RT = (NX_ins&Mask_rt)>>Shift_rt;
        unsigned int RS = (NX_ins&Mask_rs)>>Shift_rs;
        unsigned int SH = (NX_ins&Mask_sh)>>Shift_sh;
        unsigned int IMD= (NX_ins&Mask_Im);
        unsigned int FN = (NX_ins&Mask_fn);
        unsigned int AD = (NX_ins&Mask_ad);
        int Simd = IMD;
        Simd = Simd<<16>>16;
        int Load , temp = 0 , A , E;
        unsigned int B,C,D;
        unsigned int Sa,Sb,Sc,Sd;
        ITLBHit(Pc);
        Load = Reg[RS] + Simd;
        debug_print(0,0,OP,FN,RS,RT,RD,Load);
        Reg[0] = 0;
        if(OP == halt)break;
        if(OP == 0)
        {
            switch(FN)
            {
            case add:
                Reg[RD] = Reg[RS] + Reg[RT];
                Pc = Pc+4;
                break;
            case addu:
                Reg[RD] = Reg[RS] + Reg[RT];
                Pc = Pc+4;
                break;
            case sub:
                Reg[RD] = Reg[RS] - Reg[RT];
                Pc = Pc+4;
                break;
            case and:
                Reg[RD] = Reg[RS] & Reg[RT];
                Pc = Pc+4;
                break;
            case or:
                Reg[RD] = Reg[RS] | Reg[RT];
                Pc = Pc+4;
                break;
            case xor:
                Reg[RD] = Reg[RS] ^ Reg[RT];
                Pc = Pc+4;
                break;
            case nor:
                Reg[RD] =~(Reg[RS] | Reg[RT]);
                Pc = Pc+4;
                break;
            case nand:
                Reg[RD] = ~(Reg[RS] & Reg[RT]);
                Pc = Pc+4;
                break;
            case slt:
                A = Reg[RS] , E = Reg[RT];
                Reg[RD] = A < E ? 1:0;
                Pc = Pc+4;
                break;
            case sll:
                Reg[RD] = Reg[RT] << SH;
                Pc = Pc+4;
                break;
            case srl:
                Reg[RD] = Reg[RT] >> SH;
                Pc = Pc+4;
                break;
            case sra:
                A = Reg[RT];
                A = A>>SH;
                Reg[RD] = A;
                Pc = Pc+4;
                break;
            case jr:
                Pc = Reg[RS];
                break;
            default:
                Pc = Pc+4;
                break;
            }
        }
        else
        {
            switch(OP)
            {
            case addi:
                Reg[RT] = Reg[RS] + Simd;
                Pc = Pc+4;
                break;
            case addiu:
                Reg[RT] = Reg[RS] + Simd;
                Pc = Pc+4;
                break;
            case lw:
                Load = Reg[RS] + Simd;
                A = D_disk[Load]; B = D_disk[Load+1]; C = D_disk[Load+2]; D = D_disk[Load+3];
                A = A << 24; B = B<<24>>8 ; C = C<<24>>16 ; D = D<<24>>24;
                E = A+B+C+D;
                Reg[RT] = E;
                Pc = Pc+4;
                DTLBHit(Load);
                break;
            case lh:
                Load = Reg[RS] + Simd;
                A = D_disk[Load]; C = D_disk[Load+1];
                A = A<<24>>16 ; C = C<<24>>24;
                B = A+C;
                Reg[RT] = B;
                Pc = Pc+4;
                DTLBHit(Load);
                break;
            case lhu:
                Load = Reg[RS] + Simd;
                B = D_disk[Load] ; C = D_disk[Load+1];
                B = B<<24>>16 ; C = C<<24>>24;
                D = B+C;
                Reg[RT] = D;
                Pc = Pc+4;
                DTLBHit(Load);
                break;
            case lb:
                Load = Reg[RS] + Simd;
                temp = (D_disk[Load])&0x000000ff;
                temp = temp<<24>>24;
                Reg[RT] = temp;;
                Pc = Pc+4;
                DTLBHit(Load);
                break;
            case lbu:
                Load = Reg[RS] + Simd;
                B = D_disk[Load]&0x000000ff;
                Reg[RT] = B;
                Pc = Pc+4;
                DTLBHit(Load);
                break;
            case sw :
                Load = Reg[RS] + Simd;
                Sa = (Reg[RT]&0xff000000)>>24;
                Sb = (Reg[RT]&0x00ff0000)>>16;
                Sc = (Reg[RT]&0x0000ff00)>>8;
                Sd =  Reg[RT]&0x000000ff;
                D_disk[Load] = Sa;
                D_disk[Load+1] = Sb;
                D_disk[Load+2] = Sc;
                D_disk[Load+3] = Sd;
                Pc = Pc+4;
                DTLBHit(Load);
                break;
            case sh:
                Load = Reg[RS] + Simd;
                Sa = (Reg[RT]&0x0000ff00)>>8;
                Sb = (Reg[RT]&0x000000ff);
                D_disk[Load] = Sa;
                D_disk[Load+1] = Sb;
                Pc = Pc+4;
                DTLBHit(Load);
                break;
            case sb:
                Load = Reg[RS] + Simd;
                Sa = (Reg[RT]&0x000000ff);
                D_disk[Load] = Sa;
                Pc = Pc+4;
                DTLBHit(Load);
                break;
            case lui:
                Reg[RT] = IMD << 16;
                Pc = Pc+4;
                break;
            case andi:
                Reg[RT] = Reg[RS]& IMD;
                Pc = Pc+4;
                break;
            case ori:
                Reg[RT] = Reg[RS] | IMD;
                Pc = Pc+4;
                break;
            case nori:
                Reg[RT] = ~(Reg[RS]|IMD);
                Pc = Pc+4;
                break;
            case slti:
                Reg[RT] = (signed)(Reg[RS]) < Simd;
                Pc = Pc+4;
                break;
            case beq:
                if(Reg[RS] == Reg[RT]) Pc = Pc+4+4*Simd;
                else Pc = Pc+4;
                break;
            case bne:
                if(Reg[RS] != Reg[RT]) Pc = Pc+4+4*Simd;
                else Pc = Pc+4;
                break;
            case bgtz:
                if((signed)(Reg[RS]) > 0) Pc = Pc+4+4*Simd;
                else Pc = Pc+4;
                break;
            case jal:
                Reg[31] = Pc+4;
                Pc = (Pc+4)>>28<<28 | (AD*4);
            case j:
                Pc = ((Pc+4)>>28<<28) | (AD*4);
                break;
            default:
                Pc = Pc+4;
                break;
            }
        }
        Reg[0] = 0;
        Cycle++;
        Take_snap();
    }
    Take_report();
    /*
    printf("\n\nI tlb hit: %2d miss: %2d\n",I_tlb_hit,I_tlb_miss);
    printf("I pte hit: %2d miss: %2d\n",I_pte_hit,I_pte_miss);
    printf("I cah hit: %2d miss: %2d\n\n",I_cah_hit,I_cah_miss);

    printf("D tlb hit: %2d miss: %2d\n",D_tlb_hit,D_tlb_miss);
    printf("D pte hit: %2d miss: %2d\n",D_pte_hit,D_pte_miss);
    printf("D cah hit: %2d miss: %2d\n",D_cah_hit,D_cah_miss);
    */
    return 0;
}
