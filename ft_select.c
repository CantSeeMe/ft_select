/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 16:22:29 by jye               #+#    #+#             */
/*   Updated: 2017/10/29 21:57:26 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"
#include "lst.h"

#include <termios.h>
#include <term.h>
#include <curses.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/ioctl.h>

struct termios g_otermios;
t_column	   **column;
t_column       *cur_col;
int			   dsize;
t_datainfo	   *datainfo;
t_datainfo     *cur_data;
t_xy           termsize;
t_xy           cursor;

int		update_termsize(void)
{
	struct winsize w;

	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &w))
		return (1);
	termsize.x = w.ws_col;
	termsize.y = w.ws_row;
	return (0);
}

int		init_non_canon(void)
{
	struct termios termios;

	if (tcgetattr(STDIN_FILENO, &termios))
	{
		dprintf(2, "%s: could not retrieve data from stdin\n", PROGRAM_NAME);
		return (1);
	}
	g_otermios = termios;
	termios.c_lflag &= ~(ECHO | ECHONL | ECHOK | ICANON);
	if (tcsetattr(STDIN_FILENO, &termios, TCSANOW))
	{
		dprintf(2, "%s: could not set termios on stdin\n", PROGRAM_NAME);
		return (1);
	}
	return (0);
}

int		init_select_mode(void)
{
	if (tgetent(getenv("TERM")) > 0)
	{
		dprintf(2, "%s: could not find terminal definition\n", PROGRAM_NAME);
		return (1);
	}
	if (update_termsize())
	{
		dprintf(2, "%s: cannot get terminal size\n", PROGRAM_NAME);
		return (1);
	}
	if (test_termcaps())
	{
		dprintf(2, "%s: one of the definition is missing\n", PROGRAM_NAME);
		return (1);
	}
	return (0);
}

int		init_datainfo(int ac, char **av)
{
	tputs(tgetstr("ti"), 0, putchar_);
	dsize = ac - 1;
}

// start ti end te save/restore term state

int		restore_terminal(void)
{
	return (tcsetattr(STDIN_FILENO, &g_otermios, TCSANOW));
}

void	key_event(uint64_t k)
{
	if (k == KEY_ARROW_UP)
		;
	else if (k == KEY_ARROW_DOWN)
		;
	else if (k == KEY_ARROW_LEFT)
		;
	else if (k == KEY_ARROW_RIGHT)
		;
	else if (k == KEY_SPACEBAR)
		;
	else if (k == KEY_BACKSPACE)
		;
	else if (k == KEY_DELETE)
		;
}

int		test_termcaps(void)
{
	int	r;

	r = 0;
	r |= !tgetstr("ti", 0); // save termstate
	r |= !tgetstr("te", 0); // restore termstate
	r |= !tgetstr("do", 0); // down line
	r |= !tgetstr("up", 0); // up line 
	r |= !tgetstr("le", 0); // left line
	r |= !tgetstr("nd", 0); // right line
	r |= !tgetstr("NC", 0); // underline bold etc...
	return (r);
}

void	init_sigaction(void)
{
	struct sigaction s;

	sigfillset(&s.sa_mask);
	s.sa_flags = SA_RESTART;
	s.sa_handler
}

int		main(int ac, char **av)
{
	uint64_t			k;

	if (init_non_canon())
		return (1);
	if (init_select_mode() || init_datainfo())
	{
		restore_terminal();
		return (1);
	}
	init_sigaction();
	while (0xdeadbeef)
	{
		k = 0;
		read(STDIN_FILENO, &k, sizeof(k));
		key_event(k);
		refresh_column();
		refresh_screen();
	}
	tputs(tgetstr("te"), 0, putchar_);
	return (0);
}
