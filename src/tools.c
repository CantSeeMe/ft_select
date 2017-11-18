/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 00:30:57 by jye               #+#    #+#             */
/*   Updated: 2017/11/18 01:43:23 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <term.h>
#include <curses.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "ft_printf.h"
#include "ft_select.h"

static char *g_termcapid[CAPNO] = {
	"cm", "ti", "te", "me", "mb", "md",
	"mr", "us", "so", "vi", "vs", "cd"
};

int		update_termsize(void)
{
	struct winsize w;

	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &w))
		return (1);
	g_termsize.col = w.ws_col;
	g_termsize.row = w.ws_row;
	return (0);
}

int		init_non_canon(void)
{
	struct termios termios;

	if (tcgetattr(STDIN_FILENO, &termios))
	{
		ft_dprintf(2, "%s: could not retrieve data from stdin\n", PROGRAM_NAME);
		return (1);
	}
	g_otermios = termios;
	termios.c_lflag &= ~(ECHO | ECHONL | ECHOK | ICANON);
	termios.c_lflag |= ISIG;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &termios))
	{
		ft_dprintf(2, "%s: could not set termios on stdin\n", PROGRAM_NAME);
		return (1);
	}
	return (0);
}

void	init_termcap(void)
{
	int		i;

	i = 0;
	while (i < CAPNO)
	{
		g_caps[i] = tgetstr(g_termcapid[i], 0);
		i++;
	}
}

int		test_termcaps(void)
{
	int	r;

	r = 0;
	init_termcap();
	while (r < CAPNO)
	{
		if (!g_caps[r++])
			return (1);
	}
	return (0);
}

int		init_select_mode(void)
{
	if (tgetent(getenv("TERM"), 0) <= 0)
	{
		ft_dprintf(2, "%s: could not find terminal definition\n", PROGRAM_NAME);
		return (1);
	}
	if (update_termsize())
	{
		ft_dprintf(2, "%s: cannot get terminal size\n", PROGRAM_NAME);
		return (1);
	}
	if (test_termcaps())
	{
		ft_dprintf(2, "%s: one of the capability is missing\n", PROGRAM_NAME);
		return (1);
	}
	return (0);
}
