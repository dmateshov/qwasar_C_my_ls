SRC = my_ls.c

CC = gcc 

CC_OPT =  -g3 -fsanitize=address -Wall -Wextra -Werror

NAME = my_ls


all: $(NAME)

$(NAME):
	$(CC) $(CC_OPT) -o $(NAME) $(SRC)

clean:
	/bin/rm -f $(NAME)

re: clean all
