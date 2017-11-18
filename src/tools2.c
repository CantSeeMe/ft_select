/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 00:33:46 by jye               #+#    #+#             */
/*   Updated: 2017/11/18 01:36:18 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <term.h>
#include <curses.h>

#include "ft_printf.h"
#include "ft_select.h"

int		end_select_mode(void)
{
	ft_dprintf(2, "%s", g_caps[TE]);
	ft_dprintf(2, "%s", g_caps[VS]);
	return (tcsetattr(STDIN_FILENO, TCSANOW, &g_otermios));
}

void	start_select_mode(int reset)
{
	ft_dprintf(2, "%s", g_caps[TI]);
	set_help_windows();
	if (reset)
		set_column_infodata();
	select_output(g_column + g_cur_col);
	update_page();
	g_column[g_cur_col].info[g_cur_row]->state |= SL_CURSOR;
	ft_dprintf(2, "%s", g_caps[VI]);
}

void	update_page(void)
{
	if (!g_winhelp)
		return ;
	TSETCURSOR(WINHELP_PAGE_ROW, WINHELP_PAGE_COL);
	ft_dprintf(2, "%d / %d        ", g_cur_col + 1, g_ncolumn);
}

int		can_set_winhelp(void)
{
	return (g_winhelp = (g_termsize.col >= MIN_COL_WINHELP &&
					g_termsize.row >= MIN_ROW_WINHELP + 3));
}

void	set_help_windows(void)
{
	if (!(can_set_winhelp()))
		return ;
	TSETCURSOR(0, 0);
	ft_dprintf(2, "********************************"
			"**********************************\n");
	ft_dprintf(2, "* backspace / delete: remove "
			"current      arrow up   : move up   *\n");
	ft_dprintf(2, "* space             : select "
			"current      arrow down : move down *\n");
	ft_dprintf(2, "* enter             : print "
			"selected      arrow right: next page *\n");
	ft_dprintf(2, "* q / esc           : quit  "
			"arrow left : prev page *\n");
	ft_dprintf(2, "*                          pa"
			"ge:                                 *\n");
	ft_dprintf(2, "*******************************"
			"***********************************\n");
	update_page();
}
