/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 16:22:29 by jye               #+#    #+#             */
/*   Updated: 2017/11/17 15:34:58 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <term.h>
#include <curses.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string.h>

#include "ft_select.h"

struct termios	g_otermios;
t_column		*column;
t_datainfo		*datainfo;
char		 	*caps[CAPNO];

int				cur_col;
int				cur_row;

int				winhelp;
int				maxinfo;
int				eleminfo;
int				ncolumn;
int				selected;

t_rc			termsize;

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
		while (lo < hi && cmp(ptr_b[hi], ptr_b[size - 1]) >= 0)
			hi--;
		if (lo >= hi)
			break ;
		tmp_p = ptr_b[lo];
		ptr_b[lo] = ptr_b[hi];
		ptr_b[hi--] = tmp_p;
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

static char *termcapid[CAPNO] = {"cm", "ti", "te", "me", "mb", "md",
								 "mr", "us", "so", "vi", "vs", "cd"};

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

int		end_select_mode(void)
{
	dprintf(2, "%s", caps[TE]);
	dprintf(2, "%s", caps[VS]);
	return (tcsetattr(STDIN_FILENO, TCSANOW, &g_otermios));
}

void	start_select_mode(void)
{
	dprintf(2, "%s", caps[TI]);
	column[cur_col].info[cur_row]->state |= SL_CURSOR;
	set_help_windows();
	select_output(column + cur_col);
	dprintf(2, "%s", caps[VI]);
}

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
	eleminfo = maxinfo;
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

int		set_column_infodata(void)
{
	int		i;
	int		info;
	int		curmax;
	int		min_width;

	curmax = termsize.row - (winhelp * MIN_ROW_WINHELP);
	i = 0;
	min_width = 0;
	info = 0;
	ncolumn = 0;
	while (i < maxinfo)
	{
		if ((datainfo[i].state & SL_ALIVE))
		{
			column[ncolumn].info[info++] = datainfo + i;
			if (min_width < datainfo[i].len)
				min_width = datainfo[i].len;
			if (info >= curmax)
			{
				column[ncolumn].info_size = info;
				column[ncolumn++].min_width = min_width;
				min_width = 0;
				info = 0;
			}
		}
		i++;
 	}
	if (info < curmax)
	{
		column[ncolumn].info_size = info;
		column[ncolumn].min_width = min_width;
		ncolumn += 1;
	}
	return (curmax);
}

int		can_set_winhelp(void)
{
	return (termsize.col >= MIN_COL_WINHELP && termsize.row >= MIN_ROW_WINHELP + 3);
}

void	set_help_windows(void)
{
	if (!(winhelp = can_set_winhelp()))
		return ;
	TSETCURSOR(0, 0);
	dprintf(2, "******************************************************************\n");
	dprintf(2, "* backspace / delete: remove current      arrow up   : move up   *\n");
	dprintf(2, "* space             : select current      arrow down : move down *\n");
	dprintf(2, "* enter             : print selected      arrow right: next page *\n");
	dprintf(2, "* q / esc           : quit                arrow left : prev page *\n");
	dprintf(2, "*                          page:                                 *\n");
	dprintf(2, "******************************************************************\n");
	update_page();
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
	set_column_infodata();
	return (0);
}

void	output(t_datainfo *info)
{
	int		state;

	state = info->state;
	if (state & SL_SELECTED)
		dprintf(2, "%s", caps[MR]);
	if (state & SL_CURSOR)
		dprintf(2, "%s", caps[US]);
	if (termsize.col < info->len)
		dprintf(2, "%.*s%.*s%s",
				(termsize.col > 3) * (termsize.col - 3), info->s,
				termsize.col < 3 ? termsize.col % 3 : 3, "...",
				caps[ME]);
	else
		dprintf(2, "%s%s", info->s, caps[ME]);
}

void	select_output(t_column *col)
{
	int			max;
	int			i;
	int			row;

	max = col->info_size;
	i = 0;
	row = winhelp * MIN_ROW_WINHELP;
	TSETCURSOR(row, 0);
	dprintf(1, "%s", caps[CD]);
	while (i < max)
	{
		TSETCURSOR(row + i, 0);
		output(col->info[i++]);
	}
}

void	select_refresh(t_datainfo *info, int options, int state)
{
	state = options ? ~state & info->state : state | info->state;
	info->state = state;
	TSETCURSOR(cur_row + (MIN_ROW_WINHELP * winhelp), 0);
	output(info);
}

void	key_event(uint64_t k)
{
	if (k == KEY_BACKSPACE)
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
	else if (k == KEY_DELETE)
		delete_current();
}

int		main(int ac, char **av)
{
	uint64_t			k;

	ft_qsort((void **)(av + 1), ac - 1, strcmp);
	if (init_non_canon())
		return (1);
	if (init_select_mode() || init_datainfo(ac, av) || init_column())
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &g_otermios);
		return (1);
	}
	start_select_mode();
	signal_handle();
	while (eleminfo)
	{
		k = 0;
		read(STDIN_FILENO, &k, sizeof(k));
		key_event(k);
	}
	end_select_mode();
	print_selected();
	return (0);
}
