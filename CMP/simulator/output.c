#include "output.h"
#include <stdio.h>
#include <stdlib.h>
#include "var.h"
void Take_snap()
{
    int i;
    fprintf(Snapshot,"cycle %d\n",Cycle);
    for(i=0; i<33; i++)
    {
        if(i<10)
        {
            fprintf(Snapshot,"$0%d: 0x",i);
        }
        else if(i==32)
        {
            fprintf(Snapshot,"PC: 0x");
        }
        else
        {
            fprintf(Snapshot,"$%d: 0x",i);
        }
        unsigned int T1,T2,T3,T4,T5,T6,T7,T8;
        if(i==32)
        {
            T1=(Pc&0xF0000000)>>28;
            T2=(Pc&0x0F000000)>>24;
            T3=(Pc&0x00F00000)>>20;
            T4=(Pc&0x000F0000)>>16;
            T5=(Pc&0x0000F000)>>12;
            T6=(Pc&0x00000F00)>>8;
            T7=(Pc&0x000000F0)>>4;
            T8=(Pc&0x00000000F);
        }
        else
        {
            T1=(Reg[i]&0xF0000000)>>28;
            T2=(Reg[i]&0x0F000000)>>24;
            T3=(Reg[i]&0x00F00000)>>20;
            T4=(Reg[i]&0x000F0000)>>16;
            T5=(Reg[i]&0x0000F000)>>12;
            T6=(Reg[i]&0x00000F00)>>8;
            T7=(Reg[i]&0x000000F0)>>4;
            T8=(Reg[i]&0x00000000F);
        }

        fprintf(Snapshot,"%X%X%X%X%X%X%X%X\n",T1,T2,T3,T4,T5,T6,T7,T8);
        if(i==32)fprintf(Snapshot,"\n");
    }
    fprintf(Snapshot,"\n");

}

void Take_report()
{
    fprintf(Report,"ICache :\n");
    fprintf(Report,"# hits: %u\n", I_cah_hit);
    fprintf(Report,"# misses: %u\n\n", I_cah_miss);
    fprintf(Report,"DCache :\n");
    fprintf(Report,"# hits: %u\n", D_cah_hit);
    fprintf(Report,"# misses: %u\n\n", D_cah_miss);
    fprintf(Report,"ITLB :\n");
    fprintf(Report,"# hits: %u\n", I_tlb_hit );
    fprintf(Report,"# misses: %u\n\n", I_tlb_miss );
    fprintf(Report,"DTLB :\n");
    fprintf(Report,"# hits: %u\n", D_tlb_hit );
    fprintf(Report,"# misses: %u\n\n", D_tlb_miss );
    fprintf(Report,"IPageTable :\n");
    fprintf(Report,"# hits: %u\n", I_pte_hit );
    fprintf(Report,"# misses: %u\n\n", I_pte_miss );
    fprintf(Report,"DPageTable :\n");
    fprintf(Report,"# hits: %u\n", D_pte_hit );
    fprintf(Report,"# misses: %u\n\n", D_pte_miss );
}
