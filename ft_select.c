/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 16:22:29 by jye               #+#    #+#             */
/*   Updated: 2017/10/30 22:26:06 by jye              ###   ########.fr       */
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
#include <stdlib.h>
#include <string.h>

struct termios g_otermios;
t_column	   *column;
t_datainfo	   *datainfo;
char		   *caps[CAPNO];
int			   maxinfo;
int			   maxcolumn;

t_rc           termsize;
t_rc           cursor;

void	ft_qsort(void **ptr_b, ssize_t size, int (*cmp)())
{
	void		*tmp_p;
	ssize_t		lo;
	ssize_t		hi;

	if ((hi = size - 2) < 0)
		return ;
	lo = 0;
	while (0xdeadc0de)
	{
		while (lo < size - 1 && cmp(ptr_b[lo], ptr_b[size - 1]) < 0)
			lo++;
		while (lo < hi && cmp(ptr_b[hi], ptr_b[size - 1]) > 0)
			hi--;
		if (lo >= hi)
			break ;
		tmp_p = ptr_b[lo];
		ptr_b[lo] = ptr_b[hi];
		ptr_b[hi] = tmp_p;
	}
	tmp_p = ptr_b[lo];
	ptr_b[lo] = ptr_b[size - 1];
	ptr_b[size - 1] = tmp_p;
	ft_qsort(ptr_b, lo, cmp);
	ft_qsort(ptr_b + lo + 1, size - lo - 1, cmp);
}

int		update_termsize(void)
{
	struct winsize w;

	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &w))
		return (1);
	termsize.col = w.ws_col;
	termsize.row = w.ws_row;
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
		dprintf(2, "%s: one of the capability is missing\n", PROGRAM_NAME);
		return (1);
	}
	return (0);
}

int		init_datainfo(int ac, char **av)
{
	tputs(tgetstr("ti"), 0, putchar_);
	dsize = ac - 1;
}

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

void	init_termcap(void)
{
	caps[CM] = tgetstr("cm", 0); // placing relative screen cursor
	caps[TI] = tgetstr("ti", 0); // save term state
	caps[TE] = tgetstr("te", 0); // restore term state
	caps[NC] = tgetstr("NC", 0); // customize line// man terminfo NC
}

int		test_termcaps(void)
{
	int	r;

	r = 0;
	init_termcap();
	while (r < CAPNO)
		if (!caps[r++])
			return (1);
	return (0);
}

/* void	init_sigaction(void) */
/* { */
/* 	struct sigaction s; */

/* 	sigfillset(&s.sa_mask); */
/* 	s.sa_flags = SA_RESTART; */
/* 	s.sa_handler */
/* } */

int		init_datainfo(int ac, char **av)
{
	t_datainfo	*buf;
	int			i;
	char		*s;

	if (ac == 1)
		return (1);
	if ((buf = (t_datainfo *)malloc(sizeof(*buf), ac - 1)) == (t_datainfo *)0)
		return (1);
	i = 0;
	dsize = ac - 1;
	while (i < dsize)
	{
		s = av[i + 1];
		buf[i].s = s;
		buf[i].len = strlen(s);
		buf[i].state = SL_ALIVE;
		i++;
	}
	datainfo = buf;
	return (0);
}

t_infodata	**init_column_infodata(ssize_t maxno)
{
	t_infodata	**buf;
	int			i;

	if ((buf = (t_infodata **)malloc(sizeof(*buf) * maxno)) == (t_infodata **)0)
	{
		exit(1);
	}
	while (i < maxno)
		buf[i++] = (t_infodata *)0;
	return (buf);
}

int		init_column(void)
{
	int		i;
	int		maxno;

	if ((column = malloc(sizeof(*column) * dsize)) == 0)
		return (1);
	i = 0;
	while (i < dsize)
	{
		maxno = (dsize / (i + 1)) + 1;
		column[i].min_width = 0;
		column[i].maxno = maxno;
		column[i].infosize = 0;
		if ((column[i].info = init_column_infodata(maxno)) == 0)
		{
			dprintf(STDERR_FILENO, "%s: cannot allocate enough memory for select mode, aborting now\n", PROGRAM_NAME);
			return (1);
		}
	}
	return (0);
}

int		set_column_infodata(void)
{
	
}

int		main(int ac, char **av)
{
	uint64_t			k;

	ft_qsort(av + 1, ac - 1, strcmp);
	if (init_non_canon())
		return (1);
	if (init_select_mode() || init_datainfo(ac, av) || init_column())
	{
		restore_terminal();
		return (1);
	}
	/* init_sigaction(); */
	while (0xdeadbeef)
	{
		k = 0;
		read(STDIN_FILENO, &k, sizeof(k));
		key_event(k);
		refresh_column();
		refresh_screen();
	}
	tputs(caps[TE], 0, putchar_);
	return (0);
}
