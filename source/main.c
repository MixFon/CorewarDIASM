
#include "../include/diasm.h"

void	init(t_diasm *diasm)
{
	diasm->name_cor = NULL;
	diasm->fd_cor = -1;
	diasm->fd_s = -1;
}

void	open_file_cor(t_diasm *diasm, char *name)
{
	char *tmp;
	
	if ((tmp = ft_strstr(name, ".cor")) && (name - tmp) != 0
			&& ft_strlen(tmp) == 4)
	{
		if(!(diasm->name_cor = ft_strdup(name)))
			sys_err("Error malloc.");
		if ((diasm->fd_cor = open(name, O_RDONLY)) == -1)
			sys_err("File not opened.\n");
		ft_printf("fd_cor = [%d]\n", diasm->fd_cor);
	}
	else
		sys_err("Invalid file type.\nUse ./asm namefile.cor\n");
}

char	*dot_s(char *name)
{
	char	*dot_cor;
	size_t	len;

	len = ft_strlen(name);
	dot_cor = ft_strnew(len);
	ft_memccpy(dot_cor, name, '.', len);
	ft_strcat(dot_cor, "dis");
	return (dot_cor);
}

void	create_file_s(t_diasm *diasm)
{
	diasm->name_s = dot_s(diasm->name_cor);
	if (!(diasm->fd_s = open(diasm->name_s, O_WRONLY | O_TRUNC | O_CREAT,
					S_IREAD | S_IWRITE)))
		sys_err("Error create file.\n");

}

void	printf_bit_hex(unsigned char *arr)
{
	unsigned char	c;
	int				i;

	i = -1;
	while (++i != 4)
		ft_printf("%02x ", (unsigned char)arr[i]);
}

void	check_exec_magic(char *buf)
{
	unsigned int	exec;
	int				i;

	exec = 0;
	i = -1;
	while(++i < 4)
	{
		exec = exec << 8;
		exec = exec | (unsigned char)buf[i];
		//ft_printf("{%08x}", exec);
		//ft_printf("[%x]\n", (unsigned char)buf[i]);
	}
	if (exec != COREWAR_EXEC_MAGIC)
		sys_err("Wrong exec magic\n");
	//ft_printf("exec = [%u] {%08x}\n", exec, exec);
}

void	write_name(t_diasm *diasm)
{
	char	name[PROG_NAME_LENGTH + 8];

	if (read(diasm->fd_cor, name, PROG_NAME_LENGTH + 8) == -1)
		sys_err("Error read files.\n");
	ft_putstr_fd(".name \"", diasm->fd_s);
	ft_putstr_fd(name, diasm->fd_s);
	ft_putendl_fd("\"", diasm->fd_s);
	//ft_printf("name = {%s}\n", name);
}

void	write_comment(t_diasm *diasm)
{
	char	comment[COMMENT_LENGTH + 4];

	if (read(diasm->fd_cor, comment, COMMENT_LENGTH + 4) == -1)
		sys_err("Error read files.\n");
	//ft_printf("comment = {%s}\n", comment);
	ft_putstr_fd(".comment \"", diasm->fd_s);
	ft_putstr_fd(comment, diasm->fd_s);
	ft_putendl_fd("\"\n", diasm->fd_s);

}
void	write_name_comment(t_diasm *diasm)
{
	char buf[4];

	if (read(diasm->fd_cor, buf, 4) == -1)
		sys_err("Error read files.\n");
	check_exec_magic(buf);
	write_name(diasm);
	write_comment(diasm);
}

unsigned int	move_to_uint(unsigned char *buf)
{
	unsigned int	rez;
	int				i;

	rez = 0;
	i = -1;
	while(++i < 4)
	{
		rez = rez << 8;
		rez = rez | (unsigned char)buf[i];
	}
	return (rez);
}

unsigned short	move_to_ushort(unsigned char *buf)
{
	unsigned short	rez;
	int				i;

	rez = 0;
	i = -1;
	while(++i < 2)
	{
		rez = rez << 8;
		rez = rez | (unsigned char)buf[i];
	}
	return (rez);
}

unsigned char read_one_byte(t_diasm *diasm)
{
	unsigned int	one_byte;

	if (read(diasm->fd_cor, &one_byte, 1) == -1)
		sys_err("Error read files.\n");
	//ft_printf("Code arg = {%#02x}\n", code_arg);
	return (one_byte);
}

unsigned int read_four_byte(t_diasm *diasm)
{
	unsigned int	four_byte;
	unsigned char	buf[4];

	if (read(diasm->fd_cor, buf, 4) == -1)
		sys_err("Error read files.\n");
	four_byte = move_to_uint(buf);
	return (four_byte);
}

