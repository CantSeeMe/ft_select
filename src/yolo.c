/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   yolo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <jye@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/18 01:40:49 by jye               #+#    #+#             */
/*   Updated: 2017/11/18 01:51:35 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include "ft_select.h"

struct termios	g_otermios;
t_column		*g_column;
t_datainfo		*g_datainfo;
char			*g_caps[CAPNO];
int				g_cur_col;
int				g_cur_row;
int				g_winhelp;
int				g_maxinfo;
int				g_eleminfo;
int				g_selected;
int				g_ncolumn;
t_rc			g_termsize;
