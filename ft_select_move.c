/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select_move.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/16 02:07:54 by jye               #+#    #+#             */
/*   Updated: 2017/11/17 14:44:22 by root             ###   ########.fr       */
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
		cur_col -= 1;
		cur_col += (cur_col < 0) * ncolumn;
		cur_row = column[cur_col].info_size - 1;
		update_page();
		select_output(column + cur_col);
	}
	select_refresh(column[cur_col].info[cur_row], SL_ADD, SL_CURSOR);
}

void	select_move_down(void)
{
	select_refresh(column[cur_col].info[cur_row], SL_REMOVE, SL_CURSOR);
	if (++cur_row >= column[cur_col].info_size)
	{
		cur_col += 1;
		cur_col *= (cur_col < ncolumn);
		cur_row = 0;
		update_page();
		select_output(column + cur_col);
	}
	select_refresh(column[cur_col].info[cur_row], SL_ADD, SL_CURSOR);
}

void	select_move_left(void)
{
	int		infosize;

	select_refresh(column[cur_col--].info[cur_row], SL_REMOVE, SL_CURSOR);
	cur_col += (cur_col < 0) * ncolumn;
	update_page();
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
	update_page();
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
	update_page();
	select_output(column + cur_col);
	TSETCURSOR(cur_row + (MIN_ROW_WINHELP * winhelp), 0);
}

void	done(void)
{
	eleminfo = 0;
}

void	print_selected(void)
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
	dprintf(1, "\n");
}

void	update_page(void)
{
	if (!winhelp)
		return ;
	TSETCURSOR(WINHELP_PAGE_ROW, WINHELP_PAGE_COL);
	dprintf(2, "%d/%d", cur_col + 1, ncolumn);
}

#include <signal.h>
#include <stdlib.h>

void	select_suspend(int sig)
{
	end_select_mode();
	kill(0, sig);
}

void	select_restart(int sig)
{
	struct sigaction act;

	(void)sig;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART | SA_RESETHAND;
	act.sa_handler = select_suspend;
	sigaction(SIGTSTP, &act, NULL);
	start_select_mode();
}

void	select_winch(int sig)
{
	t_datainfo	*current;
	int			curmax;
	sigset_t	set;

	(void)sig;
	sigemptyset(&set);
	sigaddset(&set, SIGWINCH);
	sigprocmask(SIG_BLOCK, &set, NULL);
	update_termsize();
	TSETCURSOR(0, 0);
	dprintf(2, "%s", caps[CD]);
	current = column[cur_col].info[cur_row];
	curmax = set_column_infodata();
	cur_col = (current - datainfo) / curmax;
	cur_row = (current - datainfo) % curmax;
	set_help_windows();
	select_output(column + cur_col);
	sigprocmask(SIG_UNBLOCK, &set, NULL);
}

void	signal_handle(void)
{
	struct sigaction act;

	sigemptyset(&act.sa_mask);
	act.sa_handler = select_suspend;
	act.sa_flags = SA_RESETHAND;
	sigaction(SIGINT, &act, NULL);
	act.sa_flags |= SA_RESTART;
	sigaction(SIGTSTP, &act, NULL);
	act.sa_flags &= ~SA_RESETHAND;
	act.sa_handler = select_winch;
	sigaction(SIGWINCH, &act, NULL);
	act.sa_handler = select_restart;
	sigaction(SIGCONT, &act, NULL);
}
