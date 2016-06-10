#include "var.h"
#include "data_io.h"
unsigned char I_buf[999999] , D_buf[9999999];
void initial()
{
    int i,k,n;
    int I_size , D_size , I_num , D_num , I_idx = 0 , D_idx = 0 , idx = 0;

    Ibin = fopen("iimage.bin","rb");
    Dbin = fopen("dimage.bin","rb");
    Snapshot = fopen("snapshot.rpt","w");
    Report = fopen("report.rpt","w");

    Cycle = 0;
    I_disk_size = 1024 ;
    D_disk_size = 1024;
    for(i=0; i<1024; i++) I_disk[i] = 0;
    for(i=0; i<1024; i++) D_disk[i] = 0;

    fseek(Ibin , 0 , SEEK_END);
    fseek(Dbin , 0 , SEEK_END);
    I_size = ftell(Ibin) , D_size = ftell(Dbin);
    rewind(Ibin) ;
    rewind(Dbin);
    fread(I_buf , sizeof(char) , I_size , Ibin);
    fread(D_buf , sizeof(char) , D_size , Dbin);
    k = 0;
    n= 0 ;
    for(i=0; i<4; i++)
    {
        k<<=8;
        n<<=8;
        k+=I_buf[i];
        n+=D_buf[i];
    }
    Pc = k;
    k = 0;
    Reg[29] = n;
    n = 0;
    for(i=4; i<8; i++)
    {
        k<<=8;
        n<<=8;
        k+=I_buf[i];
        n+=D_buf[i];
    }
    I_num = k;
    k = 0;
    D_num = n;
    n = 0;
    for(i=8; i<I_num*4+8; i++)
    {

        k <<= 8;
        k += I_buf[i];
        if(i%4 == 3)
        {
            ins[Pc/4+idx++] = k;
            k = 0;
        }
        I_disk[I_idx++] = I_buf[i];
    }
    for(i=8; i<D_num*4+8; i++)
    {
        D_disk[D_idx++] = D_buf[i];
    }
}
void memory_initial()
{
    int i,k;
    I_tlb_hit = 0;
    I_tlb_miss = 0;
    D_tlb_hit = 0;
    D_tlb_miss = 0;
    I_pte_hit = 0;
    I_pte_miss = 0;
    D_pte_hit = 0;
    D_pte_miss = 0;
    for(i=0; i<I_PTE_number; i++)
    {
        I_PTE_table[i].Valid = 0;
        I_PTE_table[i].PPN = 0;
    }
    for(i=0 ; i<D_PTE_number; i++)
    {
        D_PTE_table[i].Valid = 0;
        D_PTE_table[i].PPN = 0;
    }
    for(i=0; i<I_TLB_number; i++)
    {
        I_TLB_table[i].Valid = 0;
        I_TLB_table[i].PPN = 0;
        I_TLB_table[i].Last_cycle = 0;
        I_TLB_table[i].VPN = 0;
    }
    for(i=0; i<D_TLB_number; i++)
    {
        D_TLB_table[i].Valid = 0;
        D_TLB_table[i].PPN = 0;
        D_TLB_table[i].Last_cycle = 0;
        D_TLB_table[i].VPN = 0;
    }
    for(i=0; i<I_MEM_number; i++)
    {
        I_MEM_table[i].Valid = 0;
        I_MEM_table[i].Last_cycle = 0;
    }
    for(i=0; i<D_MEM_number; i++)
    {
        D_MEM_table[i].Valid = 0;
        D_MEM_table[i].Last_cycle = 0;
    }
    for(i=0; i<I_CAH_row; i++)
    {
        for(k=0; k<I_CAH_col; k++)
        {
            I_Cache[i][k].MRU = 0;
            I_Cache[i][k].Tag = 0;
            I_Cache[i][k].Valid = 0;
        }
    }
    for(i=0; i<D_CAH_row; i++)
    {
        for(k=0; k<D_CAH_col; k++)
        {
            D_Cache[i][k].MRU = 0;
            D_Cache[i][k].Tag = 0;
            D_Cache[i][k].Valid = 0;
        }
    }
}
