/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 16:22:29 by jye               #+#    #+#             */
/*   Updated: 2017/11/15 05:52:25 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

#include <termios.h>
#include <term.h>
#include <curses.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>

struct termios	g_otermios;
t_column		*column;
t_datainfo		*datainfo;
char		 	*caps[CAPNO];

int				cur_col;
int				cur_row;

int				winhelp;
int				maxinfo;
int				ncolumn;

t_rc           termsize;

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
	if (tcsetattr(STDIN_FILENO, TCSANOW, &termios))
	{
		dprintf(2, "%s: could not set termios on stdin\n", PROGRAM_NAME);
		return (1);
	}
	return (0);
}

static char *termcapid[CAPNO] = {"cm", "ti", "te", "me", "mb", "md" ,
								  "mr", "us", "so"};

void	init_termcap(void)
{
	int		i;

	i = 0;
	while (i < CAPNO)
	{
		caps[i] = tgetstr(termcapid[i], 0);
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
		dprintf(1, "%d\n", r);
		if (!caps[r++])
			return (1);
	}
	return (0);
}

int		init_select_mode(void)
{
	if (tgetent(getenv("TERM"), 0) <= 0)
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

int		restore_terminal(void)
{
	tputs(caps[TE], 0, putchar_);
	return (tcsetattr(STDIN_FILENO, TCSANOW, &g_otermios));
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
	if ((buf = (t_datainfo *)malloc(sizeof(*buf) * (ac - 1))) == (t_datainfo *)0)
		return (1);
	i = 0;
	maxinfo = ac - 1;
	while (i < maxinfo)
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

t_datainfo	**init_column_infodata(ssize_t maxno)
{
	t_datainfo	**buf;
	int			i;

	if ((buf = (t_datainfo **)malloc(sizeof(*buf) * maxno)) == (t_datainfo **)0)
		return ((t_datainfo **)0);
	i = 0;
	while (i < maxno)
		buf[i++] = (t_datainfo *)0;
	return (buf);
}

void	reset_column_infodata(void)
{
	int		i;

	i = 0;
	while (i < ncolumn)
		column[i++].info_size = 0;
	ncolumn = 0;
}

void	set_column_infodata(void)
{
	int		i;
	int		info;
	int		curmax;
	int		min_width;

	curmax = (maxinfo / termsize.col) - (winhelp * MIN_ROW_WINHELP);
	i = 0;
	min_width = 0;
	info = 0;
	while (i < maxinfo)
	{
		column[ncolumn].info_size++;
		column[ncolumn].info[info++] = datainfo + i;
		if (min_width < datainfo[i].len)
			min_width = datainfo[i].len;
		if (info == curmax)
		{
			column[ncolumn++].min_width = min_width;
			min_width = 0;
			info = 0;
		}
		i++;
 	}
	if (info != curmax)
		column[ncolumn].min_width = min_width;
}

int		init_column(void)
{
	int		i;
	int		maxno;

	if ((column = (t_column *)malloc(sizeof(*column) * maxinfo)) == (t_column *)0)
		return (1);
	i = 0;
	while (i < maxinfo)
	{
		maxno = (maxinfo / (i + 1));
		column[i].maxno = maxno;
		column[i].info_size = 0;
		if ((column[i].info = init_column_infodata(maxno)) == 0)
		{
			dprintf(STDERR_FILENO, "%s: cannot allocate enough memory for select mode, aborting now\n", PROGRAM_NAME);
			return (1);
		}
		i++;
	}
//	set_help_windows();
	set_column_infodata();
	return (0);
}

int		can_set_winhelp(void)
{
	return (termsize.col < MIN_COL_WINHELP && termsize.row < MIN_ROW_WINHELP);
}

void	select_output(int col)
{
	t_datainfo	*t;
	int			max;
	int			i;
	int			row;

	max = column[col].info_size;
	i = 0;
	row = winhelp * MIN_ROW_WINHELP;
	TSETCURSOR(row, 0);
	while (i < max)
	{
		t = column[col].info[i];
		dprintf(2, "%s%s%s\n", caps[US], t->s, caps[ME]);
		i++;
	}
}

void	select_refresh(int col, int row)
{
	
}

int		main(int ac, char **av)
{
	uint64_t			k;

	ft_qsort((void **)(av + 1), ac - 1, strcmp);
	if (init_non_canon())
		return (1);
	if (init_select_mode() || init_datainfo(ac, av) || init_column())
	{
		restore_terminal();
		return (1);
	}
	tputs(caps[TI], 0, putchar_);
	select_output(0);
	/* while (0xdeadbeef) */
	/* { */
	/* 	k = 0; */
	/* 	read(STDIN_FILENO, &k, sizeof(k)); */
	/* 	key_event(k); */
	/* 	refresh_column(); */
	/* 	refresh_screen(); */
	/* } */
	sleep(2);
	restore_terminal();
	return (0);
}
