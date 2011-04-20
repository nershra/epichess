/*	file :	b_deplace.c	*/
/*	created by :	moya_a	*/
/*	date :	Tue Apr 19 16:30:25 CEST 2011	*/


#include "b_deplace.h"
#include <unistd.h>
#include <stdlib.h>

bitboard dep_knight (bitboard init, bitboard a)
{
    bitboard poss;
    a=a;
    poss = deplace_knight_br(init);
    poss |= deplace_knight_bl(init);
    poss |= deplace_knight_fr(init);
    poss |= deplace_knight_fl(init);
    poss |= deplace_knight_rb(init);
    poss |= deplace_knight_lb(init);
    poss |= deplace_knight_rf(init);
    poss |= deplace_knight_lf(init);
    print_ech(poss & ~a);
    return poss & ~a;
}
bitboard dep_queen (bitboard init,  bitboard e, bitboard a)
{
    bitboard poss;
    poss = deplace_poss (init, deplace_b, e | BOTT_B, a);
    poss |= deplace_poss (init, deplace_f, e | TOP_B, a);
    poss |= deplace_poss (init, deplace_r, e | RIGHT_B, a);
    poss |= deplace_poss (init, deplace_l, e | LEFT_B, a);
    poss |= deplace_poss (init, deplace_br, e | BOTT_B | RIGHT_B, a);
    poss |= deplace_poss (init, deplace_bl, e | BOTT_B | LEFT_B, a);
    poss |= deplace_poss (init, deplace_fr, e | TOP_B | RIGHT_B, a);
    poss |= deplace_poss (init, deplace_fl, e | TOP_B | LEFT_B, a);
    print_ech((~init) & poss);
    return ((~init) & poss);
}
bitboard dep_rook (bitboard init,  bitboard e, bitboard a)
{
    bitboard poss;
    poss = deplace_poss (init, deplace_b, e | BOTT_B, a);
    poss |= deplace_poss (init, deplace_f, e | TOP_B, a);
    poss |= deplace_poss (init, deplace_r, e | RIGHT_B, a);
    poss |= deplace_poss (init, deplace_l, e | LEFT_B, a);
    print_ech((~init) & poss);
    return ((~init) & poss);
}
bitboard dep_bishop (bitboard init,  bitboard e, bitboard a)
{
    bitboard poss;
    poss = deplace_poss (init, deplace_br, e | BOTT_B | RIGHT_B, a);
    poss |= deplace_poss (init, deplace_bl, e | BOTT_B | LEFT_B, a);
    poss |= deplace_poss (init, deplace_fr, e | TOP_B | RIGHT_B, a);
    poss |= deplace_poss (init, deplace_fl, e | TOP_B | LEFT_B, a);
    print_ech((~init) & poss);
    return ((~init) & poss);
}
bitboard dep_king (bitboard init, bitboard a)
{
    bitboard poss = 0x00;
    poss |= deplace_b(init);
    poss |= deplace_f(init);
    poss |= deplace_r(init);
    poss |= deplace_l(init);
    poss |= deplace_br(init);
    poss |= deplace_bl(init);
    poss |= deplace_fr(init);
    poss |= deplace_fl(init);
    print_ech(poss & ~a);
    return poss & ~a;
}
bitboard dep_pawn (bitboard init, bitboard e, bitboard a, int col)
{
    bitboard poss;
    if (col)    /* initlancs */
    {
        poss = deplace_f(init);
        init = poss;
        /* si il est sur sa ligne de initase */
        if (deplace_r(init) & e)
            poss |= deplace_r(init);
        if (deplace_l(init) & e)
            poss |= deplace_l(init);
        if (!(init & (e | a)))    
            if (init & 0x0000000000FF0000 )
            {
                poss |= deplace_f(init);
            }  
    }
    else        /* noirs */
    {
        poss = deplace_b(init);
        init = poss;
        /* si il est sur sa ligne de initase */
        if (deplace_r(init) & e)
            poss |= deplace_r(init);
        if (deplace_l(init) & e)
            poss |= deplace_l(init);
        if (!(init & (e | a)))    
            if (init & 0x0000FF0000000000 )
            {
                poss |= deplace_b(init);
            }  
    }
    print_ech(poss & ~a);
    return poss & ~a;
}
bitboard deplace_poss (bitboard init, bitboard (*fct)(bitboard), bitboard l, bitboard a)
{
    a &= ~init;
	if ((init & l) || (fct(init) & a))
		return init;
	init = fct(init);
	return init | deplace_poss(init, fct, l, a);
}
void calc_dep (struct s_bb *tab, bitboard e)
{
    int         i = 1;
    tab->possib[0] = 0x00;
    for (; i < 9; i++) /* peons */
    {
        tab->possib[i] = dep_pawn(tab->pieces[i], e, tab->pieces[0], tab->color);
        tab->possib[0] |= tab->possib[i];
    }
    for (; i < 11; i++) /* tours */
    {
        tab->possib[i] = dep_rook(tab->pieces[i], e, tab->pieces[0]);
        tab->possib[0] |= tab->possib[i];
    }
    for (; i < 13; i++) /* fous */
    {
        tab->possib[i] = dep_bishop(tab->pieces[i], e, tab->pieces[0]);
        tab->possib[0] |= tab->possib[i];
    }
    for (; i < 15; i++) /* chevals */
    {
        tab->possib[i] = dep_knight(tab->pieces[i], tab->pieces[0]);
        tab->possib[0] |= tab->possib[i];
    }
    tab->possib[i] = dep_queen(tab->pieces[i], e, tab->pieces[0]);
    tab->possib[0] |= tab->possib[i];
    i++;
    tab->possib[i] = dep_king(tab->pieces[i], tab->pieces[0]);
    tab->possib[0] |= tab->possib[i];
}


void calc_all_dep (struct s_bb *white, struct s_bb *black)
{
    calc_dep (white,black->pieces[0]);
    calc_dep (black,white->pieces[0]);
}


