/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 16:22:29 by jye               #+#    #+#             */
/*   Updated: 2017/11/27 05:20:16 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <curses.h>
#include <term.h>

#include "ft_select.h"
#include "ft_printf.h"

void	output(t_datainfo *info)
{
	int		state;

	state = info->state;
	if (state & SL_SELECTED)
		ft_dprintf(2, "%s", g_caps[MR]);
	if (state & SL_CURSOR)
		ft_dprintf(2, "%s", g_caps[US]);
	if (g_termsize.col < info->len)
		ft_dprintf(2, "%.*s%.*s%s",
				(g_termsize.col > 3) * (g_termsize.col - 3), info->s,
				g_termsize.col < 3 ? g_termsize.col % 3 : 3, "...",
				g_caps[ME]);
	else
		ft_dprintf(2, "%s%s", info->s, g_caps[ME]);
}

void	select_output(t_column *col)
{
	int			max;
	int			i;
	int			row;

	max = col->info_size;
	i = 0;
	row = g_winhelp * MIN_ROW_WINHELP;
	TSETC(row, 0);
	ft_dprintf(2, "%s", g_caps[CD]);
	while (i < max)
	{
		TSETC(row + i, 0);
		output(col->info[i++]);
	}
}

void	select_refresh(t_datainfo *info, int options, int state)
{
	state = options ? ~state & info->state : state | info->state;
	info->state = state;
	TSETC(g_cur_row + (MIN_ROW_WINHELP * g_winhelp), 0);
	output(info);
}

void	key_event(uint64_t k)
{
	if (k == KEY_BACKSPACE || k == KEY_DELETE)
		delete_current();
	else if (k == KEY_ARROW_UP)
		select_move_up();
	else if (k == KEY_ARROW_DOWN)
		select_move_down();
	else if (k == KEY_ARROW_LEFT)
		select_move_left();
	else if (k == KEY_ARROW_RIGHT)
		select_move_right();
	else if (k == KEY_SPACEBAR)
		select_current();
	else if (k == KEY_ENTER)
		done();
	else if (k == 'q' || k == KEY_ESC)
		select_quit();
}

int		main(int ac, char **av)
{
	uint64_t			k;

	ft_qsort((void **)(av + 1), ac - 1, ft_strcmp);
	if (init_non_canon())
		return (1);
	if (init_select_mode() || init_datainfo(ac, av) || init_column())
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &g_otermios);
		return (1);
	}
	ft_dprintf(2, "%s", g_caps[TI]);
	ft_dprintf(2, "%s", g_caps[VI]);
	start_select_mode(1);
	signal_handle();
	while (g_eleminfo)
	{
		k = 0;
		read(STDIN_FILENO, &k, sizeof(k));
		key_event(k);
	}
	end_select_mode();
	print_selected();
	return (0);
}
