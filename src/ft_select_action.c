/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select_action.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 00:48:09 by jye               #+#    #+#             */
/*   Updated: 2017/11/25 00:43:32 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term.h>
#include <curses.h>

#include "ft_select.h"
#include "ft_printf.h"

void	select_current(void)
{
	if (g_column[g_cur_col].info[g_cur_row]->state & SL_SELECTED)
	{
		g_selected--;
		select_refresh(g_column[g_cur_col].info[g_cur_row],
				SL_REMOVE, SL_SELECTED);
	}
	else
	{
		g_selected++;
		select_refresh(g_column[g_cur_col].info[g_cur_row],
				SL_ADD, SL_SELECTED);
	}
	select_move_down();
}

void	delete_current(void)
{
	int		infosize;

	TSETC(0 + (MIN_ROW_WINHELP * g_winhelp), 0);
	ft_dprintf(2, "%s", g_caps[CD]);
	g_column[g_cur_col].info[g_cur_row]->state &= ~(SL_ALIVE | SL_SELECTED);
	g_eleminfo--;
	set_column_infodata();
	if (g_eleminfo == 0)
		return ;
	infosize = g_column[g_cur_col].info_size;
	ft_dprintf(3, "%d\n", g_ncolumn);
	if (g_cur_row >= infosize)
		select_move_up();
	g_column[g_cur_col].info[g_cur_row]->state |= SL_CURSOR;
	update_page();
	select_output(g_column + g_cur_col);
	TSETC(g_cur_row + (MIN_ROW_WINHELP * g_winhelp), 0);
}

void	done(void)
{
	g_eleminfo = 0;
}

void	print_selected(void)
{
	int		i;

	i = 0;
	while (i < g_maxinfo)
	{
		if (g_datainfo[i].state & (SL_SELECTED))
		{
			ft_dprintf(1, "%s", g_datainfo[i].s);
			if (--g_selected)
				ft_dprintf(1, " ");
		}
		i++;
	}
	ft_dprintf(1, "\n");
}

void	select_quit(void)
{
	end_select_mode();
	exit(0);
}
