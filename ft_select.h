/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/23 16:22:10 by jye               #+#    #+#             */
/*   Updated: 2017/10/29 20:39:15 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SELECT_H
# define FT_SELECT_H

# include <unistd.h>

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
# define SL_DELETE		0x4

typedef struct	s_column
{
	int			pad;
	int			maxno;
	int			info_size;
	t_datainfo	**info;
}				t_column;

typedef struct	s_datainfo
{
	int			attribute;
	char		*s;
}

typedef struct	s_xy
{
	int		x:16;
	int		y:16;
}				t_xy;

static inline int putchar_(int c)
{
	return (write(STDERR_FILENO, &c, 1));
}	
	
#endif
