/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 16:22:10 by jye               #+#    #+#             */
/*   Updated: 2017/11/14 00:19:11 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

# include <unistd.h>
# include <stdlib.h>

# define PROGRAM_NAME "ft_select"

# define MIN_WIDTH	2

# define KEY_ARROW_UP		0x415b1bL
# define KEY_ARROW_DOWN		0x425b1bL
# define KEY_ARROW_RIGHT	0x435b1bL
# define KEY_ARROW_LEFT		0x445b1bL
# define KEY_SPACEBAR		0x20L
# define KEY_BACKSPACE		0x7fL
# define KEY_DELETE			0x7e335b1bL

# define SL_CURSOR		0x1
# define SL_SELECTED	0x2
# define SL_ALIVE		0x3

# define MIN_COL_WINHELP	26
# define MIN_ROW_WINHELP	4

# define CAPNO		4

# define CM		0
# define NC		1
# define TI		2
# define TE		3

# define NONE	""

# define A_NORMAL		0x000
# define A_STANDOUT		0x001
# define A_UNDERLINE	0x002
# define A_REVERSE		0x004
# define A_BLINK		0x008
# define A_DIM			0x010
# define A_BOLD			0x020
# define A_INVIS		0x040
# define A_PROTECT		0x080
# define A_ALTCHARSET	0x100

# define TSETCURSOR(row, col) tputs(tgoto(caps[CM], (row), (col)), 0, putchar_)

typedef struct	s_column
{
	int			maxno;
	int			info_size;
	t_datainfo	**info;
}				t_column;

typedef struct	s_datainfo
{
	int			state;
	int			len;
	char		*s;
}				t_datainfo;

typedef struct	s_rc
{
	int		col:16;
	int		row:16;
}				t_rc;

static inline int putchar_(int c)
{
	return (write(STDERR_FILENO, &c, 1));
}
	
#endif
