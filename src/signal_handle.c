/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 00:45:58 by jye               #+#    #+#             */
/*   Updated: 2017/11/27 05:20:33 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <term.h>
#include <curses.h>

#include "ft_select.h"
#include "ft_printf.h"

void	select_suspend(int sig)
{
	end_select_mode();
	kill(0, sig);
}

void	select_restart(int sig)
{
	struct sigaction	act;

	(void)sig;
	if (init_non_canon())
		exit(EXIT_FAILURE);
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART | SA_RESETHAND;
	act.sa_handler = select_suspend;
	sigaction(SIGTSTP, &act, NULL);
	ft_dprintf(2, "%s", g_caps[TI]);
	ft_dprintf(2, "%s", g_caps[VI]);
	start_select_mode(1);
}

void	select_winch(int sig)
{
	sigset_t	set;
	int			current;
	int			curmax;

	(void)sig;
	sigemptyset(&set);
	sigaddset(&set, SIGWINCH);
	sigprocmask(SIG_BLOCK, &set, NULL);
	update_termsize();
	TSETC(0, 0);
	ft_dprintf(2, "%s", g_caps[CD]);
	current = (g_column[0].info_size * g_cur_col) + g_cur_row;
	curmax = set_column_infodata();
	g_cur_col = current / curmax;
	g_cur_row = current % curmax;
	start_select_mode(1);
	select_output(g_column + g_cur_col);
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
