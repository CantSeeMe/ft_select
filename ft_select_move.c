/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select_move.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/16 02:07:54 by jye               #+#    #+#             */
/*   Updated: 2017/11/16 11:40:25 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"
#include <term.h>
#include <curses.h>

void	select_move_up(void)
{
	select_refresh(column[cur_col].info[cur_row], SL_REMOVE, SL_CURSOR);
	if (--cur_row < 0)
	{
		cur_col += (cur_col < 0) * ncolumn;
		cur_row = column[cur_col].info_size - 1;
		select_output(column + cur_col);
	}
	select_refresh(column[cur_col].info[cur_row], SL_ADD, SL_CURSOR);
}

void	select_move_down(void)
{
	select_refresh(column[cur_col].info[cur_row], SL_REMOVE, SL_CURSOR);
	if (++cur_row >= column[cur_col].info_size)
	{
		cur_col *= (cur_col < ncolumn);
		cur_row = 0;
		select_output(column + cur_col);
	}
	select_refresh(column[cur_col].info[cur_row], SL_ADD, SL_CURSOR);
}

void	select_move_left(void)
{
	int		infosize;

	select_refresh(column[cur_col--].info[cur_row], SL_REMOVE, SL_CURSOR);
	cur_col += (cur_col < 0) * ncolumn;
	select_output(column + cur_col);
	infosize = column[cur_col].info_size;
	cur_row += (cur_row >= infosize) * (infosize - cur_row - 1);
	select_refresh(column[cur_col].info[cur_row], SL_ADD, SL_CURSOR);
}

void	select_move_right(void)
{
	int		infosize;

	select_refresh(column[cur_col++].info[cur_row], SL_REMOVE, SL_CURSOR);
	cur_col *= (cur_col < ncolumn);
	select_output(column + cur_col);
	infosize = column[cur_col].info_size;
	cur_row += (cur_row >= infosize) * (infosize - cur_row - 1);
	select_refresh(column[cur_col].info[cur_row], SL_ADD, SL_CURSOR);
}

void	select_current(void)
{
	if (column[cur_col].info[cur_row]->state & SL_SELECTED)
	{
		selected--;
		select_refresh(column[cur_col].info[cur_row], SL_REMOVE, SL_SELECTED);
	}
	else
	{
		selected++;
		select_refresh(column[cur_col].info[cur_row], SL_ADD, SL_SELECTED);
	}
	select_move_down();
}

void	delete_current(void)
{
	int		infosize;

	TSETCURSOR(0 + (MIN_ROW_WINHELP * winhelp), 0);
	dprintf(2, "%s", caps[CD]);
	column[cur_col].info[cur_row]->state &= ~(SL_ALIVE | SL_SELECTED);
	eleminfo--;
	set_column_infodata();
	infosize = column[cur_col].info_size;
	cur_row += (cur_row >= infosize) * (infosize - cur_row - 1);
	column[cur_col].info[cur_row]->state |= SL_CURSOR;
	select_output(column + cur_col);
	TSETCURSOR(cur_row + (MIN_ROW_WINHELP * winhelp), 0);
}

void	done(void)
{
	int		i;

	i = 0;
	while (i < maxinfo)
	{
		if (datainfo[i].state & (SL_SELECTED))
		{
			dprintf(1, "%s", datainfo[i].s);
			if (--selected)
				dprintf(1, " ");
		}
		i++;
	}
	eleminfo = 0;
}
