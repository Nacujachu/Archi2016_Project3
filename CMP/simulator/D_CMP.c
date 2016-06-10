#include "D_CMP.h"
#include "var.h"

int to_be_place;
/*
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
*/
void DTLBHit(unsigned int VM)
{
    unsigned Vpn = VM/D_page_size;
    int i  , find = 0 , V_count = 0 , Min_cycle = 1<<20 , idx=-1 ;
    unsigned int PA , offset;

    for(i=0 ; i<D_TLB_number ; i++)
    {
        if(D_TLB_table[i].Valid == 1 && D_TLB_table[i].VPN == Vpn)
        {
            find = 1;
            PPNcur = D_TLB_table[i].PPN;
            D_TLB_table[i].Last_cycle = Cycle;
          //  printf("hit in:%d Vpn: %d\n",Cycle,Vpn);

            break;
        }
    }
    if(find == 0)
    {
        D_tlb_miss++;
        //printf("\n\n%d %d %d\n\n",Cycle,PPNcur,VM);
        DPTEHit(VM);
      //  printf("\n\n%d %d %d\n\n",Cycle,PPNcur,VM);
        for(i=0; i<D_TLB_number; i++)
        {
            if(D_TLB_table[i].Valid == 1)
            {
                V_count++;
            }
            if(D_TLB_table[i].Valid == 0)
            {
                idx = i;
                V_count = 0;
                break;
            }
        }

        if(V_count == D_TLB_number)
        {
            for(i=0 ; i<D_TLB_number ; i++)
            {
                if(D_TLB_table[i].Last_cycle < Min_cycle)
                {
                    Min_cycle = D_TLB_table[i].Last_cycle;
                    idx = i;
                }
            }
            D_TLB_table[idx].VPN = Vpn;
            D_TLB_table[idx].Last_cycle = Cycle;
            D_TLB_table[idx].PPN = PPNcur;
            D_TLB_table[idx].Valid = 1;
        }
        else
        {
            D_TLB_table[idx].Valid = 1;
            D_TLB_table[idx].VPN = Vpn;
            D_TLB_table[idx].PPN = PPNcur;
            D_TLB_table[idx].Last_cycle = Cycle;
        }
    }
    else
    {
        D_tlb_hit++;
    }

    offset = VM % D_page_size;
    PA = PPNcur*D_page_size + offset;
    D_cache(PA);
    return ;
}
unsigned int DPTEHit(unsigned int VM)
{
    unsigned int Vpn = VM/D_page_size;
    unsigned int Offset = VM%D_page_size;
    int i , find = 0;
    unsigned int new_ppn;
    to_be_place = -1;
    if(D_PTE_table[Vpn].Valid == 1 )
    {
        PPNcur = D_PTE_table[Vpn].PPN;
        D_MEM_table[PPNcur].Last_cycle = Cycle;
        find = 1;
    }
    if(find == 1)
    {
        D_pte_hit++;
    }
    else if(find == 0)
    {
        D_pte_miss++;

        new_ppn = D_Disk_to_MEM(VM);

        D_PTE_table[Vpn].Valid = 1;
        D_PTE_table[Vpn].PPN = new_ppn;
        PPNcur = new_ppn;

    }
    return PPNcur;
}
unsigned int  D_Disk_to_MEM(unsigned int VM)
{

    unsigned int Vpn = VM/D_page_size;
    int V_count = 0;
    int find = 0 , i , k;
    for(i=0 ; i< D_MEM_number ; i++)
    {
        if(D_MEM_table[i].Valid == 1)
        {
            V_count++;
        }
        if(D_MEM_table[i].Valid == 0)
        {
            D_MEM_table[i].Last_cycle = Cycle;
            D_MEM_table[i].Valid = 1;
            return i;
        }
    }
    if(V_count == D_MEM_number)
    {
        //printf("\nPF:%d\n",Cycle);
        unsigned int Min_cycle = 500000 , idx;
        for(i=0 ; i<D_MEM_number; i++)
        {
            if( D_MEM_table[i].Last_cycle < Min_cycle )
            {
                Min_cycle = D_MEM_table[i].Last_cycle;
                to_be_place = i;
                idx = i;
            }
        }
        D_MEM_table[idx].Last_cycle = Cycle;
        for(i=0;i<D_PTE_number;i++)
        {
            if(D_PTE_table[i].PPN == idx && D_PTE_table[i].Valid == 1)
            {
                D_PTE_table[i].Valid = 0;
                D_PTE_table[i].PPN = 0;
            }
        }
        for(i=0;i<D_TLB_number;i++)
        {
            if(D_TLB_table[i].PPN == idx && D_TLB_table[i].Valid == 1)
            {
                D_TLB_table[i].Valid = 0;
                D_TLB_table[i].PPN = 0;
                D_TLB_table[i].VPN = 0;
                break;
            }
        }
        unsigned int PA = idx*D_page_size+(VM%D_page_size);
        unsigned int index = (PA/D_block_size)%D_CAH_row;
        unsigned int tag = (PA/D_block_size)/D_CAH_row;

        for(i=0;i<D_CAH_row;i++){
            for(k=0;k<D_CAH_col;k++){
                if( (D_Cache[i][k].Tag*D_CAH_row*D_block_size+i*D_block_size)/D_page_size  == idx){
                    D_Cache[i][k].MRU = 0;
                    D_Cache[i][k].Valid = 0;
                }
            }
        }
        return idx;
    }
    return 0;
}
void D_cache(unsigned int PA)
{
    unsigned int index , tag,i,k;
    index = (PA/D_block_size)%D_CAH_row;
    tag = (PA/D_block_size)/D_CAH_row;
    for(i=0; i<D_CAH_col; i++)
    {
        if(tag == D_Cache[index][i].Tag && D_Cache[index][i].Valid == 1)
        {
            D_cah_hit++;
            D_Cache[index][i].MRU = 1;

            for(k=0;k<D_CAH_col;k++){
                if(D_Cache[index][k].MRU == 0) return;
            }
            for(k=0;k<D_CAH_col;k++){
                if(k!=i || D_CAH_col == 1 ){D_Cache[index][k].MRU = 0;}
            }

            return ;
        }
    }

    D_cah_miss++;

    for(i=0 ; i<D_CAH_col; i++)
    {
        if(D_Cache[index][i].Valid == 0)
        {
            D_Cache[index][i].Valid = 1;
            D_Cache[index][i].Tag = tag;
            D_Cache[index][i].MRU = 1;
            for(k = 0 ; k<D_CAH_col; k++)
            {
                if(D_Cache[index][k].MRU == 0)return;
            }
            for(k=0; k<D_CAH_col; k++)
            {
                if(k!=i || D_CAH_col == 1) D_Cache[index][k].MRU = 0;
            }
            return ;
        }
    }

    for(i=0 ; i<D_CAH_col ; i++)
    {
        if(D_Cache[index][i].MRU == 0)
        {
            D_Cache[index][i].Tag = tag;
            D_Cache[index][i].Valid = 1;
            D_Cache[index][i].MRU = 1;
            for(k = 0 ; k<D_CAH_col; k++)
            {
                if(D_Cache[index][k].MRU == 0)return;
            }
            for(k=0; k<D_CAH_col; k++)
            {
                if(k!=i || D_CAH_col == 1) D_Cache[index][k].MRU = 0;
            }
            return ;
        }
    }

}
