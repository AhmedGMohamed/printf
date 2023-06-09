#include "main.h"
/**
 * print_char - replaces format specifier with formatted input in the buffer
 * @types: a list of arguments supplied with the printf function
 * @buffer: the buffer to output the text to
 * @i: the current index in the buffer
 * @flags: a summed number representing flags supplied in the format specifier
 * @width: the padding to output to the buffer
 * @precision: the precision value sub-specifer supplied in the format spefier
 * @size: a number specifying long or short sizes
 *
 * Return: printed - the number of printed characters to the buffer
 */
int print_char(va_list types, char buffer[], int *i,
	int flags, int width, int precision, int size)
{
	int printed = 1, it;

	UNUSED(precision);
	UNUSED(size);
	if (width)
	{
		if ((F_MINUS & flags) == F_MINUS)
		{
			buffer[(*i)++] = (char)va_arg(types, int);
			for (it = 0; it < width - 1; it++, (*i)++, printed++)
				buffer[*i] = ' ';
		}
		else
		{
			for (it = 0; it < width - 1; it++, (*i)++, printed++)
				buffer[*i] = ' ';
			buffer[(*i)++] = (char)va_arg(types, int);
		}
	}
	else
		buffer[(*i)++] = (char)va_arg(types, int);
	return (printed);
}
/**
 * print_string - Replaces string format specifier with formated string
 * @types: a list of arguments supplied with the printf function
 * @buffer: the buffer to output the text to
 * @i: the current index in the buffer
 * @flags: a summed number representing flags supplied in the format specifier
 * @width: the padding to output to the buffer
 * @precision: the precision value sub-specifer supplied in the format spefier
 * @size: a number specifying long or short sizes
 *
 * Return: printed - the number of printed characters to the buffer
 */
int print_string(va_list types, char buffer[], int *i,
	int flags, int width, int precision, int size)
{
	int printed = 0, len = 0, it, j = 0;
	char *final = va_arg(types, char *), tmp_buff[BUFF_SIZE];

	UNUSED(size);
	if (final == NULL)
		final = "(null)";
	while (*final != '\0')
	{
		if (*final == '\\')
			convert_escape(++final, (tmp_buff + j));
		else
			tmp_buff[j] = *final;
		final++, len++, j++;
	}
	(precision < len && precision != -1) ? (len = precision) : len;
	if (width - len > 0)
	{
		if ((F_MINUS & flags) == F_MINUS)
		{
			for (it = 0; it < len; it++, printed++)
				buffer[(*i)++] = tmp_buff[it];
			for (it = 0; it < width - len; it++, printed++)
				buffer[(*i)++] = ' ';
		}
		else
		{
			for (it = 0; it < width - len; it++, printed++)
				buffer[(*i)++] = ' ';
			for (it = 0; it < len; it++, printed++)
				buffer[(*i)++] = tmp_buff[it];
		}
	}
	else
		for (it = 0; it < len; it++, printed++)
			buffer[(*i)++] = tmp_buff[it];
	return (printed);
}
/**
 * print_int - Replaces integer format specifier with formated number
 * @types: a list of arguments supplied with the printf function
 * @buffer: the buffer to output the text to
 * @i: the current index in the buffer
 * @flags: a summed number representing flags supplied in the format specifier
 * @width: the padding to output to the buffer
 * @precision: the precision value sub-specifer supplied in the format spefier
 * @size: a number specifying long or short sizes
 *
 * Return: printed - the number of printed characters to the buffer
 */
int print_int(va_list types, char buffer[], int *i,
		int flags, int width, int precision, int size)
{
	char tmp_buff[BUFF_SIZE] = { '0' }, sign = '+', padd = ' ';
	int num = va_arg(types, int), tmp = num, len = 0, it, printed = 0;

	UNUSED(size);
	if (num < 0)
		sign = '-';
	if ((F_ZERO & flags) && !(F_MINUS & flags))
		padd = '0';
	num == 0 ? len++ : len;
	while (tmp)
		tmp_buff[len] = ABS((tmp % 10)) + '0', tmp /= 10, len++;
	if (precision != -1)
	{
		padd = '0', width = precision;
		F_PLUS ? width++ : width;
	}
	if (F_MINUS & flags && precision == -1)
	{
		if ((F_PLUS & flags) || (F_SPACE & flags) || num < 0)
			buffer[(*i)++] = sign, printed++, len++;
		for (it = len - 1; it >= 0; it--)
			buffer[(*i)++] = tmp_buff[it], printed++;
		for (it = 0; it < width - len; it++)
			buffer[(*i)++] = padd, printed++;
		return (printed);
	}
	if (((F_ZERO & flags) || precision != -1) &&
		((F_PLUS & flags) || num < 0))
		buffer[(*i)++] = sign, printed++, len++;
	for (it = 0; it < width - len; it++)
		buffer[(*i)++] = padd, printed++;
	if (((F_PLUS & flags) || (F_SPACE & flags) || num < 0) &&
		!((F_ZERO & flags) || precision != -1))
		buffer[(*i) - 1] = sign;
	for (it = len - 1; it >= 0; it--)
		buffer[(*i)++] = tmp_buff[it], printed++;
	return (printed);
}
/**
 * print_binary - converts and prints an unsigned int as binary
 * @types: a list of arguments supplied with the printf function
 * @buffer: the buffer to output the text to
 * @i: the current index in the buffer
 * @flags: a summed number representing flags supplied in the format specifier
 * @width: the padding to output to the buffer
 * @precision: the precision value sub-specifer supplied in the format spefier
 * @size: a number specifying long or short sizes
 *
 * Return: printed - the number of printed characters to the buffer
 */
int print_binary(va_list types, char buffer[], int *i,
	int flags, int width, int precision, int size)
{
	char tmp_buff[33];
	int len = 0, printed = 0;
	unsigned int num = va_arg(types, unsigned int);

	UNUSED(flags);
	UNUSED(width);
	UNUSED(precision);
	UNUSED(size);
	if (num == 0)
	{
		tmp_buff[0] = '0';
		len++;
		goto print_to_buff;
	}
	while (num > 0)
	{
		tmp_buff[len] = num % 2 + '0';
		num /= 2, len++;
	}
print_to_buff:
	for (len = len - 1; len >= 0; len--)
	{
		buffer[(*i)++] = tmp_buff[len];
		printed++;
	}
	return (printed);
}
