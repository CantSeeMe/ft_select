/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 00:51:02 by jye               #+#    #+#             */
/*   Updated: 2017/11/22 07:21:23 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"
#include "ft_printf.h"
#include "libft.h"

int			init_datainfo(int ac, char **av)
{
	t_datainfo	*buf;
	int			i;
	char		*s;

	if (ac == 1)
		return (1);
	if ((buf = (t_datainfo *)malloc(sizeof(*buf) * (ac - 1))) == 0)
		return (1);
	i = 0;
	g_maxinfo = ac - 1;
	g_eleminfo = g_maxinfo;
	while (i < g_maxinfo)
	{
		s = av[i + 1];
		buf[i].s = s;
		buf[i].len = ft_strlen(s);
		buf[i].state = SL_ALIVE;
		i++;
	}
	g_datainfo = buf;
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

int			set_column_infodata(void)
{
	int		i;
	int		info;
	int		curmax;

	curmax = g_termsize.row - (g_winhelp * MIN_ROW_WINHELP);
	i = 0;
	info = 0;
	g_ncolumn = 0;
	while (i < g_maxinfo)
	{
		if ((g_datainfo[i].state & SL_ALIVE))
		{
			g_column[g_ncolumn].info[info++] = g_datainfo + i;
			if (info >= curmax)
			{
				g_column[g_ncolumn++].info_size = info;
				info = 0;
			}
		}
		i++;
	}
	if (info % curmax)
		g_column[g_ncolumn++].info_size = info;
	return (curmax);
}

int			init_column(void)
{
	int		i;
	int		maxno;

	if ((g_column = (t_column *)malloc(sizeof(*g_column) * g_maxinfo)) == 0)
		return (1);
	i = 0;
	while (i < g_maxinfo)
	{
		maxno = (g_maxinfo / (i + 1));
		g_column[i].maxno = maxno;
		g_column[i].info_size = 0;
		if ((g_column[i].info = init_column_infodata(maxno)) == 0)
		{
			ft_dprintf(STDERR_FILENO,
					"%s: cannot allocate enough memory for"
					" select mode, aborting now\n", PROGRAM_NAME);
			return (1);
		}
		i++;
	}
	return (0);
}
