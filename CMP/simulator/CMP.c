#include "CMP.h"
#include "var.h"

int to_be_place;

unsigned int LOG_2(unsigned int Num)
{
    unsigned int Arr[1024];
    Arr[1] = 0 ;
    Arr[2] = 1;
    Arr[4] = 2;
    Arr[8] = 3 ;
    Arr[16]= 4;
    Arr[32]= 5;
    Arr[64]= 6 ;
    Arr[128]=7;
    Arr[256]=8;
    Arr[512]=9 ;
    Arr[1024]=10;
    return Arr[Num];
}

void ITLBHit(unsigned int VM)
{
    unsigned Vpn = VM/I_page_size;
    int i  , find = 0 , V_count = 0 , Min_cycle = 1<<20 , idx=-1 ;
    unsigned int PA , offset;

    for(i=0 ; i<I_TLB_number ; i++)
    {
        if(I_TLB_table[i].Valid == 1 && I_TLB_table[i].VPN == Vpn)
        {
            find = 1;
            PPNcur = I_TLB_table[i].PPN;
            I_TLB_table[i].Last_cycle = Cycle;
            break;
        }
    }
    if(find == 0)
    {

        I_tlb_miss++;
        IPTEHit(VM);

        for(i=0; i<I_TLB_number; i++)
        {
            if(I_TLB_table[i].Valid == 1)
            {
                V_count++;
            }
            if(I_TLB_table[i].Valid == 0)
            {
                idx = i;
                break;
            }
        }

        if(V_count == I_TLB_number)
        {
            for(i=0 ; i<I_TLB_number ; i++)
            {
                if(I_TLB_table[i].Last_cycle < Min_cycle)
                {
                    Min_cycle = I_TLB_table[i].Last_cycle;
                    idx = i;
                }
            }
            I_TLB_table[idx].VPN = Vpn;
            I_TLB_table[idx].Last_cycle = Cycle;
            I_TLB_table[idx].PPN = PPNcur;
            I_TLB_table[idx].Valid = 1;
        }
        else
        {
            I_TLB_table[idx].Valid = 1;
            I_TLB_table[idx].VPN = Vpn;
            I_TLB_table[idx].PPN = PPNcur;
            I_TLB_table[idx].Last_cycle = Cycle;
        }
    }
    else
    {
        I_tlb_hit++;
    }

    offset = VM % I_page_size;
    PA = PPNcur*I_page_size + offset;
    I_cache(PA);
    return ;
}
unsigned int IPTEHit(unsigned int VM)
{
    unsigned int Vpn = VM/I_page_size;
    unsigned int Offset = VM%I_page_size;
    int i , find = 0;
    unsigned int new_ppn;
    to_be_place = -1;
    if(I_PTE_table[Vpn].Valid == 1 )
    {
        PPNcur = I_PTE_table[Vpn].PPN;
        I_MEM_table[PPNcur].Last_cycle = Cycle;
        find = 1;
    }
    if(find == 1)
    {
        I_pte_hit++;
    }
    else if(find == 0)
    {
        I_pte_miss++;

        new_ppn = I_Disk_to_MEM(VM);

        I_PTE_table[Vpn].Valid = 1;
        I_PTE_table[Vpn].PPN = new_ppn;
        PPNcur = new_ppn;

    }
    return PPNcur;
}
unsigned int  I_Disk_to_MEM(unsigned int VM)
{
    unsigned int Vpn = VM/I_page_size;
    int V_count = 0;
    int find = 0 , i , k;
    for(i=0 ; i< I_MEM_number ; i++)
    {
        if(I_MEM_table[i].Valid == 1)
        {
            V_count++;
        }
        if(I_MEM_table[i].Valid == 0)
        {
            I_MEM_table[i].Last_cycle = Cycle;
            I_MEM_table[i].Valid = 1;
            return i;
        }
    }
    if(V_count == I_MEM_number)
    {
        unsigned int Min_cycle = 500000 , idx;
        for(i=0 ; i<I_MEM_number; i++)
        {
            if( I_MEM_table[i].Last_cycle < Min_cycle )
            {
                Min_cycle = I_MEM_table[i].Last_cycle;
                to_be_place = i;
                idx = i;
            }
        }
        I_MEM_table[idx].Last_cycle = Cycle;
        for(i=0;i<I_PTE_number;i++)
        {
            if(I_PTE_table[i].PPN == idx && I_PTE_table[i].Valid == 1)
            {
                I_PTE_table[i].Valid = 0;
                I_PTE_table[i].PPN = 0;
            }
        }
        for(i=0;i<I_TLB_number;i++)
        {
            if( I_TLB_table[i].PPN == idx && I_TLB_table[i].Valid == 1 )
            {
                I_TLB_table[i].Valid = 0;
                I_TLB_table[i].PPN = 0;
                I_TLB_table[i].VPN = 0;
                break;
            }
        }
        //*/
        unsigned int PA = idx*I_page_size+(VM%I_page_size);
        unsigned int index = (PA/I_block_size)%I_CAH_row;
        unsigned int tag = (PA/I_block_size)/I_CAH_row;
        for(i=0;i<I_CAH_row;i++){
            for(k=0;k<I_CAH_col;k++){
                if( (I_Cache[i][k].Tag*I_CAH_row*I_block_size + i*I_block_size)/I_page_size == idx){
                    I_Cache[i][k].MRU = 0;
                    I_Cache[i][k].Valid = 0;
                }
            }
        }
        return idx;
    }
    return 0;
}
void I_cache(unsigned int PA)
{
    unsigned int index , tag,i,k;

    index = (PA/I_block_size)%I_CAH_row;
    tag = (PA/I_block_size)/I_CAH_row;


    for(i=0; i<I_CAH_col; i++)
    {
        if(tag == I_Cache[index][i].Tag && I_Cache[index][i].Valid == 1)
        {
            I_cah_hit++;
            I_Cache[index][i].MRU = 1;
            for(k = 0 ; k<I_CAH_col; k++)
            {
                if(I_Cache[index][k].MRU == 0)return;
            }
            for(k=0; k<I_CAH_col; k++)
            {
                if(k!=i || I_CAH_col == 1) I_Cache[index][k].MRU = 0;
            }
            return ;
        }
    }

    I_cah_miss++;

    for(i=0 ; i<I_CAH_col; i++)
    {
        if(I_Cache[index][i].Valid == 0)
        {
            I_Cache[index][i].Valid = 1;
            I_Cache[index][i].Tag = tag;
            I_Cache[index][i].MRU = 1;
            for(k = 0 ; k<I_CAH_col; k++)
            {
                if(I_Cache[index][k].MRU == 0)return;
            }
            for(k=0; k<I_CAH_col; k++)
            {
                if(k!=i || I_CAH_col == 1) I_Cache[index][k].MRU = 0;
            }
            return ;
        }
    }

    for(i=0 ; i<I_CAH_col ; i++)
    {
        if(I_Cache[index][i].MRU == 0)
        {
            I_Cache[index][i].Tag = tag;
            I_Cache[index][i].Valid = 1;
            I_Cache[index][i].MRU = 1;
            for(k = 0 ; k<I_CAH_col; k++)
            {
                if(I_Cache[index][k].MRU == 0)return;
            }
            for(k=0; k<I_CAH_col; k++)
            {
                if(k!=i || I_CAH_col == 1) I_Cache[index][k].MRU = 0;
            }
            return ;
        }
    }

}
