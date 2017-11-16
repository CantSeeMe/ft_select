/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 16:22:10 by jye               #+#    #+#             */
/*   Updated: 2017/11/16 11:44:45 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

# include <unistd.h>
# include <stdlib.h>

# define PROGRAM_NAME "ft_select"

# define MIN_WIDTH	2

# undef KEY_ENTER
# undef KEY_BACKSPACE

# define KEY_ARROW_UP		0x415b1bL
# define KEY_ARROW_DOWN		0x425b1bL
# define KEY_ARROW_RIGHT	0x435b1bL
# define KEY_ARROW_LEFT		0x445b1bL
# define KEY_SPACEBAR		0x20L
# define KEY_BACKSPACE		0x7fL
# define KEY_ENTER			0x0aL
# define KEY_DELETE			0x7e335b1bL

# define SL_ADD			00
# define SL_REMOVE		01
# define SL_CURSOR		0x1
# define SL_SELECTED	0x2
# define SL_ALIVE		0x4

# define MIN_COL_WINHELP	26
# define MIN_ROW_WINHELP	4

# define CAPNO		12

# define CM		0
# define TI		1
# define TE		2
# define ME		3 // reset
# define MB		4 // blink
# define MD		5 // double bright
# define MR		6 // reverse
# define US		7 // underline
# define SO		8 // standout
# define VI		9 // invisible cursor
# define VS		10 // visible cursorx
# define CD		11 // clear screen

# define NONE	""

# define TSETCURSOR(row, col) dprintf(2, "%s", tgoto(caps[CM], (col), (row)));

typedef struct	s_datainfo
{
	int			state;
	int			len;
	char		*s;
}				t_datainfo;

typedef struct	s_column
{
	int			maxno;
	int			min_width;
	int			info_size;
	t_datainfo	**info;
}				t_column;

typedef struct	s_rc
{
	int		col:16;
	int		row:16;
}				t_rc;

extern struct termios	g_otermios;
extern t_column			*column;
extern t_datainfo		*datainfo;
extern char		 		*caps[CAPNO];

extern int				cur_col;
extern int				cur_row;

extern int				winhelp;
extern int				maxinfo;
extern int				eleminfo;
extern int				selected;
extern int				ncolumn;

extern t_rc				termsize;

void	select_move_up(void);
void	select_move_right(void);
void	select_move_left(void);
void	select_move_down(void);
void	select_current(void);
void	delete_current(void);
void	done(void);

void	select_refresh(t_datainfo *info, int options, int state);
void	set_column_infodata(void);
void	select_output(t_column *col);

#endif
