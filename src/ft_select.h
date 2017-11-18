/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 16:22:10 by jye               #+#    #+#             */
/*   Updated: 2017/11/18 02:42:36 by jye              ###   ########.fr       */
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
# define KEY_ESC			0x1bL
# define KEY_DELETE			0x7e335b1bL

# define SL_ADD			00
# define SL_REMOVE		01
# define SL_CURSOR		0x1
# define SL_SELECTED	0x2
# define SL_ALIVE		0x4

# define MIN_COL_WINHELP	67
# define MIN_ROW_WINHELP	7

# define WINHELP_PAGE_ROW	5
# define WINHELP_PAGE_COL	33

# define CAPNO		12

/*
** CM set cursor address
** TI save terminal state
** TE restore terminal sate
** ME reset cursor attribute
** MB blink
** MD double bright
** MR reverse
** SO standout
** VI invisible cursor
** VS visible cursor
** clear screen
*/

# define CM		0
# define TI		1
# define TE		2
# define ME		3
# define MB		4
# define MD		5
# define MR		6
# define US		7
# define SO		8
# define VI		9
# define VS		10
# define CD		11

# define NONE	""

# define TSETC(row, col) ft_dprintf(2, "%s", tgoto(g_caps[CM], (col), (row)))

typedef struct			s_datainfo
{
	int			state;
	int			len;
	char		*s;
}						t_datainfo;

typedef struct			s_column
{
	int			maxno;
	int			info_size;
	t_datainfo	**info;
}						t_column;

typedef struct			s_rc
{
	int		col:16;
	int		row:16;
}						t_rc;

extern struct termios	g_otermios;
extern t_column			*g_column;
extern t_datainfo		*g_datainfo;
extern char				*g_caps[CAPNO];

extern int				g_cur_col;
extern int				g_cur_row;

extern int				g_winhelp;
extern int				g_maxinfo;
extern int				g_eleminfo;
extern int				g_selected;
extern int				g_ncolumn;

extern t_rc				g_termsize;

void					select_move_up(void);
void					select_move_right(void);
void					select_move_left(void);
void					select_move_down(void);
void					select_current(void);
void					select_quit(void);
void					delete_current(void);
void					print_selected(void);
void					done(void);

void					update_page(void);
void					set_help_windows(void);
int						update_termsize(void);
void					start_select_mode(int reset);
int						end_select_mode(void);
void					signal_handle(void);

void					ft_qsort(void **ptr_b, ssize_t size, int (*cmp)());

void					select_refresh(t_datainfo *info,
								int options, int state);
void					select_output(t_column *col);
int						init_datainfo(int ac, char **av);
t_datainfo				**init_column_infodata(ssize_t maxno);
int						set_column_infodata(void);
int						init_column(void);
int						init_select_mode(void);
int						init_non_canon(void);

#endif