unsigned short read_two_byte(t_diasm *diasm)
{
	unsigned short	tow_byte;
	unsigned char	buf[2];

	if (read(diasm->fd_cor, buf, 2) == -1)
		sys_err("Error read files.\n");
	tow_byte = move_to_ushort(buf);
	return (tow_byte);
	
}

void	write_registr(t_diasm *diasm, int flag)
{
	unsigned char	reg;
	char			*str_num;
	
	reg = read_one_byte(diasm);
	str_num = ft_itoa(reg);
	ft_putchar_fd('r', diasm->fd_s);
	ft_putstr_fd(str_num, diasm->fd_s);
	ft_strdel(&str_num);
	flag == 1 ? ft_putstr_fd(", ", diasm->fd_s) : ft_putstr_fd("\n", diasm->fd_s);
}

void	write_direct(t_diasm *diasm, int flag)
{
	unsigned int	dir;
	char			*str_num;
	
	dir = read_four_byte(diasm);
	str_num = ft_itoa((int)dir);
	ft_putchar_fd('%', diasm->fd_s);
	ft_putstr_fd(str_num, diasm->fd_s);
	ft_strdel(&str_num);
	flag == 1 ? ft_putstr_fd(", ", diasm->fd_s) : ft_putstr_fd("\n", diasm->fd_s);
}

void	write_direct_two_byte(t_diasm *diasm, int flag)
{
	unsigned short	dir;
	char			*str_num;
	
	dir = read_two_byte(diasm);
	str_num = ft_itoa((short)dir);
	ft_putchar_fd('%', diasm->fd_s);
	ft_putstr_fd(str_num, diasm->fd_s);
	ft_strdel(&str_num);
	flag == 1 ? ft_putstr_fd(", ", diasm->fd_s) : ft_putstr_fd("\n", diasm->fd_s);
}

void	write_indirect(t_diasm *diasm, int flag)
{
	unsigned short	ind;
	char			*str_num;
	
	ind = read_two_byte(diasm);
	str_num = ft_itoa((short)ind);
	ft_putstr_fd(str_num, diasm->fd_s);
	ft_strdel(&str_num);
	flag == 1 ? ft_putstr_fd(", ", diasm->fd_s) : ft_putstr_fd("\n", diasm->fd_s);
}

void	op_live(t_diasm *diasm)
{
	unsigned int	dir;
	char			*str_num;

	ft_putstr_fd("\tlive %", diasm->fd_s);
	dir = read_four_byte(diasm);
	str_num = ft_itoa((int)dir);
	ft_putendl_fd(str_num, diasm->fd_s);
	ft_strdel(&str_num);
}

void	op_zjmp_fork_lfork(t_diasm *diasm, const char *op_name)
{
	unsigned short	dir;
	char			*str_num;

	ft_putendl(op_name);
	ft_putstr_fd(op_name, diasm->fd_s);
	dir = read_two_byte(diasm);
	str_num = ft_itoa((short)dir);
	ft_putendl_fd(str_num, diasm->fd_s);
	ft_strdel(&str_num);
}

void	op_add_sub(t_diasm *diasm, char *op_name)
{
	unsigned char code_arg;

	code_arg = read_one_byte(diasm);
	ft_putstr_fd(op_name, diasm->fd_s);
	if (code_arg== 0x54)
	{
		write_registr(diasm, 1);
		write_registr(diasm, 1);
		write_registr(diasm, 0);
	}

}

void	op_aff(t_diasm *diasm)
{
	unsigned char code_arg;

	ft_putstr_fd("\taff ", diasm->fd_s);
	code_arg = read_one_byte(diasm);
	if (code_arg == 0x40)
		write_registr(diasm, 0);

}

void	op_and_or_xor(t_diasm *diasm, char *op_name)
{
	unsigned char code_arg;

	ft_putstr_fd(op_name, diasm->fd_s);
	code_arg = read_one_byte(diasm);
	if ((code_arg & FIR_ARG) == 0x40)
		write_registr(diasm, 1);
	else if ((code_arg & FIR_ARG) == 0x80)
		write_direct(diasm, 1);
	else if ((code_arg & FIR_ARG) == 0xc0)
		write_indirect(diasm, 1);
	if ((code_arg & SEC_ARG) == 0x10)
		write_registr(diasm, 1);
	else if ((code_arg & SEC_ARG) == 0x20)
		write_direct(diasm, 1);
	else if ((code_arg & SEC_ARG) == 0x30)
		write_indirect(diasm, 1);
	if ((code_arg & THI_ARG) == 0x04)
		write_registr(diasm, 0);
}

