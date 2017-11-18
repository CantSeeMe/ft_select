/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select_move.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/16 02:07:54 by jye               #+#    #+#             */
/*   Updated: 2017/11/18 01:27:18 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"
#include <term.h>
#include <curses.h>

void	select_move_up(void)
{
	select_refresh(g_column[g_cur_col].info[g_cur_row], SL_REMOVE, SL_CURSOR);
	if (--g_cur_row < 0)
	{
		g_cur_col -= 1;
		g_cur_col += (g_cur_col < 0) * g_ncolumn;
		g_cur_row = g_column[g_cur_col].info_size - 1;
		update_page();
		select_output(g_column + g_cur_col);
	}
	select_refresh(g_column[g_cur_col].info[g_cur_row], SL_ADD, SL_CURSOR);
}

void	select_move_down(void)
{
	select_refresh(g_column[g_cur_col].info[g_cur_row], SL_REMOVE, SL_CURSOR);
	if (++g_cur_row >= g_column[g_cur_col].info_size)
	{
		g_cur_col += 1;
		g_cur_col *= (g_cur_col < g_ncolumn);
		g_cur_row = 0;
		update_page();
		select_output(g_column + g_cur_col);
	}
	select_refresh(g_column[g_cur_col].info[g_cur_row], SL_ADD, SL_CURSOR);
}

void	select_move_left(void)
{
	int		infosize;

	select_refresh(g_column[g_cur_col--].info[g_cur_row], SL_REMOVE, SL_CURSOR);
	g_cur_col += (g_cur_col < 0) * g_ncolumn;
	update_page();
	select_output(g_column + g_cur_col);
	infosize = g_column[g_cur_col].info_size;
	g_cur_row += (g_cur_row >= infosize) * (infosize - g_cur_row - 1);
	select_refresh(g_column[g_cur_col].info[g_cur_row], SL_ADD, SL_CURSOR);
}

void	select_move_right(void)
{
	int		infosize;

	select_refresh(g_column[g_cur_col++].info[g_cur_row], SL_REMOVE, SL_CURSOR);
	g_cur_col *= (g_cur_col < g_ncolumn);
	update_page();
	select_output(g_column + g_cur_col);
	infosize = g_column[g_cur_col].info_size;
	g_cur_row += (g_cur_row >= infosize) * (infosize - g_cur_row - 1);
	select_refresh(g_column[g_cur_col].info[g_cur_row], SL_ADD, SL_CURSOR);
}
