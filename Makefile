NAME = diasm

FILE_C = main.c\
		op_live_zjmp_fork_fork_add_sub_aff_and_or_xor.c\
		op_ls_lld_st_sti_ldi_lldi.c\
		print_bit_hex.c\
		read_byts.c\
		write_name_comment.c\
		write_to_file.c 

FILE_LIB_C = ft_itoa.c\
			 ft_memccpy.c\
			 ft_putchar_fd.c\
			 ft_putendl.c\
			 ft_putendl_fd.c\
			 ft_strcat.c\
			 ft_strdel.c\
			 ft_strdup.c\
			 ft_strlen.c\
			 ft_strnew.c\
			 ft_strstr.c\
			 ft_memset.c\
			 ft_putchar.c\
			 ft_putstr.c\
			 ft_putstr_fd.c\
			 sys_err.c


FLAGS = -Wall -Wextra -Werror -I include -I $(DIRMLIB)

DIRMLIB = ./minilibft/

DIRC = ./source/

VPATH = $(DIROBJ)

DIROBJ = ./obj/

OBJ = $(addprefix $(DIROBJ), $(FILE_C:.c=.o))

OBJLIB = $(addprefix $(DIROBJ), $(FILE_LIB_C:.c=.o))

all : $(NAME)

$(NAME): $(DIROBJ) $(OBJ) $(OBJLIB) 
	gcc $(FLAGS) $(OBJ) $(OBJLIB) $(FMLXLIB) -o $(NAME)

$(DIROBJ)%.o : $(DIRC)%.c
	gcc -g $(FLAGS) -c $< -o $@

$(DIROBJ)%.o : $(DIRMLIB)%.c
	gcc -g $(FLAGS) -c $< -o $@

$(DIROBJ):
	mkdir -p $(DIROBJ)

clean:
	/bin/rm -rf $(DIROBJ)
	
fclean: clean
	/bin/rm -f $(NAME)
	
re: fclean all 