void	op_ls_lld(t_diasm *diasm, char *op_name)
{
	unsigned char code_arg;

	ft_putstr_fd(op_name, diasm->fd_s);
	code_arg = read_one_byte(diasm);
	if ((code_arg & FIR_ARG) == 0x80)
		write_direct(diasm, 1);
	else if ((code_arg & FIR_ARG) == 0xc0)
		write_indirect(diasm, 1);
	if ((code_arg & SEC_ARG) == 0x10)
		write_registr(diasm, 0);
}

void	op_st(t_diasm *diasm)
{
	unsigned char code_arg;

	ft_putstr_fd("\tst ", diasm->fd_s);
	code_arg = read_one_byte(diasm);
	if ((code_arg & FIR_ARG) == 0x40)
		write_registr(diasm, 1);
	if ((code_arg & SEC_ARG) == 0x10)
		write_registr(diasm, 0);
	else if ((code_arg & SEC_ARG) == 0x30)
		write_indirect(diasm, 0);
}

void	op_sti(t_diasm *diasm)
{
	unsigned char code_arg;

	ft_putstr_fd("\tsti ", diasm->fd_s);
	code_arg = read_one_byte(diasm);
	if ((code_arg & FIR_ARG) == 0x40)
		write_registr(diasm, 1);
	if ((code_arg & SEC_ARG) == 0x10)
		write_registr(diasm, 1);
	else if ((code_arg & SEC_ARG) == 0x20)
		write_direct_two_byte(diasm, 1);
	else if ((code_arg & SEC_ARG) == 0x30)
		write_indirect(diasm, 1);
	if ((code_arg & THI_ARG) == 0x04)
		write_registr(diasm, 0);
	else if ((code_arg & THI_ARG) == 0x08)
		write_direct_two_byte(diasm, 0);
}

void	op_ldi_lldi(t_diasm *diasm, char *op_name)
{
	unsigned char code_arg;

	ft_putstr_fd(op_name, diasm->fd_s);
	code_arg = read_one_byte(diasm);
	if ((code_arg & FIR_ARG) == 0x40)
		write_registr(diasm, 1);
	else if ((code_arg & FIR_ARG) == 0x80)
		write_direct_two_byte(diasm, 1);
	else if ((code_arg & FIR_ARG) == 0xc0)
		write_indirect(diasm, 1);
	if ((code_arg & SEC_ARG) == 0x10)
		write_registr(diasm, 1);
	else if ((code_arg & SEC_ARG) == 0x20)
		write_direct_two_byte(diasm, 1);
	if ((code_arg & THI_ARG) == 0x04)
		write_registr(diasm, 0);
}

void	woking_operation(t_diasm *diasm, unsigned char op)
{
	if (op == 0x01)
		op_live(diasm);
	else if (op == 0x09)
		op_zjmp_fork_lfork(diasm, "\tzjmp %");
	else if (op == 0x0c)
		op_zjmp_fork_lfork(diasm, "\tfork %");
	else if (op == 0x0f)
		op_zjmp_fork_lfork(diasm, "\tlfork %");
	else if (op == 0x04)
		op_add_sub(diasm, "\tadd ");
	else if (op == 0x05)
		op_add_sub(diasm, "\tsub ");
	else if (op == 0x10)
		op_aff(diasm);
	else if (op == 0x06)
		op_and_or_xor(diasm, "\tand ");
	else if (op == 0x07)
		op_and_or_xor(diasm, "\tor ");
	else if (op == 0x08)
		op_and_or_xor(diasm, "\txor ");
	else if (op == 0x02)
		op_ls_lld(diasm, "\tld ");
	else if (op == 0x0d)
		op_ls_lld(diasm, "\tlld ");
	else if (op == 0x03)
		op_st(diasm);
	else if (op == 0x0b)
		op_sti(diasm);
	else if (op == 0x0a)
		op_ldi_lldi(diasm, "\tldi ");
	else if (op == 0x0e)
		op_ldi_lldi(diasm, "\tlidi ");
}

void	write_instruction(t_diasm *diasm)
{
	int				count;
	unsigned char	op;
	
	count = 1;	
	while (1)
	{
		if ((count = read(diasm->fd_cor, &op, 1)) == -1)
			sys_err("Error read files.\n");
		if (!count)
			break;
		woking_operation(diasm, op);
	}
}

void	clone_file(t_diasm *diasm)
{
	close(diasm->fd_cor);
	close(diasm->fd_s);
}

int		main(int ac, char **av)
{
	t_diasm diasm;
	
	if (ac != 2)
		sys_err("Error!\nUse ./asm namefile.cor\n");
	init(&diasm);
	open_file_cor(&diasm, av[1]);
	create_file_s(&diasm);
	write_name_comment(&diasm);
	write_instruction(&diasm);
	clone_file(&diasm);
	//create_file_s;
	return (0);
}
