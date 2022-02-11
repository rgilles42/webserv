/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rush01.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mguerra <mguerra@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/16 11:57:21 by mguerra           #+#    #+#             */
/*   Updated: 2021/10/17 18:28:36 by mguerra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_putchar(char c);

void	fill(int len, char c)
{
	int	col;

	col = -1;
	while (++col < len)
		ft_putchar(c);
}

void	draw_line(int x, char first, char last, char center)
{
	ft_putchar(first);
	fill(x - 2, center);
	if (x > 1)
		ft_putchar(last);
}

int	rush(int x, int y)
{
	int	row;

	row = -1;
	while (++row < y)
	{
		if (row == 0)
			draw_line(x, '/', '\\', '*');
		else if (row == y - 1)
			draw_line(x, '\\', '/', '*');
		else
			draw_line(x, '*', '*', ' ');
		ft_putchar('\n');
	}
	return (0);
}
